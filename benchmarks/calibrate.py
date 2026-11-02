#!/usr/bin/env python3
"""Measure primitive throughput and fit small, nonnegative, interpretable weights.
No evaluation add/reuse/fusion/mixed kernels are used in fitting.
"""
import argparse
import csv
from datetime import datetime,timezone
import hashlib
import io
import json
import os
from pathlib import Path
import re
import statistics
import subprocess

ROOT=Path(__file__).resolve().parents[1]


def run(args):
    p=subprocess.run([str(x) for x in args],cwd=ROOT,text=True,capture_output=True)
    if p.returncode:raise RuntimeError(f'{args}\n{p.stdout}\n{p.stderr}')
    return p.stdout


def write_csv(path,rows):
    with path.open('w',newline='') as f:
        w=csv.DictWriter(f,fieldnames=list(rows[0]));w.writeheader();w.writerows(rows)


def linear_nonnegative(points):
    """Exact 2-variable nonnegative least squares y = intercept + slope*x."""
    xs=[x for x,y in points];ys=[y for x,y in points]
    mx=statistics.mean(xs);my=statistics.mean(ys)
    denom=sum((x-mx)**2 for x in xs)
    slope=sum((x-mx)*(y-my) for x,y in points)/denom if denom else 0
    intercept=my-slope*mx
    choices=[(max(0,my),0),(0,max(0,sum(x*y for x,y in points)/sum(x*x for x in xs)))]
    if intercept>=0 and slope>=0:choices.append((intercept,slope))
    return min(choices,key=lambda v:sum((y-v[0]-v[1]*x)**2 for x,y in points))


def assembly():
    code=['.text']
    for op in ['control','load','store','add','multiply']:
        for count in [1,4,8,16,32]:
            if op=='control' and count!=1:continue
            name=f'{op}{count}'
            code += [f'.globl {name}',f'.type {name}, @function',f'{name}:']
            code += [f'vpxor %ymm{r}, %ymm{r}, %ymm{r}' for r in range(9)]
            code += ['vpcmpeqd %ymm8, %ymm8, %ymm8',f'.L{name}:']
            for j in range(count):
                r=j%8;offset=(j%16)*32
                if op=='load':code.append(f'vmovdqu {offset}(%rdi), %ymm{r}')
                if op=='store':code.append(f'vmovdqu %ymm{r}, {offset}(%rdi)')
                if op=='add':code.append(f'vpaddd %ymm8, %ymm{r}, %ymm{r}')
                if op=='multiply':code.append(f'vpmulld %ymm8, %ymm{r}, %ymm{r}')
            code += ['decq %rsi',f'jnz .L{name}','vzeroupper','ret',f'.size {name}, .-{name}']
    code += ['.globl streamcopy','.type streamcopy, @function','streamcopy:','xorq %rax, %rax',
             '.Lcopy:','vmovdqu (%rdi,%rax,4), %ymm0','vmovdqu %ymm0, (%rsi,%rax,4)',
             'addq $8, %rax','cmpq %rdx, %rax','jl .Lcopy','vzeroupper','ret',
             '.size streamcopy, .-streamcopy','.section .note.GNU-stack,"",@progbits']
    return '\n'.join(code)+'\n'


def calibration_source():
    bodies={'fill':'a[i]=11;', 'copy':'b[i]=a[i];',
            'fan':'b[i]=a[i];c[i]=a[i];', 'fanwide':'b[i]=a[i];c[i]=a[i];d[i]=a[i];'}
    # One runtime-bound library is measured at multiple lengths. During fitting,
    # --assume-trip supplies each known experimental length to the cost estimator.
    return '\n'.join(f'int {name}(int *a,int *b,int *c,int *d,int n){{int i;for(i=0;i<n;i=i+1){{{body}}}return i;}}'
                     for name,body in bodies.items())


