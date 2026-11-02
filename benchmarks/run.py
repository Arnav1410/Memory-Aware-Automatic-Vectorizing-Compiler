#!/usr/bin/env python3
"""Build actual TinyC/GCC kernels; record raw timed samples, predictions and medians."""
import argparse
import csv
from datetime import datetime, timezone
import hashlib
import io
import json
import os
from pathlib import Path
import re
import statistics
import subprocess
from summarize import summarize

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build'/'benchmarks'
MODES=['scalar','simd','reuse','forward','fusion','auto']
WORKLOADS=['add','repeated','producer','mixed']


def run(args):
    p=subprocess.run([str(x) for x in args],cwd=ROOT,text=True,capture_output=True)
    if p.returncode: raise RuntimeError(f'{args}\n{p.stdout}\n{p.stderr}')
    return p.stdout


def source(n):
    start='int {name}(int *a,int *b,int *c,int *d,int n)'
    loop=f'for(i=0;i<{n};i=i+1)'
    bodies={
        'add': f'int i;{loop}c[i]=a[i]+b[i];return i;',
        'repeated': f'int i;{loop}{{c[i]=a[i]+b[i];d[i]=a[i]-b[i];}}return i;',
        'producer': f'int t[{max(1,n)}];int i;{loop}t[i]=a[i]+b[i];{loop}c[i]=t[i]*2;return i;',
        # Held-out expression structure: defaults are not fitted to any workload.
        'mixed': f'int i;{loop}{{c[i]=(a[i]-b[i])*(a[i]+3);d[i]=a[i]*b[i]+b[i];}}return i;',
    }
    return '\n'.join(start.format(name=name)+'{'+body+'}' for name,body in bodies.items())


def sections(trace):
    found={name:[] for name in WORKLOADS}
    for part in re.split(r'function=',trace)[1:]:
        name=part.split()[0]
        if name in found: found[name].append(part)
    return found


def write_csv(path, rows):
    with path.open('w',newline='') as f:
        writer=csv.DictWriter(f,fieldnames=list(rows[0]));writer.writeheader();writer.writerows(rows)


