#!/usr/bin/env python3
"""Source-to-machine-code application experiments. Compiler/model are never modified."""
import argparse
import csv
from datetime import datetime,timezone
import hashlib
import io
import json
import os
from pathlib import Path
import re
import shutil
import statistics
import subprocess
import sys
import time
from native import inspect

APP=Path(__file__).resolve().parent
ROOT=APP.parents[1]
BUILD=ROOT/'build/applications'
KERNELS={'vector_triad':'vectorTriad','multi_output':'multiOutput','two_stage':'twoStage','in_place_forward':'inPlaceForward'}
MODES=['scalar','simd','reuse','forward','fusion','auto','gcc','gcc_scalar']
EXPLICIT=MODES[:5]
SIZES=[0,1,7,8,9,15,16,17,127,128,1003,1024,8192,32768,262144,1048576]
DYNAMIC_SIZES=[0,1,7,8,9,17,1024,32768,262144,1048576]
GCC_FLAGS=['-std=c11','-O3','-march=native','-fwrapv','-fPIC']
WEIGHTS=ROOT/'benchmarks/default.conf'


def run(args):
    p=subprocess.run([str(x) for x in args],cwd=ROOT,text=True,capture_output=True)
    if p.returncode:raise RuntimeError(f'{args}\n{p.stdout}\n{p.stderr}')
    return p.stdout


def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def write_csv(path,rows):
    path.parent.mkdir(parents=True,exist_ok=True)
    with path.open('w',newline='') as f:
        w=csv.DictWriter(f,fieldnames=list(rows[0]));w.writeheader();w.writerows(rows)


def compile_case(kernel,domain,n):
    folder=BUILD/domain/(str(n) if domain=='static' else 'shared')/kernel
    folder.mkdir(parents=True,exist_ok=True)
    source=(APP/kernel/'kernel.c').read_text()
    if domain=='static':source=source.replace('i<n;',f'i<{n};').replace('temporary[1048576]',f'temporary[{max(1,n)}]')
    key=hashlib.sha256((source+sha(ROOT/'backend/x86_64_backend')+sha(ROOT/'tinyc_part3/tinyc3')+
        WEIGHTS.read_text()+run(['gcc','--version'])+' '.join(GCC_FLAGS)+'application-v1').encode()).hexdigest()
    stamp=folder/'manifest.json'
    if stamp.exists():
        manifest=json.loads(stamp.read_text())
        if manifest['build_key']==key and all((folder/(m+'.so')).exists() and sha(folder/(m+'.so'))==manifest['binaries'][m] for m in MODES):return folder,manifest
    src=folder/'kernel.c';src.write_text(source)
    (folder/'reference.c').write_text((APP/kernel/'reference.c').read_text())
    tac=folder/'kernel.tac'
    (folder/'frontend.log').write_text(run([ROOT/'tinyc_part3/tinyc3',src,ROOT/'targets/x86_64_tinyc.conf',tac]))
    native={}
    for mode in MODES:
        asm=folder/f'{mode}.s'
        if mode.startswith('gcc'):
            flags=GCC_FLAGS+(['-fno-tree-vectorize','-fno-tree-slp-vectorize'] if mode=='gcc_scalar' else [])
            run(['gcc',*flags,'-S','-fverbose-asm',f'-fopt-info-vec-all={folder/(mode+".vectorization.txt")}',
                 f'-fdump-tree-optimized={folder/(mode+".optimized.txt")}',folder/'reference.c','-o',asm])
        else:
            asm.write_text(run([ROOT/'backend/x86_64_backend','--mode',mode,tac,'--weights',WEIGHTS,
                                '--trace',folder/f'{mode}.trace','--dump-ir']))
        binary=folder/f'{mode}.so'
        run(['gcc','-shared','-fPIC','-Wa,-L',asm,'-o',binary])
        (folder/f'{mode}.objdump').write_text(run(['objdump','-d','--no-show-raw-insn',binary]))
        native[mode]=inspect(binary,KERNELS[kernel],not mode.startswith('gcc'))
    manifest={'kernel':kernel,'function':KERNELS[kernel],'domain':domain,'compile_bound':n if domain=='static' else 'runtime_n',
              'build_key':key,'source_sha256':sha(src),'native':native,'binaries':{m:sha(folder/f'{m}.so') for m in MODES}}
    stamp.write_text(json.dumps(manifest,indent=2))
    return folder,manifest


