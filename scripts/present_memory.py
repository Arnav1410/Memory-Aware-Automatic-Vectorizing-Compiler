#!/usr/bin/env python3
"""Short native-executable demonstration; full evaluation remains a separate target."""
from pathlib import Path
import os
import subprocess

ROOT=Path(__file__).resolve().parents[1]
OUT=ROOT/'build'/'presentation'


def run(args):
    p=subprocess.run([str(x) for x in args],cwd=ROOT,text=True,capture_output=True)
    if p.returncode:raise RuntimeError(f'{args}\n{p.stdout}\n{p.stderr}')
    return p.stdout


def main():
    OUT.mkdir(parents=True,exist_ok=True)
    os.sched_setaffinity(0,{min(os.sched_getaffinity(0))})
    src=ROOT/'examples/memory/presentation.c';tac=OUT/'presentation.tac'
    frontend=run([ROOT/'tinyc_part3/tinyc3',src,ROOT/'targets/x86_64_tinyc.conf',tac])
    (OUT/'frontend.log').write_text(frontend)
    print(f'TinyC source: {src}\nTyped TAC: {tac}',flush=True)
    weights=ROOT/'benchmarks/calibration/results/calibrated.conf'
    rows=['implementation,workload,n,median_ns,full_array_check\n']
    for mode in ['scalar','simd','reuse','forward','fusion','auto','gcc']:
        if mode=='gcc':
            obj=OUT/'gcc.o';run(['gcc','-O3','-march=native','-fwrapv','-c',src,'-o',obj])
            code=obj
        else:
            asm=OUT/f'{mode}.s';trace=OUT/f'{mode}.trace'
            cmd=[ROOT/'backend/x86_64_backend','--mode',mode,tac,'--trace',trace,'--dump-ir']
            if mode=='auto' and weights.exists():cmd+=['--weights',weights]
            asm.write_text(run(cmd));code=asm
            if mode in ['simd','reuse','forward','fusion','auto']:
                visible='\n'.join(line for line in trace.read_text().splitlines() if '    ir=' not in line)
                print(f'\n{mode}: analysis, legal candidates, costs, selection\n{visible}\nIR stages: {trace}',flush=True)
        exe=OUT/mode
        run(['g++','-std=c++17','-O2','-Wa,-L',ROOT/'examples/memory/presentation_driver.cpp',code,'-o',exe])
        (OUT/f'{mode}.objdump').write_text(run(['objdump','-d',exe]))
        rows.append(run([exe,mode]))
    import sys, csv
    sys.path.insert(0,str(ROOT/'tests/memory'))
    from native_counts import vector_bodies
    counts=[]
    for mode in ['scalar','simd','reuse','forward','fusion','auto']:
        for name in ['reuseExample','fusionExample','forwardExample','shortExample','dynamicExample']:
            for body in vector_bodies(OUT/mode,name):
                counts.append(dict(mode=mode,function=name,loop=body['label'],loads=body['loads'],stores=body['stores']))
    with (OUT/'instruction-counts.csv').open('w',newline='') as file:
        writer=csv.DictWriter(file,fieldnames=list(counts[0]));writer.writeheader();writer.writerows(counts)
    (OUT/'measurements.csv').write_text(''.join(rows))
    print('Native executable results (9-sample medians, ns/call):\n'+''.join(rows),flush=True)
    print(f'All source/TAC/trace/assembly/executable/disassembly artifacts: {OUT}')
    print('These short demonstration timings are illustrative. Use make benchmark-forward for the forwarding experiment and make evaluate-memory for model calibration evaluation.')

if __name__=='__main__':main()
