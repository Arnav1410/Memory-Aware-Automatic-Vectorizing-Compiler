#!/usr/bin/env python3
"""Paired evaluation of original weights, fitted weights and runtime dispatch.
Training uses different copy/fill/fan-out kernels in calibrate.py.
"""
import argparse
import csv
from datetime import datetime,timezone
import hashlib
import io
import json
import math
import os
from pathlib import Path
import random
import re
import statistics
from zoneinfo import ZoneInfo
from calibrate import run,write_csv
from run import source as original_source

ROOT=Path(__file__).resolve().parents[1]
WORKLOADS=['add','repeated','producer','mixed','strided']
EXPLICIT=['scalar','simd','reuse','forward','fusion']


def source(n,dynamic=False,capacity=1048576):
    text=original_source(n)
    if dynamic:
        text=text.replace(f'i<{n};','i<n;').replace(f'int t[{max(1,n)}]',f'int t[{capacity}]')
    bound='n' if dynamic else str(n)
    return text+f'\nint strided(int *a,int *b,int *c,int *d,int n){{int i;for(i=0;i<{bound};i=i+1)c[i]=a[i*2]+b[i];return i;}}\n'


def parts(text,workload):
    return [s for s in re.split('function=',text)[1:] if s.startswith(workload+' ')]


def decision(trace,workload,n):
    picks=[];loads=stores=0;cost=0;thresholds=[]
    for s in parts(trace,workload):
        pick=re.search(r'selected=(\w+)',s)[1]
        if pick=='absorbed_by_fusion':continue
        t=re.search(r'runtime_threshold=(\d+)',s);threshold=int(t[1]) if t else 0
        thresholds.append(threshold)
        if threshold and n<threshold:pick='scalar'
        picks.append(pick)
        count=re.search(r'candidate='+pick+r' cost=([\d.]+) loads=(\d+) stores=(\d+)',s)
        if count:
            cost+=float(count[1])
            if pick!='scalar':loads+=int(count[2]);stores+=int(count[3])
    return '+'.join(picks),loads,stores,cost,max(thresholds,default=0)


def paired_interval(rows,old,new):
    a={int(r['sample']):float(r['ns_per_call']) for r in rows if r['mode']==old}
    b={int(r['sample']):float(r['ns_per_call']) for r in rows if r['mode']==new}
    ratios=[a[s]/b[s] for s in sorted(a)]
    rng=random.Random(64873)
    boot=sorted(statistics.median(rng.choices(ratios,k=len(ratios))) for _ in range(2000))
    return statistics.median(ratios),boot[49],boot[1949]