def trace_parts(folder,mode):
    return re.split(r'function=',(folder/f'{mode}.trace').read_text())[1:]


def plan(folder,mode,n):
    if mode.startswith('gcc'):return dict(selected=mode,selected_at_n=mode,cost='',threshold=0)
    selected=[];actual=[];cost=0;threshold=0
    for part in trace_parts(folder,mode):
        # Match the selected-plan field, never fusion_not_selected=<reason>.
        choice=re.search(r'^\s*selected=(\w+)',part,re.MULTILINE)[1]
        if choice=='absorbed_by_fusion':continue
        t=re.search(r'runtime_threshold=(\d+)',part);t=int(t[1]) if t else 0
        threshold=max(threshold,t);selected.append(choice);actual.append('scalar' if t and n<t else choice)
        c=re.search(r'candidate='+choice+r' cost=([\d.]+)',part)
        if c:cost+=float(c[1])
    return dict(selected='+'.join(selected),selected_at_n='+'.join(actual),cost=cost,threshold=threshold)


def instrument(folder,fn):
    assembly=(folder/'auto.s').read_text()
    pattern=r'(\.L'+fn+r'_vector\d+:)'
    loops=len(re.findall(pattern,assembly))
    assembly=re.sub(pattern,r'\1\n incq .Lappcount(%rip)',assembly)
    assembly+='\n.bss\n.p2align 3\n.Lappcount: .quad 0\n.text\n.globl getcount\ngetcount:\n movq .Lappcount(%rip), %rax\n ret\n.globl resetcount\nresetcount:\n movq $0, .Lappcount(%rip)\n ret\n.section .note.GNU-stack,"",@progbits\n'
    for mode,code in [('auto_on',assembly),('auto_off',assembly.replace('.Lmemory_cpu_state: .long -1','.Lmemory_cpu_state: .long 0'))]:
        path=folder/f'{mode}.s';path.write_text(code);run(['gcc','-shared','-fPIC',path,'-o',folder/f'{mode}.so'])
    return loops


def validate_native(compiled):
    # Expected counts were first checked manually in linked N=1024 disassembly.
    expected={
        'vector_triad':[(2,1),(2,1),(2,1),(2,1)],
        'multi_output':[(6,3),(2,3),(2,3),(2,3)],
        'two_stage':[(3,2),(3,2),(3,2),(2,1)],
        'in_place_forward':[(6,4),(4,4),(2,4),(4,4)]}
    for (domain,kernel,n),(folder,m) in compiled.items():
        for mode,want in zip(['simd','reuse','forward','fusion'],expected[kernel]):
            actual=tuple(sum(r[x] for r in m['native'][mode]['regions']) for x in ['loads','stores'])
            assert actual==want,(domain,kernel,n,mode,actual,want)
            trace=(folder/f'{mode}.trace').read_text()
            traced=tuple(sum(int(v) for v in re.findall(r'vector_'+x+r'=(\d+)',trace)) for x in ['loads','stores'])
            assert actual==traced,(folder,mode,actual,traced)
        assert m['native']['scalar']['ymm_instructions']==0
        assert m['native']['gcc_scalar']['ymm_instructions']==0
    print('Linked instruction counts match expected transformations and traces for every compiled configuration.',flush=True)


