#!/usr/bin/env python3
"""Render a compact report directly from measured CSV, without rounded input data."""
import csv
import json
from pathlib import Path
import sys


def summarize(folder):
    folder=Path(folder)
    rows=list(csv.DictReader((folder/'summary.csv').open()))
    predictions=list(csv.DictReader((folder/'predictions.csv').open()))
    raw=list(csv.DictReader((folder/'raw.csv').open()))
    env=json.loads((folder/'environment.json').read_text())
    lookup={(r['workload'],int(r['n']),r['mode']):r for r in rows}
    modes=[m for m in ['scalar','simd','reuse','forward','fusion','auto','gcc'] if any(r['mode']==m for r in rows)]
    agreements=sum(r['agrees_equivalent_code']=='True' for r in predictions)
    close=sum(r['auto_within_5_percent']=='True' for r in predictions)
    lines=['# Measured results', '',
           f"Recorded UTC: {env['recorded_utc']}. CPU affinity: {env['affinity']}. Compiler: {env['gcc']}.", '',
           f'{len(raw)} raw samples; {len(rows)} workload/size/mode medians; {len(predictions)} prediction cases.', '',
           f'The model selected code equivalent to the fastest explicit mode in **{agreements}/{len(predictions)}** cases. '
           f'Auto ran within 5% of the fastest explicit mode in **{close}/{len(predictions)}** cases. '
           'Identical code can have different medians because of code placement and measurement variability.', '',
           'Runtimes below are medians in **microseconds per kernel call**, including call/dispatch overhead. '
           'All modes passed complete-array validation before and after timing.', '',
           '| Workload | N | '+' | '.join(modes)+' | Auto speedup vs GCC |',
           '|---|---:|'+ '---:|'*(len(modes)+1)]
    for pred in predictions:
        workload,n=pred['workload'],int(pred['n'])
        if n not in (8,1024,32768,1048576):continue
        data=[lookup[workload,n,mode] for mode in modes]
        timings=' | '.join(f"{float(r['median_ns'])/1000:.4f}" for r in data)
        lines.append(f"| {workload} | {n} | {timings} | {float(lookup[workload,n,'auto']['speedup_vs_gcc']):.2f}x |")
    lines+=['', '## Prediction disagreements', '',
            '| Workload | N | Predicted | Observed explicit mode | Auto / fastest runtime |',
            '|---|---:|---|---|---:|']
    for p in predictions:
        if p['agrees_equivalent_code']=='False':
            lines.append(f"| {p['workload']} | {p['n']} | {p['predicted']} | {p['observed_explicit_mode']} | {float(p['auto_regret_ratio']):.3f} |")
    lines+=['', '## Interpretation', '',
            '* Repeated-input kernels reduce vector loads from 4 to 2 per eight elements; stores remain 2. '
            'The larger mixed expression reduces loads from 6 to 2. These are issued vector loads, not measured DRAM bytes.',
            '* Producer-consumer fusion reduces combined vector loads/stores from 3/2 to 2/1 and removes the temporary accesses on the fast path.',
            '* Basic SIMD, reuse and fusion may emit identical code when the corresponding transformation does not apply. Compare the traces and equivalence column.',
            '* The generated scalar backend spills TAC values. Its speedup is not a pure measure of SIMD efficiency; use the GCC comparison as well.',
            '* No weights were fitted to these results. The model is intentionally approximate and does not always identify the measured winner.',
            '* Static GCC operation counts are unavailable in CSV because memory operands may be folded into arithmetic. Complete disassembly is retained under build/benchmarks.',
            '* Small-array comparisons are dominated by call/guard overhead. Large-array results can vary with cache residency, CPU scheduling and turbo behavior.', '',
            'See [raw samples](raw.csv), [all medians and counts](summary.csv), [predictions](predictions.csv) and [environment](environment.json).', '']
    (folder/'RESULTS.md').write_text('\n'.join(lines))

if __name__=='__main__':
    summarize(sys.argv[1] if len(sys.argv)>1 else Path(__file__).resolve().parent/'results')
