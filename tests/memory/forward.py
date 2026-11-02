#!/usr/bin/env python3
"""Additional full-allocation and native-instruction checks for store forwarding."""
import ctypes as C
import csv
import random
import re
import subprocess
import run as original
from native_counts import vector_bodies

ROOT=original.ROOT
OUT=ROOT/'build'/'forward-tests'
POSITIVE=['basicForward','multipleReads','multipleStores','longChain','localForward']


def pressure_source(name,count):
    decl=''.join(f'int t{k}[1024];' for k in range(count))
    stores=''.join(f't{k}[i]=a[i]+{k};' for k in range(count))
    total='+'.join(f't{k}[i]' for k in range(count))
    return f'int {name}(int *a,int *b,int *c,int *d,int n){{{decl}int i;for(i=0;i<n;i=i+1){{{stores}c[i]={total};}}return i;}}'


def section(trace,fn):
    return next(s for s in re.split('function=',trace) if s.startswith(fn+' '))


def main():
    OUT.mkdir(parents=True,exist_ok=True);original.OUT=OUT
    source=(ROOT/'examples/memory/forwarding.c').read_text()
    source+='\n'+pressure_source('registers',8)+'\n'+pressure_source('pressure',18)+'\n'+pressure_source('costPressure',12)
    libs,tac=original.build(source,'forwarding')
    functions=POSITIVE+['differentBase','fusionConsumer','registers','pressure','costPressure']
    trials=original.check_functions(libs,functions)
    comparisons=trials//(len(original.MODES)+1)*len(original.MODES)
    counts=[]
    for mode in original.MODES:
        # Inspect LINKED native instructions, keeping vector/tail local symbols.
        lib=OUT/f'count-{mode}.so'
        original.run(['gcc','-shared','-fPIC','-Wa,-L',OUT/f'forwarding-{mode}.s','-o',lib])
        text=original.run(['objdump','-d',lib]).stdout
        (OUT/f'{mode}.objdump').write_text(text)
        for fn in functions:
            bodies=vector_bodies(lib,fn)
            trace=section(original.trace('forwarding',mode),fn)
            for body in bodies:
                counts.append(dict(function=fn,mode=mode,label=body['label'],loads=body['loads'],stores=body['stores'],alu=body['alu'],registers=str(body['registers'])))
            if fn in POSITIVE and mode!='scalar':
                assert len(bodies)==1,(mode,fn,bodies)
                declared=re.search(r'selected=\w+ vector_loads=(\d+) vector_stores=(\d+)',trace)
                assert (bodies[0]['loads'],bodies[0]['stores'])==(int(declared[1]),int(declared[2]))
    lookup={(r['function'],r['mode']):(r['loads'],r['stores']) for r in counts}
    # Counts observed from the actual frontend and native vector body.
    expected={'basicForward':(3,3,2),'multipleReads':(5,3,3),'multipleStores':(4,4,3),
              'longChain':(7,5,4),'localForward':(3,3,2)}
    for fn,(basic,reused,stores) in expected.items():
        assert lookup[fn,'simd']==(basic,stores)
        assert lookup[fn,'reuse']==(reused,stores)
        assert lookup[fn,'forward']==(2,stores)
        assert 'selected=forward ' in section(original.trace('forwarding','auto'),fn)
    assert 'forwarded_loads=0' in section(original.trace('forwarding','forward'),'differentBase')
    assert lookup['differentBase','forward']==lookup['differentBase','reuse']
    pressure=section(original.trace('forwarding','forward'),'pressure')
    assert 'forward_rejected=vector register pressure' in pressure and 'selected=reuse ' in pressure
    registers=section(original.trace('forwarding','forward'),'registers')
    assert 'forwarded_loads=8' in registers and 'selected=forward ' in registers
    # High pressure cost, rather than an illegal allocator requirement, can reject forwarding.
    weights=OUT/'pressure.conf';weights.write_text('pressure=1000\n')
    expensive=original.run([ROOT/'backend/x86_64_backend','--mode','auto',tac,'--weights',weights])
    cost=section(expensive.stderr,'costPressure')
    assert 'candidate=forward ' in cost and 'selected=reuse ' in cost,cost
    # Scalar at a known tiny trip count; actual code changes, not just the trace.
    short=OUT/'short.c';short.write_text(source[:source.index('int multipleReads')].replace('i<n;','i<1;'))
    shorttac=OUT/'short.tac';original.run([ROOT/'tinyc_part3/tinyc3',short,ROOT/'targets/x86_64_tinyc.conf',shorttac])
    shortasm=original.run([ROOT/'backend/x86_64_backend','--mode','auto',shorttac])
    assert 'selected=scalar ' in shortasm.stderr and 'vmovdqu' not in shortasm.stdout
    rejected=original.run([ROOT/'backend/x86_64_backend','--mode','forward',tac,'--alias-policy','reject'])
    assert 'unknown pointer aliasing' in section(rejected.stderr,'basicForward')
    # Count executed batches: identical and shifted aliases must take ORIGINAL TAC;
    # CPU-disabled execution and runtime profitability boundaries are independently tested.
    high=OUT/'setup.conf';high.write_text('setup=1000\n')
    pathchecks=0
    layouts=[[(0,1),(1,1),(2,1),(3,1)],[(0,1),(0,1),(2,1),(3,1)],
             [(0,1),(1,1),(1,1),(3,1)],[(0,1),(1,1),(0,2),(3,1)],
             [(0,2),(1,1),(0,1),(3,1)],[(0,1),(0,2),(2,1),(3,1)]]
    for name,mode,extra in [('forward','forward',[]),('auto','auto',[]),('high','auto',['--weights',high])]:
        compiled=original.run([ROOT/'backend/x86_64_backend','--mode',mode,tac,*extra])
        threshold=int(re.search(r'runtime_threshold=(\d+)',section(compiled.stderr,'basicForward'))[1])
        asm=re.sub(r'(\.LbasicForward_vector\d+:)',r'\1\n incq .Lforwardcount(%rip)',compiled.stdout)
        asm+='\n.bss\n.p2align 3\n.Lforwardcount: .quad 0\n.text\n.globl getcount\ngetcount:\n movq .Lforwardcount(%rip), %rax\n ret\n.globl resetcount\nresetcount:\n movq $0, .Lforwardcount(%rip)\n ret\n.section .note.GNU-stack,"",@progbits\n'
        for cpu in [True,False]:
            path=OUT/f'paths-{name}-{cpu}.s'
            path.write_text(asm if cpu else asm.replace('.Lmemory_cpu_state: .long -1','.Lmemory_cpu_state: .long 0'))
            lib=path.with_suffix('.so');original.run(['gcc','-shared','-fPIC',path,'-o',lib]);dll=C.CDLL(str(lib));dll.getcount.restype=C.c_uint64
            for n in sorted(set(original.LENGTHS+[max(0,threshold-1),threshold,threshold+1])):
                for seed in [19,627,2026]:
                    rng=random.Random(seed)
                    initial=[[rng.randrange(-100,101) for _ in range(2100)] for _ in range(4)]
                    for layout in layouts:
                        expected=None
                        for obj in [libs['gcc'],dll]:
                            arrays=[(C.c_int32*len(a))(*a) for a in initial]
                            fn=obj.basicForward;fn.argtypes=[original.PTR]*4+[C.c_int];fn.restype=C.c_int
                            if obj is dll:dll.resetcount()
                            ret=fn(*[original.ptr(arrays[k],off) for k,off in layout],n)
                            value=(ret,[list(a) for a in arrays]);trials+=1
                            if expected is None:expected=value
                            else:
                                assert value==expected,(name,cpu,n,seed,layout)
                                # For these nontrivial overlapping intervals, any shared
                                # accessed allocation overlaps whenever n >= 8.
                                want=n//8 if cpu and layout==layouts[0] and n>=max(8,threshold) else 0
                                assert dll.getcount()==want,(name,cpu,n,layout,dll.getcount(),want)
                                comparisons+=1;pathchecks+=1
    with (OUT/'instruction-counts.csv').open('w',newline='') as file:
        writer=csv.DictWriter(file,fieldnames=list(counts[0]));writer.writeheader();writer.writerows(counts)
    print(f'PASS: {trials} kernel executions, {comparisons} complete-allocation/reference comparisons; {pathchecks} instrumented path checks.')
    print('Native vector loads (SIMD / reuse / forward), stores retained:')
    for fn in POSITIVE:print(fn,lookup[fn,'simd'],lookup[fn,'reuse'],lookup[fn,'forward'])
    print('Stress fallback, pressure cost rejection, local arrays, fusion interaction, aliases, runtime thresholds, CPU-disabled paths: PASS')

if __name__=='__main__':main()