def main():
    ap=argparse.ArgumentParser()
    ap.add_argument('--output',type=Path,default=ROOT/'benchmarks'/'calibration'/'results')
    ap.add_argument('--cpu',type=int,default=0);ap.add_argument('--samples',type=int,default=9)
    args=ap.parse_args();out=args.output.resolve();out.mkdir(parents=True,exist_ok=True)
    build=ROOT/'build'/'calibration';build.mkdir(parents=True,exist_ok=True)
    os.sched_setaffinity(0,{args.cpu})
    flags=run(['lscpu'])
    if 'avx2' not in flags:raise RuntimeError('AVX2 target required for direct assembly probes')
    asm=build/'probes.s';asm.write_text(assembly())
    run(['gcc','-shared','-fPIC',asm,'-o',build/'probes.so'])
    run(['g++','-std=c++17','-O2','-Wall','-Wextra','benchmarks/calibration/probe.cpp','-ldl','-o',build/'probe'])
    (build/'probes.objdump').write_text(run(['objdump','-d',build/'probes.so']))
    primitive=list(csv.DictReader(io.StringIO(run([build/'probe',build/'probes.so',args.samples]))))
    write_csv(out/'primitive-raw.csv',primitive)
    fits={}
    for op in ['load','store','add','multiply']:
        points=[(u,statistics.median(float(r['ns']) for r in primitive if r['kind']==op and int(r['unroll'])==u)) for u in [4,8,16,32]]
        intercept,slope=linear_nonnegative(points)
        fits[op]={'intercept_ns_per_group':intercept,'slope_ns_per_operation':slope,'points':points}
    branch=statistics.median(float(r['ns']) for r in primitive if r['kind']=='control')
    cache_text=Path(f'/sys/devices/system/cpu/cpu{args.cpu}/cache/index0/size').read_text().strip()
    cache=int(cache_text.rstrip('K'))*1024
    streams={n:statistics.median(float(r['ns']) for r in primitive if r['kind']=='stream' and int(r['n'])==n) for n in [1024,4096,16384,65536,262144,2097152]}
    l1_rate=statistics.median(ns/(n*8) for n,ns in streams.items() if n*8<=cache)
    # Relative least squares on cycles/byte, equal weight per footprint regime.
    memory=max(0,statistics.mean(ns/(n*8)-l1_rate for n,ns in streams.items() if n*8>cache))
    src=build/'training.c';src.write_text(calibration_source());tac=build/'training.tac'
    run([ROOT/'tinyc_part3/tinyc3',src,ROOT/'targets/x86_64_tinyc.conf',tac])
    for mode in ['scalar','simd']:
        s=build/f'{mode}.s';s.write_text(run([ROOT/'backend/x86_64_backend','--mode',mode,tac]))
        run(['gcc','-shared','-fPIC',s,'-o',build/f'{mode}.so'])
    # fill modifies an input; a dedicated generated timing driver validates all modes
    # against freshly initialized arrays and restores input outside each timed sample.
    run(['g++','-std=c++17','-O2','benchmarks/calibration/generated.cpp','-ldl','-o',build/'generated'])
    measured=list(csv.DictReader(io.StringIO(run([build/'generated',build,args.samples]))))
    write_csv(out/'generated-raw.csv',measured)
    med={(fn,n,mode):statistics.median(float(r['ns']) for r in measured if r['kernel']==fn and int(r['n'])==n and r['mode']==mode)
         for fn in ['fill','copy','fan','fanwide'] for n in [0,1,2,3,4,7,256,4096,65536] for mode in ['scalar','simd']}
    pairs={'fill':0,'copy':1,'fan':3,'fanwide':6}
    setup_points=[(pairs[fn],med[fn,n,'simd']-med[fn,n,'scalar']) for fn in pairs for n in [1,2,3,4,7]]
    setup,guard=linear_nonnegative(setup_points)
    weights={'units':'ns','setup':setup,'load':fits['load']['slope_ns_per_operation'],
             'store':fits['store']['slope_ns_per_operation'],'alu':fits['add']['slope_ns_per_operation'],
             'multiply_extra':max(0,fits['multiply']['slope_ns_per_operation']-fits['add']['slope_ns_per_operation']),
             'branch':branch,'guard':guard,'pressure':0,'memory':memory,'cache':cache,'assumed_trip':1024,'scalar_scale':1}
    def write_weights(path):
        path.write_text('# Fitted on calibration-only probes; units are nanoseconds, NOT CPU cycles.\n'+
                        '\n'.join(f'{k}={v}' for k,v in weights.items())+'\n')
    preliminary=build/'preliminary.conf';write_weights(preliminary)
    ratios=[]
    for n in [256,4096,65536]:
        trace=run_trace(tac,preliminary,n)
        for fn in ['fill','copy','fan','fanwide']:
            part=next(x for x in re.split('function=',trace) if x.startswith(fn+' '))
            cost=float(re.search(r'candidate=scalar cost=([\d.]+)',part)[1])
            # Fit scalar instruction overlap only. Streaming penalty stays unscaled.
            streams_count={'fill':1,'copy':2,'fan':3,'fanwide':4}[fn]
            footprint=n*4*streams_count
            penalty=n*4*streams_count*memory if footprint>cache else 0
            measured_body=med[fn,n,'scalar']-med[fn,0,'scalar']
            ratios.append(max(0,(measured_body-penalty)/max(1e-12,cost-penalty)))
    weights['scalar_scale']=statistics.median(ratios)
    write_weights(out/'calibrated.conf')
    env={'utc':datetime.now(timezone.utc).isoformat(),'cpu':flags,'affinity':[args.cpu],
         'gcc':run(['gcc','--version']).splitlines()[0],'units':'ns','samples':args.samples,
         'training_source_sha256':hashlib.sha256(src.read_bytes()).hexdigest(),
         'primitive_assembly_sha256':hashlib.sha256(asm.read_bytes()).hexdigest(),
         'excluded_evaluation_kernels':['add','repeated','producer','mixed','strided'],
         'fit':fits,'setup_guard_points':setup_points,'scalar_scale_ratios':ratios,
         'stream_medians_ns':streams,'l1_copy_ns_per_byte':l1_rate,'weights':weights,
         'limitations':'Effective throughput slopes, not instruction latency. Additive costs approximate overlapping execution. No hardware counters or frequency conversion. Pressure penalty disabled because it was not identified.'}
    (out/'fit.json').write_text(json.dumps(env,indent=2))
    print(json.dumps(weights,indent=2));print(f'Calibration raw data and fit: {out}')


def run_trace(tac,weights,n):
    p=subprocess.run([str(ROOT/'backend/x86_64_backend'),'--mode','scalar',str(tac),'--weights',str(weights),'--assume-trip',str(n)],cwd=ROOT,text=True,capture_output=True,check=True)
    return p.stderr

if __name__=='__main__':main()