def main():
    ap=argparse.ArgumentParser()
    ap.add_argument('--sizes',default='1,7,8,17,1024,32768,1048576')
    ap.add_argument('--samples',type=int,default=9)
    ap.add_argument('--output',type=Path,default=ROOT/'benchmarks'/'results')
    ap.add_argument('--weights',type=Path)
    ap.add_argument('--cpu',type=int)
    args=ap.parse_args()
    sizes=[int(n) for n in args.sizes.split(',')]
    if any(n<0 or n>1048576 for n in sizes) or args.samples<3: ap.error('sizes 0..1048576, samples >=3')
    BUILD.mkdir(parents=True,exist_ok=True);args.output.mkdir(parents=True,exist_ok=True)
    affinity='unavailable'
    if hasattr(os,'sched_getaffinity'):
        allowed=sorted(os.sched_getaffinity(0));cpu=args.cpu if args.cpu is not None else allowed[0]
        try: os.sched_setaffinity(0,{cpu});affinity=[cpu]
        except OSError as e: affinity=str(e)
    run(['g++','-std=c++17','-O2','benchmarks/runner.cpp','-ldl','-o',BUILD/'runner'])
    env={'recorded_utc':datetime.now(timezone.utc).isoformat(),'platform':run(['uname','-a']).strip(),
         'cpu':run(['lscpu']),'gcc':run(['gcc','--version']).splitlines()[0],
         'affinity':affinity,'sizes':sizes,'samples':args.samples,'gcc_flags':'-O3 -march=native -fwrapv -shared -fPIC',
         'weights':str(args.weights) if args.weights else 'default analytic priors, no calibration fit',
         'timing':'steady_clock; five warmups; ~10ms/sample; fixed randomized method order; initialization/validation excluded',
         'bounds':'literal in source, exact trip count; no benchmark-specific optimizer rules',
         'source_sha256':{str(n):hashlib.sha256(source(n).encode()).hexdigest() for n in sizes}}
    (args.output/'environment.json').write_text(json.dumps(env,indent=2))
    raw=[];summary=[];predictions=[]
    for n in sizes:
        folder=BUILD/str(n);folder.mkdir(exist_ok=True)
        src=folder/'kernels.c';src.write_text(source(n));tac=folder/'kernels.tac'
        run([ROOT/'tinyc_part3/tinyc3',src,ROOT/'targets/x86_64_tinyc.conf',tac])
        traces={};fingerprints={}
        for mode in MODES:
            cmd=[ROOT/'backend/x86_64_backend','--mode',mode,tac,'--trace',folder/f'{mode}.trace']
            if args.weights: cmd+=['--weights',args.weights]
            assembly=run(cmd);asm=folder/f'{mode}.s';asm.write_text(assembly)
            run(['gcc','-shared','-fPIC',asm,'-o',folder/f'{mode}.so'])
            traces[mode]=sections((folder/f'{mode}.trace').read_text())
            fingerprints[mode]={}
            for workload in WORKLOADS:
                body=assembly.split(f'{workload}:\n')[1].split(f'.size {workload},')[0]
                fingerprints[mode][workload]=hashlib.sha256(body.encode()).hexdigest()
        run(['gcc','-std=c11','-O3','-march=native','-fwrapv','-shared','-fPIC',src,'-o',folder/'gcc.so'])
        for mode in MODES+['gcc']:
            (folder/f'{mode}.objdump').write_text(run(['objdump','-d',folder/f'{mode}.so']))
        for workload in WORKLOADS:
            rows=list(csv.DictReader(io.StringIO(run([BUILD/'runner',folder,workload,n,args.samples]))))
            raw+=rows
            medians={mode:statistics.median(float(r['ns_per_call']) for r in rows if r['mode']==mode) for mode in MODES+['gcc']}
            predicted=[]
            for part in traces['auto'][workload]:
                winner=re.search(r'selected=(\w+)',part)
                if winner and winner[1]!='absorbed_by_fusion': predicted.append(winner[1])
            observed=min(MODES[:-1],key=medians.get)
            equivalent=[mode for mode in MODES[:-1] if fingerprints[mode][workload]==fingerprints['auto'][workload]]
            predictions.append({'workload':workload,'n':n,'predicted':'+'.join(predicted),
                'observed_explicit_mode':observed,'auto_equivalent_modes':'+'.join(equivalent),
                'agrees_equivalent_code':observed in equivalent,
                'auto_within_5_percent':medians['auto']<=1.05*medians[observed],
                'auto_regret_ratio':medians['auto']/medians[observed],
                'best_including_gcc':min(medians,key=medians.get)})
            for mode in MODES+['gcc']:
                vl=vs=alu=sl=ss=0;selected='gcc'
                if mode!='gcc':
                    parts=traces[mode][workload]
                    vl=sum(int(x) for part in parts for x in re.findall(r'vector_loads=(\d+)',part))
                    vs=sum(int(x) for part in parts for x in re.findall(r'vector_stores=(\d+)',part))
                    picks=[]
                    for part in parts:
                        pick=re.search(r'selected=(\w+)',part)
                        if not pick or pick[1]=='absorbed_by_fusion': continue
                        picks.append(pick[1])
                        counts=re.search(r'candidate='+pick[1]+r' cost=[\d.]+ loads=(\d+) stores=(\d+) alu=(\d+)',part)
                        if counts:
                            alu+=int(counts[3]);sl+=int(counts[1]);ss+=int(counts[2])
                    selected='+'.join(picks)
                else: vl=vs=alu=sl=ss='NA' # GCC may fold vector memory operands; do not invent counts.
                summary.append({'workload':workload,'n':n,'mode':mode,'selected':selected,
                    'median_ns':medians[mode],'speedup_vs_generated_scalar':medians['scalar']/medians[mode],
                    'speedup_vs_gcc':medians['gcc']/medians[mode],
                    'vector_loads_per_batch':vl,'vector_stores_per_batch':vs,
                    'lane_ir_loads':sl,'lane_ir_stores':ss,'lane_ir_alu':alu})
            print(f'{workload:9s} N={n:7d}: predicted={"+".join(predicted):12s} observed={observed:6s} auto={medians["auto"]:.1f} ns GCC={medians["gcc"]:.1f} ns',flush=True)
        write_csv(args.output/'raw.csv',raw);write_csv(args.output/'summary.csv',summary);write_csv(args.output/'predictions.csv',predictions)
    summarize(args.output)
    print(f'Results: {args.output}; equality counts equivalent emitted code, not only mode names.')

if __name__=='__main__':main()