def main():
    ap=argparse.ArgumentParser()
    ap.add_argument('--weights',type=Path,default=ROOT/'benchmarks/calibration/results/calibrated.conf')
    ap.add_argument('--output',type=Path,default=ROOT/'benchmarks/results-v2')
    ap.add_argument('--sizes',default='1,7,8,17,1024,32768,1048576')
    ap.add_argument('--dynamic-sizes',default='0,1,3,7,8,9,17,64,1024,32768')
    ap.add_argument('--patterns',default='aligned,unaligned')
    ap.add_argument('--samples',type=int,default=9);ap.add_argument('--cpu',type=int,default=0)
    args=ap.parse_args()
    if args.samples<3 or not args.weights.is_file():ap.error('samples >=3 and existing calibrated weights required')
    sizes=[int(n) for n in args.sizes.split(',')];dynamic_sizes=[int(n) for n in args.dynamic_sizes.split(',')]
    if any(n<0 or n>1048576 for n in sizes+dynamic_sizes):ap.error('sizes 0..1048576')
    patterns=args.patterns.split(',')
    if any(p not in ['aligned','unaligned'] for p in patterns):ap.error('invalid pattern')
    os.sched_setaffinity(0,{args.cpu});out=args.output.resolve();out.mkdir(parents=True,exist_ok=True)
    build=ROOT/'build'/'evaluation-v2';build.mkdir(parents=True,exist_ok=True)
    run(['g++','-std=c++17','-O2','benchmarks/runner.cpp','-ldl','-o',build/'runner'])
    env={'utc':datetime.now(timezone.utc).isoformat(),'cpu':run(['lscpu']),'affinity':[args.cpu],
         'gcc':run(['gcc','--version']).splitlines()[0],'gcc_flags':'-O3 -march=native -fwrapv -shared -fPIC',
         'samples':args.samples,'sizes':sizes,'dynamic_sizes':dynamic_sizes,'patterns':patterns,
         'original_weights':(ROOT/'benchmarks/default.conf').read_text(),'calibrated_weights':args.weights.read_text(),
         'calibration_sha256':hashlib.sha256(args.weights.read_bytes()).hexdigest(),
         'compiler_sha256':hashlib.sha256((ROOT/'backend/x86_64_backend').read_bytes()).hexdigest(),
         'runner_source_sha256':hashlib.sha256((ROOT/'benchmarks/runner.cpp').read_bytes()).hexdigest(),
         'protocol':'5 warmups, ~10ms per sample, shuffled paired method rounds, full allocation validation before/after timing, function call included, initialization excluded',
         'accuracy':'fastest group by pooled median of samples with identical explicit-mode function assembly; runtime guard path tracked separately from exact code identity',
         'bootstrap':'2000 fixed-seed paired resamples of per-round speedup ratios; 2.5/97.5 percentiles; exploratory, not correction for multiple comparisons'}
    (out/'environment.json').write_text(json.dumps(env,indent=2))
    raw=[];summary=[];predictions=[];comparisons=[]
    for domain,ns in [('static',sizes),('dynamic',dynamic_sizes)]:
        for n in ns:
            folder=build/domain/(str(n) if domain=='static' else 'shared');folder.mkdir(parents=True,exist_ok=True)
            modes=EXPLICIT+['auto-original','auto-calibrated']+(['auto-threshold'] if domain=='dynamic' else [])+['gcc']
            # One unknown-bound library is shared by ALL dynamic lengths; no specialization to N.
            if domain=='static' or n==ns[0]:
                src=folder/'kernels.c';src.write_text(source(n,domain=='dynamic'));tac=folder/'kernels.tac'
                run([ROOT/'tinyc_part3/tinyc3',src,ROOT/'targets/x86_64_tinyc.conf',tac])
                for mode in modes[:-1]:
                    actual='auto' if mode.startswith('auto-') else mode
                    cmd=[ROOT/'backend/x86_64_backend','--mode',actual,tac,'--trace',folder/f'{mode}.trace',
                         '--runtime-threshold','model' if mode=='auto-threshold' else 'off']
                    if mode in ['auto-calibrated','auto-threshold']:cmd+=['--weights',args.weights]
                    else:cmd+=['--weights',ROOT/'benchmarks/default.conf']
                    asm=folder/f'{mode}.s';asm.write_text(run(cmd))
                    run(['gcc','-shared','-fPIC',asm,'-o',folder/f'{mode}.so'])
                run(['gcc','-std=c11','-O3','-march=native','-fwrapv','-shared','-fPIC',src,'-o',folder/'gcc.so'])
                for mode in modes:(folder/f'{mode}.objdump').write_text(run(['objdump','-d',folder/f'{mode}.so']))
            traces={m:(folder/f'{m}.trace').read_text() for m in modes[:-1]}
            for pattern in patterns:
                for workload in WORKLOADS:
                    rows=list(csv.DictReader(io.StringIO(run([build/'runner',folder,workload,n,args.samples,','.join(modes),pattern]))))
                    for r in rows:r.update(domain=domain,pattern=pattern)
                    raw+=rows
                    med={m:statistics.median(float(r['ns_per_call']) for r in rows if r['mode']==m) for m in modes}
                    fingerprints={m:hashlib.sha256((folder/f'{m}.s').read_text().split(f'{workload}:\n')[1].split(f'.size {workload},')[0].encode()).hexdigest() for m in modes[:-1]}
                    groups={}
                    for m in EXPLICIT:groups.setdefault(fingerprints[m],[]).append(m)
                    group_median={key:statistics.median(float(r['ns_per_call']) for r in rows if r['mode'] in members) for key,members in groups.items()}
                    best=min(group_median,key=group_median.get);winners=groups[best]
                    for mode in modes:
                        pick,vl,vs,cost,threshold=decision(traces[mode],workload,n) if mode!='gcc' else ('gcc','NA','NA','NA',0)
                        summary.append({'domain':domain,'pattern':pattern,'workload':workload,'n':n,'mode':mode,
                            'selected_at_n':pick,'threshold':threshold,'median_ns':med[mode],
                            'speedup_vs_scalar':med['scalar']/med[mode],'speedup_vs_gcc':med['gcc']/med[mode],
                            'vector_loads_per_batch':vl,'vector_stores_per_batch':vs,'estimated_cost':cost,
                            'cost_basis':'assumed_trip_1024' if domain=='dynamic' else 'exact_static_trip',
                            'cost_units':'ns' if mode in ['auto-calibrated','auto-threshold'] else 'analytic_cycle_units' if mode!='gcc' else 'NA',
                            'code_sha256':fingerprints.get(mode,'gcc')})
                        if mode.startswith('auto-'):
                            exact=[m for m in EXPLICIT if fingerprints[m]==fingerprints[mode]]
                            # Threshold introduces real extra instructions. Below it the dispatch
                            # chooses original scalar TAC, not code identical to a scalar function.
                            equivalent=exact
                            if mode=='auto-threshold' and threshold:
                                if n<threshold:equivalent=['scalar']
                                else:equivalent=[m for m in EXPLICIT if fingerprints[m]==fingerprints['auto-calibrated']]
                            predictions.append({'domain':domain,'pattern':pattern,'workload':workload,'n':n,'model':mode,
                                'selected_at_n':pick,'observed_code_group':'+'.join(winners),
                                'exact_code_equivalents':'+'.join(exact),'decision_equivalents':'+'.join(equivalent),
                                'agrees':bool(set(winners)&set(equivalent)),
                                'within_5_percent':med[mode]<=1.05*group_median[best],
                                'regret_ratio':med[mode]/group_median[best]})
                    for before,after in [('auto-original','auto-calibrated')]+([('auto-calibrated','auto-threshold')] if domain=='dynamic' else []):
                        ratio,low,high=paired_interval(rows,before,after)
                        comparisons.append({'domain':domain,'pattern':pattern,'workload':workload,'n':n,
                            'before':before,'after':after,'identical_code':fingerprints[before]==fingerprints[after],
                            'median_paired_speedup':ratio,'bootstrap_low':low,'bootstrap_high':high})
                    print(f'{domain:7s} {pattern:9s} {workload:8s} N={n:7}: original={med["auto-original"]:.3f} ns calibrated={med["auto-calibrated"]:.3f} ns'+(f' threshold={med["auto-threshold"]:.3f} ns' if domain=='dynamic' else ''),flush=True)
            for name,data in [('raw',raw),('summary',summary),('predictions',predictions),('comparisons',comparisons)]:write_csv(out/f'{name}.csv',data)
    render(out)


