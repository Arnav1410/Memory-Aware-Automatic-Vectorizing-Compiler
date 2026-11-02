#!/usr/bin/env python3
"""Forwarding experiment: exact static trip counts, stateful inputs, separate calibration."""
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
import sys
from calibrate import run,write_csv
from evaluate import paired_interval

ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'tests/memory'))
from native_counts import vector_bodies
MODES=['scalar','simd','reuse','forward','fusion','auto','gcc']
WORKLOADS=['basicForward','multipleReads','multipleStores','longChain','repeated','add','producer']


def source(n):
    original=(ROOT/'examples/memory/forwarding.c').read_text()
    original=original[:original.index('int localForward')].replace('i<n;',f'i<{n};')
    loop=f'for(i=0;i<{n};i=i+1)'
    bodies={'repeated':f'{loop}{{c[i]=a[i]+b[i];d[i]=a[i]-b[i];}}',
            'add':f'{loop}c[i]=a[i]+b[i];',
            'producer':f'int t[{max(1,n)}];{loop}t[i]=a[i]+b[i];{loop}c[i]=t[i]*2;'}
    return original+'\n'.join(f'int {name}(int *a,int *b,int *c,int *d,int n){{int i;{body}return i;}}' for name,body in bodies.items())


def main():
    ap=argparse.ArgumentParser();ap.add_argument('--sizes',default='1,7,8,17,1024,32768,1048576')
    ap.add_argument('--samples',type=int,default=9);ap.add_argument('--patterns',default='aligned,unaligned')
    ap.add_argument('--output',type=Path,default=ROOT/'benchmarks/results-forward')
    ap.add_argument('--cpu',type=int);ap.add_argument('--weights',type=Path)
    args=ap.parse_args();sizes=[int(n) for n in args.sizes.split(',')];patterns=args.patterns.split(',')
    if args.samples<3 or any(n<0 or n>1048576 for n in sizes) or any(p not in ['aligned','unaligned'] for p in patterns):ap.error('invalid sizes/samples/patterns')
    cpu=min(os.sched_getaffinity(0)) if args.cpu is None else args.cpu;os.sched_setaffinity(0,{cpu})
    out=args.output.resolve();out.mkdir(parents=True,exist_ok=True)
    build=ROOT/'build/forward-benchmarks';build.mkdir(parents=True,exist_ok=True)
    run(['g++','-O2','-std=c++17','-Wall','-Wextra',ROOT/'benchmarks/forward_runner.cpp','-ldl','-o',build/'runner'])
    env={'utc':datetime.now(timezone.utc).isoformat(),'cpu':run(['lscpu']),'affinity':[cpu],
         'gcc':run(['gcc','--version']).splitlines()[0],'gcc_flags':'-O3 -march=native -fwrapv -shared -fPIC',
         'sizes':sizes,'patterns':patterns,'samples':args.samples,
         'weights':args.weights.read_text() if args.weights else (ROOT/'benchmarks/default.conf').read_text(),
         'calibration':'No fitting to these evaluation kernels. Default original analytical profile unless --weights is supplied.',
         'compiler_sha256':hashlib.sha256((ROOT/'backend/x86_64_backend').read_bytes()).hexdigest(),
         'runner_sha256':hashlib.sha256((ROOT/'benchmarks/forward_runner.cpp').read_bytes()).hexdigest(),
         'source_sha256':{n:hashlib.sha256(source(n).encode()).hexdigest() for n in sizes},
         'protocol':'Exact static trip counts; five warmup calls; same reps and identical initial buffers for EVERY mode and sample; resets outside timing; single-call and repeated-call full-array GCC verification; fixed-seed randomized mode order; function-call overhead included. Modifying input values wrap as int32.',
         'counts':'objdump of linked libraries, assembler local vector/tail labels retained; count vmovdqu memory operands only inside vector bodies; GCC counts not inferred from vmovdqu because GCC can fold memory operands.'}
    (out/'environment.json').write_text(json.dumps(env,indent=2))
    raw=[];summary=[];predictions=[];comparisons=[];counts=[]
    for n in sizes:
        folder=build/str(n);folder.mkdir(exist_ok=True);src=folder/'kernels.c';src.write_text(source(n));tac=folder/'kernels.tac'
        run([ROOT/'tinyc_part3/tinyc3',src,ROOT/'targets/x86_64_tinyc.conf',tac])
        traces={};fingerprints={};native={}
        for mode in MODES[:-1]:
            cmd=[ROOT/'backend/x86_64_backend','--mode',mode,tac,'--trace',folder/f'{mode}.trace']
            if args.weights:cmd+=['--weights',args.weights]
            asm=folder/f'{mode}.s';asm.write_text(run(cmd))
            run(['gcc','-shared','-fPIC','-Wa,-L',asm,'-o',folder/f'{mode}.so'])
            traces[mode]=(folder/f'{mode}.trace').read_text()
            fingerprints[mode]={};native[mode]={}
            for fn in WORKLOADS:
                code=asm.read_text().split(fn+':\n')[1].split(f'.size {fn},')[0]
                fingerprints[mode][fn]=hashlib.sha256(code.encode()).hexdigest()
                bodies=vector_bodies(folder/f'{mode}.so',fn);native[mode][fn]=bodies
                for body in bodies:counts.append(dict(n=n,workload=fn,mode=mode,label=body['label'],loads=body['loads'],stores=body['stores'],alu=body['alu'],registers=str(body['registers'])))
        run(['gcc','-O3','-march=native','-fwrapv','-shared','-fPIC',src,'-o',folder/'gcc.so'])
        for mode in MODES:(folder/f'{mode}.objdump').write_text(run(['objdump','-d',folder/f'{mode}.so']))
        for pattern in patterns:
            for fn in WORKLOADS:
                rows=list(csv.DictReader(io.StringIO(run([build/'runner',folder,fn,n,args.samples,pattern]))))
                for row in rows:row['pattern']=pattern
                raw+=rows
                med={m:statistics.median(float(r['ns_per_call']) for r in rows if r['mode']==m) for m in MODES}
                groups={}
                for m in MODES[:-2]:groups.setdefault(fingerprints[m][fn],[]).append(m)
                groupmed={key:statistics.median(float(r['ns_per_call']) for r in rows if r['mode'] in members) for key,members in groups.items()}
                best=min(groupmed,key=groupmed.get)
                picks=[]
                for mode in MODES:
                    parts=[p for p in re.split('function=',traces.get(mode,'')) if p.startswith(fn+' ')]
                    chosen=[re.search(r'selected=(\w+)',p)[1] for p in parts]
                    chosen=[p for p in chosen if p!='absorbed_by_fusion']
                    if mode=='auto':picks=chosen
                    summary.append(dict(workload=fn,n=n,pattern=pattern,mode=mode,selected='+'.join(chosen) or 'gcc',
                        median_ns=med[mode],speedup_vs_scalar=med['scalar']/med[mode],speedup_vs_reuse=med['reuse']/med[mode],speedup_vs_gcc=med['gcc']/med[mode],
                        vector_loads=sum(b['loads'] for b in native[mode][fn]) if mode!='gcc' else 'NA',
                        vector_stores=sum(b['stores'] for b in native[mode][fn]) if mode!='gcc' else 'NA',
                        code_sha256=fingerprints[mode][fn] if mode!='gcc' else 'gcc'))
                predictions.append(dict(workload=fn,n=n,pattern=pattern,predicted='+'.join(picks),
                    observed_group='+'.join(groups[best]),agrees=fingerprints['auto'][fn]==best,
                    within_5_percent=med['auto']<=1.05*groupmed[best],regret=med['auto']/groupmed[best]))
                ratio,low,high=paired_interval(rows,'reuse','forward')
                comparisons.append(dict(workload=fn,n=n,pattern=pattern,identical_code=fingerprints['reuse'][fn]==fingerprints['forward'][fn],
                    median_paired_speedup=ratio,bootstrap_low=low,bootstrap_high=high))
                print(f'{pattern} {fn} N={n}: reuse={med["reuse"]:.3f} ns forward={med["forward"]:.3f} ns auto={"+".join(picks)}',flush=True)
        for name,data in [('raw',raw),('summary',summary),('predictions',predictions),('comparisons',comparisons),('instruction-counts',counts)]:write_csv(out/f'{name}.csv',data)
    render(out)


