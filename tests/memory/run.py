#!/usr/bin/env python3
"""Compile real TinyC, assemble it, and compare entire allocations with GCC C."""
import ctypes as C
import os
from pathlib import Path
import random
import re
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / 'build' / 'memory-tests'
MODES = ['scalar', 'simd', 'reuse', 'forward', 'fusion', 'auto']
LENGTHS = [-3, 0, 1, 7, 8, 9, 15, 16, 17, 1003, 1024]
PTR = C.POINTER(C.c_int32)


def run(args, **kwargs):
    result = subprocess.run([str(x) for x in args], cwd=ROOT, text=True, capture_output=True, **kwargs)
    if result.returncode:
        raise RuntimeError(f'{args}\n{result.stdout}\n{result.stderr}')
    return result


def build(source, name, extra=()):
    c = OUT / f'{name}.c'
    tac = OUT / f'{name}.tac'
    c.write_text(source)
    run([ROOT/'tinyc_part3/tinyc3', c, ROOT/'targets/x86_64_tinyc.conf', tac])
    libs = {}
    for mode in MODES:
        asm = OUT/f'{name}-{mode}.s'
        result = run([ROOT/'backend/x86_64_backend', '--mode', mode, tac, '--trace', OUT/f'{name}-{mode}.trace', *extra])
        asm.write_text(result.stdout)
        lib = OUT/f'{name}-{mode}.so'
        run(['gcc', '-shared', '-fPIC', asm, '-o', lib])
        libs[mode] = C.CDLL(str(lib))
    lib = OUT/f'{name}-gcc.so'
    run(['gcc', '-std=c11', '-O3', '-march=native', '-fwrapv', '-shared', '-fPIC', c, '-o', lib])
    libs['gcc'] = C.CDLL(str(lib))
    return libs, tac


def ptr(array, index=0):
    return C.cast(C.byref(array, index*4), PTR)


def check_functions(libs, functions):
    trials = 0
    for fn in functions:
        for n in LENGTHS:
            for seed in [19, 627, 2026]:
                rng = random.Random(seed)
                initial = [[rng.randint(-7, 7) for _ in range(2100)] for _ in range(4)]
                # Independent, identical, forward-shifted and backward-shifted aliases.
                for layout in [[(0,1),(1,1),(2,1),(3,1)], [(0,1),(1,1),(0,1),(3,1)],
                               [(0,1),(1,1),(0,2),(3,1)], [(0,2),(1,1),(0,1),(1,2)]]:
                    expected = None
                    for mode in ['gcc'] + MODES:
                        arrays = [(C.c_int32*len(x))(*x) for x in initial]
                        func = getattr(libs[mode],fn)
                        func.argtypes = [PTR]*4 + [C.c_int]
                        func.restype = C.c_int
                        ret = func(*[ptr(arrays[a], off) for a,off in layout],n)
                        result = (ret, [list(a) for a in arrays])
                        if expected is None:
                            expected = result
                        elif result != expected:
                            for a in range(4):
                                for j,(x,y) in enumerate(zip(result[1][a],expected[1][a])):
                                    if x != y:
                                        raise AssertionError(f'{fn} {mode} n={n} seed={seed} alias={layout}: array {a}[{j}]={x}, GCC={y}')
                            raise AssertionError(f'{fn} {mode}: return {ret}, GCC={expected[0]}')
                        trials += 1
    return trials


def trace(name, mode):
    return (OUT/f'{name}-{mode}.trace').read_text()