def render(out):
    read=lambda name:list(csv.DictReader((out/f'{name}.csv').open()))
    summary,predictions,comparisons,raw=map(read,['summary','predictions','comparisons','raw'])
    env=json.loads((out/'environment.json').read_text())
    local_time=datetime.fromisoformat(env['utc']).astimezone(ZoneInfo('Asia/Kolkata')).strftime('%Y-%m-%d %H:%M:%S IST')
    lines=['# Calibrated-model evaluation','',f"Recorded: {local_time}; CPU affinity {env['affinity']}; {env['gcc']}. Raw timestamps use UTC.",
           f'{len(raw)} raw samples; {len(summary)} medians. Raw timing and prediction datasets are linked below.','',
           '## Prediction accuracy','',
           'Accuracy compares the selected implementation with the fastest explicit code group, pooling samples of identical assembly. Runtime dispatch is a distinct implementation; its chosen branch is reported separately from exact code identity.','',
           '| Domain | Model | Agreement | Runtime within 5% of fastest group |','|---|---|---:|---:|']
    for domain in ['static','dynamic']:
        for model in ['auto-original','auto-calibrated','auto-threshold']:
            subset=[r for r in predictions if r['domain']==domain and r['model']==model]
            if not subset:continue
            lines.append(f"| {domain} | {model} | {sum(r['agrees']=='True' for r in subset)}/{len(subset)} | {sum(r['within_5_percent']=='True' for r in subset)}/{len(subset)} |")
    lines+=['','## What changed in emitted code','',
            '| Comparison | Cases | Different code | Different-code cases with CI entirely >1.05 | Different-code cases with CI entirely <0.95 |',
            '|---|---:|---:|---:|---:|']
    for before,after in [('auto-original','auto-calibrated'),('auto-calibrated','auto-threshold')]:
        cases=[r for r in comparisons if r['before']==before and r['after']==after];changed=[r for r in cases if r['identical_code']=='False']
        lines.append(f"| {before} → {after} | {len(cases)} | {len(changed)} | {sum(float(r['bootstrap_low'])>1.05 for r in changed)} | {sum(float(r['bootstrap_high'])<0.95 for r in changed)} |")
    lines+=['','Paired speedup is before/after runtime. Bootstrap intervals use nine paired rounds and are exploratory; they do not correct for multiple comparisons. Differences between identical-code models are timing/layout variability, not evidence of model improvement.','',
            '## Representative unaligned static timings','',
            'Median microseconds per call. Full results include both alignments, all sizes and scalar fallback for stride-2 accesses.','',
            '| Workload | N | Scalar | SIMD | Reuse | Forward | Fusion | Original auto | Calibrated auto | GCC |',
            '|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|']
    lookup={(r['domain'],r['pattern'],r['workload'],int(r['n']),r['mode']):r for r in summary}
    for n in [8,1024,32768,1048576]:
        for fn in WORKLOADS:
            key=('static','unaligned',fn,n)
            if key+('gcc',) not in lookup:continue
            vals=[float(lookup[key+(m,)]['median_ns'])/1000 for m in EXPLICIT+['auto-original','auto-calibrated','gcc']]
            lines.append(f'| {fn} | {n} | '+' | '.join(f'{v:.4f}' for v in vals)+' |')
    lines+=['','## Runtime-threshold measurements','',
            'First function (`add`) keeps the same function entry offset across variants, reducing code-placement confounding. The check is a cost-derived batch threshold, not a fitted lookup table.','',
            '| Alignment | N | Off (ns) | On (ns) | Paired speedup | Bootstrap interval |',
            '|---|---:|---:|---:|---:|---:|']
    for r in comparisons:
        if r['after']!='auto-threshold' or r['workload']!='add':continue
        key=('dynamic',r['pattern'],'add',int(r['n']))
        off=float(lookup[key+('auto-calibrated',)]['median_ns']);on=float(lookup[key+('auto-threshold',)]['median_ns'])
        lines.append(f"| {r['pattern']} | {r['n']} | {off:.3f} | {on:.3f} | {float(r['median_paired_speedup']):.3f} | [{float(r['bootstrap_low']):.3f}, {float(r['bootstrap_high']):.3f}] |")
    lines+=['','## Limitations','',
            '* Calibration and evaluation source kernels are separate. Fitted weights are effective nanoseconds under this pinning/frequency state, not universal instruction latencies or measured DRAM byte counts.',
            '* Single logical CPU on WSL, warm buffers, no hardware-counter attribution. Alignment, code layout, thermal drift and turbo can affect rankings.',
            '* Runtime dispatch is implemented for non-fused loops. Fusion keeps its prior policy because its scalar tail is a different transformed computation.',
            '* The static/dynamic benchmark binaries and traces are under `build/evaluation-v2`. Original historical results remain under `benchmarks/results`.',
            '* This is an exploratory experiment. Do not infer broad superiority over GCC or over the original model from isolated timing rows.','',
            '[Raw samples](raw.csv) · [Medians](summary.csv) · [Predictions](predictions.csv) · [Paired comparisons](comparisons.csv) · [Environment and exact weights](environment.json)','']
    (out/'RESULTS.md').write_text('\n'.join(lines))

if __name__=='__main__':main()
