#!/usr/bin/env python3
"""Additive tests: retain the entire original suite and prove runtime paths execute."""
import ctypes as C
import importlib.util
from pathlib import Path
import re
import subprocess

ROOT=Path(__file__).resolve().parents[2]
spec=importlib.util.spec_from_file_location('original',ROOT/'tests/memory/run.py')
original=importlib.util.module_from_spec(spec);spec.loader.exec_module(original)
OUT=ROOT/'build'/'runtime-tests';OUT.mkdir(parents=True,exist_ok=True)
SOURCE='''int dynamic(int *a,int *b,int *c,int *d,int n){int i;for(i=3;i<n;i=i+1){c[i]=a[i]+b[i];d[i]=a[i]-b[i];}return i;}'''


def main():
    src=OUT/'kernel.c';src.write_text(SOURCE);tac=OUT/'kernel.tac'
    original.run([ROOT/'tinyc_part3/tinyc3',src,ROOT/'targets/x86_64_tinyc.conf',tac])
    reference=OUT/'gcc.so';original.run(['gcc','-O3','-march=native','-fwrapv','-shared','-fPIC',src,'-o',reference])
    high=OUT/'high.conf';high.write_text('setup=1000\n')
    configs=[('default',[]),('high',['--weights',high])]
    calibrated=ROOT/'benchmarks/calibration/results/calibrated.conf'
    if calibrated.exists():configs.append(('calibrated',['--weights',calibrated]))
    checks=0
    for label,options in configs:
        result=original.run([ROOT/'backend/x86_64_backend','--mode','auto',tac,*options])
        threshold=int(re.search(r'runtime_threshold=(\d+)',result.stderr)[1]);assert threshold>=8
        # Test-only assembly instrumentation, separate from timed/uninstrumented code.
        # Increment a local counter at vector body entry; exported accessor uses RIP addressing.
        assert re.search(r'\.Ldynamic_vector\d+:',result.stdout)
        instrumented=re.sub(r'(\.Ldynamic_vector\d+:)',r'\1\n    incq .Lpathcount(%rip)',result.stdout)
        instrumented+='\n.bss\n.p2align 3\n.Lpathcount: .quad 0\n.text\n.globl getcount\ngetcount:\n movq .Lpathcount(%rip), %rax\n ret\n.globl resetcount\nresetcount:\n movq $0, .Lpathcount(%rip)\n ret\n.section .note.GNU-stack,"",@progbits\n'
        for cpu in ['enabled','disabled']:
            assembly=instrumented if cpu=='enabled' else instrumented.replace('.Lmemory_cpu_state: .long -1','.Lmemory_cpu_state: .long 0')
            path=OUT/f'{label}-{cpu}.s';path.write_text(assembly);lib=path.with_suffix('.so')
            original.run(['gcc','-shared','-fPIC',path,'-o',lib])
            dll=C.CDLL(str(lib));gcc=C.CDLL(str(reference));dll.getcount.restype=C.c_uint64
            for n in sorted(set([-3,0,1,3,7,8,9,10,11,3+threshold-1,3+threshold,3+threshold+1,1003,1024])):
                for alias in [False,True]:
                    expected=None
                    for obj in [gcc,dll]:
                        arrays=[(C.c_int32*1100)(*[((i*7+j)%19)-9 for i in range(1100)]) for j in range(4)]
                        ptrs=[original.ptr(a,1) for a in arrays]
                        if alias:ptrs[2]=original.ptr(arrays[0],2)
                        fn=obj.dynamic;fn.argtypes=[original.PTR]*4+[C.c_int];fn.restype=C.c_int
                        if obj is dll:dll.resetcount()
                        ret=fn(*ptrs,n);actual=(ret,[list(a) for a in arrays])
                        if expected is None:expected=actual
                        else:
                            assert actual==expected,(label,cpu,n,alias)
                            count=dll.getcount()
                            want=cpu=='enabled' and not alias and n-3>=threshold
                            assert (count>0)==want,(label,cpu,n,alias,count,threshold)
                            checks+=1
        # Explicit off restores the original profitability policy; CPU/alias guards remain.
        off=original.run([ROOT/'backend/x86_64_backend','--mode','auto',tac,*options,'--runtime-threshold','off'])
        assert 'runtime_threshold=0' in off.stderr and 'call .Lmemory_cpu_check' in off.stdout
    print(f'PASS: {checks} runtime full-array/path-counter checks; threshold boundary, nonzero start, overlap, CPU-disabled fallback and off switch.')

if __name__=='__main__':main()