def correctness(kernels,sizes,out,allow_cached=False):
    out.mkdir(parents=True,exist_ok=True);compiled={}
    for kernel in kernels:
        for n in sizes:compiled['static',kernel,n]=compile_case(kernel,'static',n)
        compiled['dynamic',kernel,0]=compile_case(kernel,'dynamic',0)
    validate_native(compiled)
    key=hashlib.sha256(json.dumps({str(k):m['binaries'] for k,(f,m) in compiled.items()},sort_keys=True).encode()+ (APP/'runner.cpp').read_bytes()+str(sizes).encode()).hexdigest()
    marker=out/'correctness.json'
    if allow_cached and marker.exists() and json.loads(marker.read_text()).get('validation_key')==key:
        print('Using completed correctness certificate for these exact binaries and full matrix.',flush=True)
        return json.loads(marker.read_text()),compiled
    rows=[]
    for kernel in kernels:
        for domain in ['static','dynamic']:
            for n in sizes:
                folder,_=compiled[domain,kernel,n if domain=='static' else 0]
                data=list(csv.DictReader(io.StringIO(run([BUILD/'runner','check',folder,KERNELS[kernel],n]))))[0]
                rows.append(dict(kernel=kernel,domain=domain,n=n,kind='full_matrix',**data,path_checks=0))
            print(f'correctness {kernel} {domain}: {len(sizes)} lengths × 3 seeds × 6 alias layouts × 8 implementations PASS',flush=True)
        folder,_=compiled['dynamic',kernel,0];loops=instrument(folder,KERNELS[kernel]);threshold=plan(folder,'auto',1024)['threshold']
        for n in sizes:
            data=list(csv.DictReader(io.StringIO(run([BUILD/'runner','paths',folder,KERNELS[kernel],n,loops,threshold]))))[0]
            rows.append(dict(kernel=kernel,domain='dynamic',n=n,kind='instrumented',**data))
    write_csv(out/'correctness.csv',rows)
    cert={'status':'PASS','validation_key':key,'utc':datetime.now(timezone.utc).isoformat(),'sizes':sizes,'kernels':kernels,
          'seeds':[19,627,2026],'layouts':['disjoint','C=A','B=A','C=A+1','C=A-1','D=C and E=C+1'],
          'executions':sum(int(r['executions']) for r in rows),'comparisons':sum(int(r['comparisons']) for r in rows),
          'path_checks':sum(int(r['path_checks']) for r in rows),'implementations':MODES,
          'contract':'all five complete allocations including guards and return value, GCC -fwrapv; no timing accepted until complete certificate'}
    marker.write_text(json.dumps(cert,indent=2));print('Application correctness:',json.dumps(cert),flush=True)
    return cert,compiled


def candidates(folder,kernel,domain,n):
    result=[]
    # SIMD mode retains both separate-loop traces even when auto absorbs the consumer.
    for part in trace_parts(folder,'simd'):
        loop=int(re.search(r'loop=(\d+)',part)[1])
        for m in re.finditer(r'candidate=(\w+) cost=([\d.]+) loads=(\d+) stores=(\d+) alu=(\d+) broadcasts=(\d+) peak_registers=(\d+) reused_loads=(\d+) forwarded_loads=(\d+)',part):
            values=m.groups()
            result.append(dict(kernel=kernel,domain=domain,compile_bound=n if domain=='static' else 'runtime_n',loop=loop,
                candidate=values[0],estimated_cost=values[1],loads=values[2],stores=values[3],arithmetic=values[4],broadcasts=values[5],peak=values[6],reused_loads=values[7],forwarded_loads=values[8],
                units='analytic_cycle_units',trip_basis='exact_static' if domain=='static' else 'assumed_1024'))
    return result


