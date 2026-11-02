#!/usr/bin/env python3
"""Independently reconcile retained samples, summaries, decisions and validation totals."""
import argparse
from collections import defaultdict
import csv
import hashlib
import json
import math
from pathlib import Path
import statistics

APP=Path(__file__).resolve().parent
ROOT=APP.parents[1]
MODES=['scalar','simd','reuse','forward','fusion','auto','gcc','gcc_scalar']


def verify(out,check_binaries=False):
    def read(name):
        with (out/(name+'.csv')).open() as f:return list(csv.DictReader(f))
    def same(a,b):
        assert math.isclose(float(a),float(b),rel_tol=1e-12,abs_tol=1e-9),(a,b)
    def key(row):return row['domain'],row['pattern'],row['kernel'],int(row['n']),row['mode']
    raw,summary,pred,checks=map(read,['raw','summary','predictions','correctness'])
    env=json.loads((out/'environment.json').read_text())
    cert=json.loads((out/'correctness.json').read_text())
    assert cert['status']=='PASS' and env['samples']>=15
    by=defaultdict(list)
    for row in raw:
        assert row['correctness']=='PASS' and math.isfinite(float(row['ns_per_call'])) and float(row['ns_per_call'])>0
        assert int(row['repetitions'])>0
        assert int(row['cpu_before']) in env['affinity'] and int(row['cpu_after']) in env['affinity']
        by[key(row)].append(row)
    lookup={key(row):row for row in summary}
    assert len(lookup)==len(summary)==len(by)
    wanted={(domain,pattern,kernel,int(n),mode)
            for domain in ['static','dynamic'] for pattern in env['patterns']
            for kernel in cert['kernels'] for n in env[domain+'_sizes'] for mode in MODES}
    assert set(by)==wanted
    for k,row in lookup.items():
        samples=by[k];times=[float(r['ns_per_call']) for r in samples]
        assert len(times)==env['samples'] and {int(r['sample']) for r in samples}==set(range(env['samples']))
        assert row['correctness']=='PASS'
        same(row['median_ns'],statistics.median(times))
        q=statistics.quantiles(times,n=4,method='inclusive')
        for field,value in [('q1_ns',q[0]),('q3_ns',q[2]),('min_ns',min(times)),('max_ns',max(times))]:same(row[field],value)
        for base,field in [('scalar','speedup_vs_scalar'),('simd','speedup_vs_simd'),('reuse','speedup_vs_reuse'),('gcc','speedup_vs_gcc')]:
            same(row[field],float(lookup[k[:-1]+(base,)]['median_ns'])/statistics.median(times))
    configs={k[:-1] for k in by}
    assert len(pred)==len(configs)
    assert {(r['domain'],r['pattern'],r['kernel'],int(r['n'])) for r in pred}==configs
    for row in pred:
        cfg=row['domain'],row['pattern'],row['kernel'],int(row['n'])
        groups=defaultdict(list)
        for mode in MODES[:5]:groups[lookup[cfg+(mode,)]['code_sha256']].append(mode)
        medians={h:statistics.median(float(r['ns_per_call']) for m in members for r in by[cfg+(m,)]) for h,members in groups.items()}
        best=min(medians,key=medians.get)
        assert row['measured_winner']=='+'.join(groups[best])
        same(row['winner_median_ns'],medians[best])
        assert (row['agrees']=='True')==bool(set(groups[best])&set(row['decision_equivalents'].split('+')))
        same(row['regret_ratio'],float(lookup[cfg+('auto',)]['median_ns'])/medians[best])
        assert (row['within_5_percent']=='True')==(float(row['auto_median_ns'])<=1.05*medians[best])
    for field in ['executions','comparisons','path_checks']:assert cert[field]==sum(int(r[field]) for r in checks)
    if check_binaries:
        from run import plan
        for path,field in [('backend/x86_64_backend','backend_sha256'),('tinyc_part3/tinyc3','frontend_sha256'),('benchmarks/default.conf','weights_sha256'),('benchmarks/applications/runner.cpp','runner_sha256')]:
            assert hashlib.sha256((ROOT/path).read_bytes()).hexdigest()==env[field],path
        checked=set()
        for k,row in lookup.items():
            domain,_,kernel,n,mode=k
            folder=ROOT/'build/applications'/domain/(str(n) if domain=='static' else 'shared')/kernel
            manifest=json.loads((folder/'manifest.json').read_text())
            actual_plan=plan(folder,mode,n)
            for field in ['selected','selected_at_n']:
                assert row[field]==actual_plan[field],(folder,mode,field)
            same(row['threshold'],actual_plan['threshold'])
            if row['cost']!='':same(row['cost'],actual_plan['cost'])
            assert row['code_sha256']==manifest['native'][mode]['code_sha256']
            binary=folder/(mode+'.so')
            if binary not in checked:
                assert hashlib.sha256(binary.read_bytes()).hexdigest()==manifest['binaries'][mode],binary
                checked.add(binary)
            source=(APP/kernel/'kernel.c').read_text()
            if domain=='static':source=source.replace('i<n;',f'i<{n};').replace('temporary[1048576]',f'temporary[{max(1,n)}]')
            assert hashlib.sha256(source.encode()).hexdigest()==manifest['source_sha256'],kernel
    print(f'PASS: {len(raw)} raw samples, {len(summary)} medians/dispersion/speedups, {len(pred)} grouped predictions and correctness totals reconciled'+('; binary/source/weight hashes match.' if check_binaries else '.'))


if __name__=='__main__':
    ap=argparse.ArgumentParser();ap.add_argument('--input',type=Path,default=APP/'results');ap.add_argument('--check-binaries',action='store_true')
    args=ap.parse_args();verify(args.input.resolve(),args.check_binaries)
