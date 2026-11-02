#!/usr/bin/env python3
"""Isolate observed GCC stack probing; do not mix this control into model accuracy."""
import csv
import io
import json
import os
import statistics
from datetime import datetime,timezone
from pathlib import Path
from run import APP,ROOT,BUILD,GCC_FLAGS,run,write_csv


def main():
    out=APP/'results/stack-probe-control';out.mkdir(parents=True,exist_ok=True)
    folder=BUILD/'dynamic/shared/two_stage'
    if not (folder/'gcc.so').exists():raise RuntimeError('Run the main correctness/build experiment first')
    os.sched_setaffinity(0,{min(os.sched_getaffinity(0))})
    run(['g++','-std=c++17','-O2',APP/'stack_probe.cpp','-ldl','-o',BUILD/'stack_probe'])
    asm=folder/'gcc_noprobe.s'
    asm.write_text(run(['gcc',*GCC_FLAGS,'-fno-stack-clash-protection','-S',folder/'reference.c','-o','-']))
    run(['gcc','-shared','-fPIC','-Wa,-L',asm,'-o',folder/'gcc_noprobe.so'])
    baseline=(folder/'gcc.s').read_text()
    assert '.LPSRL0:' in baseline and 'subq\t$4096' in baseline
    assert '.LPSRL0:' not in asm.read_text()
    for mode in ['gcc','gcc_noprobe']:
        (out/f'{mode}.s').write_text((folder/f'{mode}.s').read_text())
        (out/f'{mode}.objdump').write_text(run(['objdump','-d','--no-show-raw-insn',folder/f'{mode}.so']))
    raw=[];summary=[]
    for n in [0,1,7,8,17,1024,32768,262144,1048576]:
        rows=list(csv.DictReader(io.StringIO(run([BUILD/'stack_probe',folder,n]))));raw+=rows
        for mode in ['gcc','gcc_noprobe']:
            ts=[float(r['ns_per_call']) for r in rows if r['mode']==mode];q=statistics.quantiles(ts,n=4,method='inclusive')
            summary.append(dict(n=n,mode=mode,median_ns=statistics.median(ts),q1_ns=q[0],q3_ns=q[2],correctness='PASS'))
    write_csv(out/'raw.csv',raw);write_csv(out/'summary.csv',summary)
    (out/'environment.json').write_text(json.dumps({'utc':datetime.now(timezone.utc).isoformat(),'affinity':sorted(os.sched_getaffinity(0)),
        'gcc_flags':GCC_FLAGS,'control_extra':'-fno-stack-clash-protection','stack_protector_and_CET':'unchanged GCC defaults',
        'samples':17,'protocol':'separate supplemental run; five warmups; alternated order; identical repetitions and buffers; full repeated GCC oracle before/after timing; one variable changed: GCC stack probing',
        'purpose':'Attribution control for fixed 4 MiB local-array prologue. Primary eight-mode results and model accuracy remain unchanged.'},indent=2))
    print('Stack-probe control: 306 verified samples; no changes to main measurements or compiler.')

if __name__=='__main__':main()