def render(out):
    read=lambda name:list(csv.DictReader((out/f'{name}.csv').open()))
    raw,summary,pred,comparisons=map(read,['raw','summary','predictions','comparisons'])
    changed=[r for r in comparisons if r['identical_code']=='False']
    active=[r for r in changed if int(r['n'])>=8]
    env=json.loads((out/'environment.json').read_text())
    lines=['# Store-to-load forwarding measurements','',f"Recorded UTC: {env['utc']}; affinity {env['affinity']}; {env['gcc']}.",
           f'{len(raw)} raw samples; {len(summary)} medians; {len(pred)} prediction cases. All timed outputs passed repeated-call, full-allocation GCC checks.',
           '',f"Auto selected the fastest equivalent explicit code group in **{sum(p['agrees']=='True' for p in pred)}/{len(pred)}** cases; auto runtime was within 5% in **{sum(p['within_5_percent']=='True' for p in pred)}/{len(pred)}**.",
           f"Reuse/forward code differs in {len(changed)}/{len(comparisons)} cases. Of those, paired exploratory 95% bootstrap intervals lie entirely above 1.05x in {sum(float(c['bootstrap_low'])>1.05 for c in changed)} and below 0.95x in {sum(float(c['bootstrap_high'])<.95 for c in changed)} cases. No multiple-comparison correction; nine rounds are not a broad performance guarantee.",
           f"Only {len(active)} changed-code cases execute a vector batch (N >= 8); {sum(float(c['bootstrap_low'])>1.05 for c in active)} have intervals entirely above 1.05x, {sum(float(c['bootstrap_high'])<.95 for c in active)} entirely below 0.95x. At N < 8 the transformed vector body is not executed.",
           '', 'Median microseconds per call, unaligned buffers. Forward/reuse speedup is reuse time / forward time; below one means slower.',
           '', '| Workload | N | Scalar | SIMD | Reuse | Forward | Fusion | Auto | GCC | Forward/reuse |',
           '|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|']
    lookup={(r['workload'],int(r['n']),r['pattern'],r['mode']):r for r in summary}
    for n in [8,1024,32768,1048576]:
        for fn in WORKLOADS:
            key=(fn,n,'unaligned')
            if key+('gcc',) not in lookup:continue
            times=' | '.join(f"{float(lookup[key+(m,)]['median_ns'])/1000:.4f}" for m in MODES)
            ratio=float(lookup[key+('forward',)]['speedup_vs_reuse'])
            lines.append(f'| {fn} | {n} | {times} | {ratio:.3f}x |')
    lines+=['','## Interpretation and limits','',
            '* A/B/C/D native loads per eight elements: SIMD 3/5/4/7; reuse 3/3/4/5; forward 2/2/2/2. Stores stay 2/3/3/4. Counts come from linked vector bodies, excluding guard/fallback/remainder code.',
            '* Repeated-read and add controls have identical reuse/forward code. Timings for identical code reflect measurement and placement variability, not a transformation benefit.',
            '* Fusion keeps its separate applicability: producer has two loads/one store when fused; forwarding preserves all stores and does not fuse loops.',
            '* Scalar is the existing spill-based TAC backend. Speedups over it combine SIMD with removal of scalar TAC overhead; compare GCC as well.',
            '* CPU is the recorded Intel Core i7-1255U under WSL, pinned to one logical CPU. Warm buffers and stateful repeated calls; initialization, resets, I/O and validation excluded. No hardware counters or DRAM-byte measurement.',
            '* Changed-code slowdowns can arise from value dependency chains, cache-resident reloads supported by hardware store forwarding, register lifetimes, layout, turbo or noise. These experiments do not isolate a microarchitectural cause.',
            '* Cost weights are not fitted to these kernels. Cost selection models issued loads and pressure, not all dependencies, throughput overlap or hardware store forwarding. Identical explicit implementations are grouped before judging predictions.',
            '* Raw data, exact weights, CPU details and hashes are retained below. Generated source, TAC, traces, assembly, linked objects and disassembly are under build/forward-benchmarks.',
            '', '[Raw samples](raw.csv) · [Medians](summary.csv) · [Predictions](predictions.csv) · [Paired comparisons](comparisons.csv) · [Native instruction counts](instruction-counts.csv) · [Environment](environment.json)','']
    (out/'RESULTS.md').write_text('\n'.join(lines))

if __name__=='__main__':main()