def benchmark(args,kernels,sizes,out):
    check_sizes=sorted(set(SIZES+sizes+args.dynamic_sizes))
    cert,compiled=correctness(kernels,check_sizes,out,True)
    cpu=min(os.sched_getaffinity(0)) if args.cpu is None else args.cpu;os.sched_setaffinity(0,{cpu})
    topology={}
    for name in ['core_type','core_id','physical_package_id','thread_siblings_list']:
        p=Path(f'/sys/devices/system/cpu/cpu{cpu}/topology/{name}');topology[name]=p.read_text().strip() if p.exists() else 'not exposed'
    env={'utc':datetime.now(timezone.utc).isoformat(),'cpu':run(['lscpu']),'cpu_topology':run(['lscpu','-e']),
         'uname':run(['uname','-a']),'affinity':sorted(os.sched_getaffinity(0)),'guest_topology':topology,
         'gcc':run(['gcc','--version']).splitlines()[0],'g++':run(['g++','--version']).splitlines()[0],
         'binutils':run(['objdump','--version']).splitlines()[0],'python':sys.version,
         'gcc_flags':GCC_FLAGS,'gcc_scalar_extra':['-fno-tree-vectorize','-fno-tree-slp-vectorize'],
         'samples':args.samples,'static_sizes':sizes,'dynamic_sizes':args.dynamic_sizes,'patterns':args.patterns,
         'weights':WEIGHTS.read_text(),'weights_sha256':sha(WEIGHTS),'backend_sha256':sha(ROOT/'backend/x86_64_backend'),
         'frontend_sha256':sha(ROOT/'tinyc_part3/tinyc3'),'runner_sha256':sha(APP/'runner.cpp'),
         'clock':str(time.get_clock_info('monotonic')),'loadavg_at_start':Path('/proc/loadavg').read_text().strip(),
         'methodology':'5 warmups; same repetitions and initial arrays for every implementation/sample; reset and full GCC oracle outside timing; balanced randomized order; 17 samples by default; all sample outputs verified; function calls and return accumulation included; no model fitting',
         'physical_core_limit':'Affinity pins a guest logical CPU only. WSL does not establish P/E core placement or fixed frequency.',
         'correctness':cert}
    (out/'environment.json').write_text(json.dumps(env,indent=2));(out/'weights.conf').write_text(WEIGHTS.read_text())
    raw=[];summary=[];predictions=[];costrows=[];instructionrows=[];gccrows=[];seen=set()
    for domain,ns in [('static',sizes),('dynamic',args.dynamic_sizes)]:
        for n in ns:
            for kernel in kernels:
                folder,manifest=compiled[domain,kernel,n if domain=='static' else 0]
                native=manifest['native'];fn=KERNELS[kernel]
                artifact_key=(domain,kernel,n if domain=='static' else 'shared')
                if artifact_key not in seen:
                    seen.add(artifact_key);costrows+=candidates(folder,kernel,domain,n)
                    for mode in MODES:
                        info=native[mode]
                        for region in info['regions']:
                            instructionrows.append(dict(kernel=kernel,domain=domain,compile_bound=n if domain=='static' else 'runtime_n',mode=mode,
                                **{k:v for k,v in region.items() if k not in ['opcodes','byte_increments']},byte_increments=str(region['byte_increments']),opcodes=json.dumps(region['opcodes'],sort_keys=True)))
                        if mode.startswith('gcc'):
                            gccrows.append(dict(kernel=kernel,domain=domain,compile_bound=n if domain=='static' else 'runtime_n',mode=mode,
                                ymm_instructions=info['ymm_instructions'],xmm_instructions=info['xmm_instructions'],vector_regions=len(info['regions']),
                                vectorization_diagnostics=(folder/f'{mode}.vectorization.txt').read_text().strip()))
                    if domain=='dynamic' or n in [8,17,1024,262144]:
                        dest=out/'artifacts'/domain/(str(n) if domain=='static' else 'shared')/kernel;dest.mkdir(parents=True,exist_ok=True)
                        for file in folder.iterdir():
                            if file.suffix in ['.c','.tac','.trace','.s','.objdump','.json','.txt'] and not file.name.startswith(('auto_on','auto_off')):shutil.copy2(file,dest/file.name)
                for pattern in args.patterns:
                    rows=list(csv.DictReader(io.StringIO(run([BUILD/'runner','bench',folder,fn,n,args.samples,pattern]))))
                    for r in rows:r.update(kernel=kernel,domain=domain,n=n,pattern=pattern)
                    raw+=rows
                    times={m:[float(r['ns_per_call']) for r in rows if r['mode']==m] for m in MODES}
                    med={m:statistics.median(ts) for m,ts in times.items()}
                    groups={}
                    for m in EXPLICIT:groups.setdefault(native[m]['code_sha256'],[]).append(m)
                    group_median={key:statistics.median(float(r['ns_per_call']) for r in rows if r['mode'] in members) for key,members in groups.items()}
                    best=min(group_median,key=group_median.get)
                    plans={m:plan(folder,m,n) for m in MODES}
                    exact=[m for m in EXPLICIT if native[m]['code_sha256']==native['auto']['code_sha256']]
                    decision_equiv=exact
                    selected=plans['auto']['selected_at_n'];pieces=selected.split('+')
                    if not exact and len(set(pieces))==1 and pieces[0] in EXPLICIT:
                        decision_equiv=groups[native[pieces[0]]['code_sha256']]
                    predictions.append(dict(kernel=kernel,domain=domain,n=n,pattern=pattern,**plans['auto'],
                        exact_code_equivalents='+'.join(exact),decision_equivalents='+'.join(decision_equiv),
                        measured_winner='+'.join(groups[best]),winner_median_ns=group_median[best],
                        agrees=bool(set(decision_equiv)&set(groups[best])),auto_median_ns=med['auto'],
                        within_5_percent=med['auto']<=1.05*group_median[best],regret_ratio=med['auto']/group_median[best],
                        loss_percent=(med['auto']/group_median[best]-1)*100))
                    for mode in MODES:
                        q=statistics.quantiles(times[mode],n=4,method='inclusive')
                        regs=native[mode]['regions'];equiv=[m for m in MODES if native[m]['code_sha256']==native[mode]['code_sha256']]
                        summary.append(dict(kernel=kernel,domain=domain,n=n,pattern=pattern,mode=mode,**plans[mode],
                            median_ns=med[mode],q1_ns=q[0],q3_ns=q[2],min_ns=min(times[mode]),max_ns=max(times[mode]),
                            speedup_vs_scalar=med['scalar']/med[mode],speedup_vs_simd=med['simd']/med[mode],
                            speedup_vs_reuse=med['reuse']/med[mode],speedup_vs_gcc=med['gcc']/med[mode],
                            vector_loads=sum(r['loads'] for r in regs),vector_stores=sum(r['stores'] for r in regs),
                            vector_arithmetic=sum(r['arithmetic'] for r in regs),vector_regions=len(regs),
                            count_scope='+'.join(sorted(set(r['scope'] for r in regs))) or 'no_256bit_region',
                            code_sha256=native[mode]['code_sha256'],identical_machine_code='+'.join(equiv),
                            cost_basis='exact_static' if domain=='static' else 'assumed_1024',correctness='PASS'))
                    print(f'{domain:7} {kernel:16} n={n:7} {pattern:9}: auto={selected:14} {med["auto"]:.2f} ns; fastest group={"+".join(groups[best])}',flush=True)
                for name,data in [('raw',raw),('summary',summary),('predictions',predictions),('candidates',costrows),('instructions',instructionrows),('gcc',gccrows)]:
                    if data:write_csv(out/(name+'.csv'),data)
    env['loadavg_at_end']=Path('/proc/loadavg').read_text().strip();env['completed_utc']=datetime.now(timezone.utc).isoformat()
    (out/'environment.json').write_text(json.dumps(env,indent=2))
    print(f'Completed {len(raw)} verified raw samples, {len(summary)} medians. Run report.py to regenerate all tables/figures.',flush=True)