def main():
    OUT.mkdir(parents=True, exist_ok=True)
    # All functions have one signature, including unused parameters, for uniform ABI tests.
    signature = 'int {name}(int *a, int *b, int *c, int *d, int n)'
    bodies = {
        'add': 'int i; for(i=0;i<n;i=i+1) c[i]=a[i]+b[i]; return i;',
        'reuse': 'int i; for(i=0;i<n;i=i+1) {c[i]=a[i]+b[i];d[i]=a[i]-b[i];} return i;',
        'multiply': 'int i; for(i=0;i<n;i=i+1) c[i]=(a[i]-b[i])*(a[i]+3); return i;',
        'invalidation': 'int i; for(i=0;i<n;i=i+1) {a[i]=a[i]+1;c[i]=a[i]+b[i];} return i;',
        'fusion': 'int t[1024]; int i; for(i=0;i<n;i=i+1)t[i]=a[i]+b[i]; for(i=0;i<n;i=i+1)c[i]=t[i]*2; return i;',
        'laterUse': 'int t[1024]; int i; for(i=0;i<n;i=i+1)t[i]=a[i]+b[i]; for(i=0;i<n;i=i+1)c[i]=t[i]*2; if(n>0)return t[0]; return 0;',
        'escaping': 'int t[1024]; int *p; int i; p=t; for(i=0;i<n;i=i+1)t[i]=a[i]+b[i]; for(i=0;i<n;i=i+1)c[i]=t[i]*2; if(n>0)return p[0]; return 0;',
        'domain': 'int t[1024]; int i; for(i=0;i<n;i=i+1)t[i]=a[i]+b[i]; for(i=1;i<n;i=i+1)c[i]=t[i]*2; return i;',
        'externalTemp': 'int i; for(i=0;i<n;i=i+1)d[i]=a[i]+b[i]; for(i=0;i<n;i=i+1)c[i]=d[i]*2; return i;',
        'recurrence': 'int i; for(i=1;i<n;i=i+1)a[i]=a[i-1]+b[i]; return i;',
        'stride': 'int i; for(i=0;i<n;i=i+1)c[i]=a[i*2]+b[i]; return i;',
        'branch': 'int i; for(i=0;i<n;i=i+1) {if(a[i]>0)c[i]=a[i];else c[i]=b[i];} return i;',
        'division': 'int i; for(i=0;i<n;i=i+1)c[i]=a[i]/3; return i;',
        'accumulator': 'int i; int s; s=0; for(i=0;i<n;i=i+1) {s=s+a[i];c[i]=s;} return s;',
        'start': 'int i; for(i=3;i<n;i=i+1)c[i]=a[i]+b[i]; return i;',
        'whileLoop': 'int i; i=0; while(i<n) {c[i]=a[i]*b[i];i=i+1;} return i;',
        'callLoop': 'int i; for(i=0;i<n;i=i+1)c[i]=helper(a[i]); return i;',
        'boundChange': 'int i; for(i=0;i<n;i=i+1) {c[i]=a[i];n=n-1;} return i;',
        'invariant': 'int i; for(i=0;i<n;i=i+1)c[i]=a[i]+n; return i;',
        'rebound': 'int t[1024];int i;a=t;for(i=0;i<n;i=i+1)t[i]=2;for(i=0;i<n;i=i+1)c[i]=a[i]+b[i];return i;',
    }
    source = 'int helper(int x) {return x*2;}\n' + '\n'.join(signature.format(name=name)+'{'+body+'}' for name,body in bodies.items())
    libs,tac = build(source,'kernels')
    trials = check_functions(libs,list(bodies))
    sections = re.split(r'function=',trace('kernels','reuse'))
    reuse = next(s for s in sections if s.startswith('reuse '))
    assert 'candidate=simd' in reuse and 'loads=4 stores=2' in reuse
    assert 'selected=reuse vector_loads=2 vector_stores=2' in reuse
    fusion = next(s for s in re.split(r'function=',trace('kernels','fusion')) if s.startswith('fusion '))
    assert 'selected=fusion vector_loads=2 vector_stores=1' in fusion
    for name in ['laterUse','escaping','domain','externalTemp']:
        section = next(s for s in re.split(r'function=',trace('kernels','fusion')) if s.startswith(name+' '))
        assert 'fusion_rejected=' in section, (name,section)
    for name in ['recurrence','stride','branch','division','accumulator','callLoop','boundChange']:
        section = next(s for s in re.split(r'function=',trace('kernels','auto')) if s.startswith(name+' '))
        assert 'selected=scalar rejection=' in section,(name,section)
    result = run([ROOT/'backend/x86_64_backend','--mode','auto',tac,'--alias-policy','reject'])
    assert 'unknown pointer aliasing' in result.stderr
    # Exercise the generated capability fallback without requiring a second CPU.
    disabled = (OUT/'kernels-fusion.s').read_text().replace('.Lmemory_cpu_state: .long -1', '.Lmemory_cpu_state: .long 0')
    (OUT/'cpu-disabled.s').write_text(disabled)
    run(['gcc','-shared','-fPIC',OUT/'cpu-disabled.s','-o',OUT/'cpu-disabled.so'])
    fallback = dict(libs)
    fallback['fusion'] = C.CDLL(str(OUT/'cpu-disabled.so'))
    trials += check_functions(fallback,['fusion','reuse'])
    # Constant trip counts exercise model rejection and vector remainders without assumed N.
    for n in [0,1,7,8,9,15,16,17,1003,1024]:
        source = signature.format(name='fixed')+'{int i;for(i=0;i<'+str(n)+';i=i+1)c[i]=a[i]+b[i];return i;}'
        fixed,_ = build(source,f'fixed{n}')
        trials += check_functions(fixed,['fixed'])
        if n < 8: assert 'selected=scalar ' in trace(f'fixed{n}','auto')
    # Observable induction variable after fusion, full data comparison above.
    # Force a cost decision with configurable weights and verify actual absence of AVX2.
    weights = OUT/'expensive.conf'; weights.write_text('setup=1000000000\n')
    result = run([ROOT/'backend/x86_64_backend','--mode','auto',tac,'--weights',weights])
    assert 'vmovdqu' not in result.stdout
    assert 'fusion_not_selected=unprofitable' in result.stderr
    invalid = OUT/'invalid.conf'; invalid.write_text('load=-1\n')
    result = subprocess.run([str(ROOT/'backend/x86_64_backend'),'--mode','auto',str(tac),'--weights',str(invalid)],capture_output=True)
    assert result.returncode != 0 and not result.stdout
    disassembly = run(['objdump','-d',OUT/'kernels-fusion.so']).stdout
    (OUT/'disassembly.txt').write_text(disassembly)
    for op in ['vmovdqu','vpaddd','vpsubd','vpmulld','vzeroupper']: assert op in disassembly
    # Real executable linkage, including a call to another generated TinyC function.
    mainlibs, _ = build('int plus(int x){return x+3;} int main(){int a[9];int i;for(i=0;i<9;i=i+1)a[i]=5;return plus(a[8]);}','main')
    for mode in MODES:
        exe=OUT/f'main-{mode}'
        run(['gcc',OUT/f'main-{mode}.s','-o',exe])
        assert subprocess.run([str(exe)]).returncode == 8
    for name, source in {
        'float': 'int main(){float x;x=1.5;return 0;}',
        'global': 'int g; int main(){g=1;return g;}',
        'shadow': 'int main(){int x;x=1;{int x;x=2;}return x;}',
        'seven': 'int seven(int a,int b,int c,int d,int e,int f,int g){return g;}',
    }.items():
        path=OUT/f'rejected-{name}.c'; path.write_text(source)
        rejected=OUT/f'rejected-{name}.tac'
        run([ROOT/'tinyc_part3/tinyc3',path,ROOT/'targets/x86_64_tinyc.conf',rejected])
        failure=subprocess.run([str(ROOT/'backend/x86_64_backend'),'--mode','auto',str(rejected)],capture_output=True)
        assert failure.returncode != 0 and not failure.stdout, name
    print(f'PASS: {trials} kernel executions, {trials//(len(MODES)+1)*len(MODES)} full-allocation comparisons against GCC; alias layouts, tails, fusion rejection, cost selection, executable calls and objdump.')

if __name__ == '__main__':
    main()