def main():
    ap=argparse.ArgumentParser();ap.add_argument('action',choices=['check','bench','build'])
    ap.add_argument('--kernel',action='append',choices=list(KERNELS));ap.add_argument('--sizes',default=','.join(map(str,SIZES)))
    ap.add_argument('--dynamic-sizes',default=','.join(map(str,DYNAMIC_SIZES)))
    ap.add_argument('--samples',type=int,default=17);ap.add_argument('--patterns',default='aligned,unaligned')
    ap.add_argument('--output',type=Path,default=APP/'results');ap.add_argument('--cpu',type=int)
    args=ap.parse_args();kernels=args.kernel or list(KERNELS);sizes=[int(n) for n in args.sizes.split(',')]
    args.dynamic_sizes=[int(n) for n in args.dynamic_sizes.split(',')];args.patterns=args.patterns.split(',')
    if args.samples<15 or any(n<0 or n>1048576 for n in sizes+args.dynamic_sizes) or any(p not in ['aligned','unaligned'] for p in args.patterns):ap.error('samples >=15, lengths 0..1048576, patterns aligned|unaligned')
    if len(sizes)!=len(set(sizes)) or len(args.dynamic_sizes)!=len(set(args.dynamic_sizes)) or len(kernels)!=len(set(kernels)) or len(args.patterns)!=len(set(args.patterns)):
        ap.error('duplicate lengths, kernels or patterns would duplicate timing configurations')
    BUILD.mkdir(parents=True,exist_ok=True);out=args.output.resolve();out.mkdir(parents=True,exist_ok=True)
    run(['make','build'])
    run(['g++','-std=c++17','-O2','-Wall','-Wextra',APP/'runner.cpp','-ldl','-o',BUILD/'runner'])
    if args.action=='check':correctness(kernels,sizes,out)
    elif args.action=='bench':benchmark(args,kernels,sizes,out)
    else:
        for kernel in kernels:
            for n in sizes:compile_case(kernel,'static',n)
            compile_case(kernel,'dynamic',0)
        print('All selected sources accepted and assembled; artifacts:',BUILD)

if __name__=='__main__':main()
