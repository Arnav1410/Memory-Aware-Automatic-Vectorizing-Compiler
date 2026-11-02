#!/usr/bin/env python3
"""Regenerate tables, figures and academic prose solely from retained measured CSV."""
import argparse
import csv
from datetime import datetime
import json
import math
import os
from pathlib import Path
import random
import statistics
from zoneinfo import ZoneInfo

APP=Path(__file__).resolve().parent
MODES=['scalar','simd','reuse','forward','fusion','auto','gcc','gcc_scalar']
NAMES={'vector_triad':'Integer vector triad','multi_output':'Multi-output transform','two_stage':'Two-stage transform','in_place_forward':'In-place derived outputs'}
LABELS={'scalar':'Scalar','simd':'SIMD','reuse':'Reuse','forward':'Forward','fusion':'Fusion','auto':'Auto','gcc':'GCC O3','gcc_scalar':'GCC scalar'}
EFFECTS={'vector_triad':('scalar','simd'),'multi_output':('simd','reuse'),'two_stage':('reuse','fusion'),'in_place_forward':('reuse','forward')}


def read(folder,name):
    with (folder/(name+'.csv')).open() as f:return list(csv.DictReader(f))


def write_csv(path,rows):
    with path.open('w',newline='') as f:
        w=csv.DictWriter(f,fieldnames=list(rows[0]));w.writeheader();w.writerows(rows)


def paired(rows,before,after):
    a={int(r['sample']):float(r['ns_per_call']) for r in rows if r['mode']==before}
    b={int(r['sample']):float(r['ns_per_call']) for r in rows if r['mode']==after}
    ratios=[a[s]/b[s] for s in sorted(a)];rng=random.Random(627)
    boot=sorted(statistics.median(rng.choices(ratios,k=len(ratios))) for _ in range(2000))
    return statistics.median(ratios),boot[49],boot[1949]


def figures(out,summary,lookup,kernels):
    os.environ.setdefault('MPLCONFIGDIR',str(APP.parents[1]/'build/application-plot-cache'))
    import matplotlib
    matplotlib.use('Agg')
    import matplotlib.pyplot as plt
    import numpy as np
    plt.rcParams.update({'font.size':10,'axes.spines.top':False,'axes.spines.right':False,'pdf.fonttype':42,'ps.fonttype':42})
    dest=out/'figures';dest.mkdir(exist_ok=True)
    colors=dict(zip(MODES,['#777777','#0072B2','#009E73','#D55E00','#CC79A7','#000000','#E69F00','#56B4E9']))
    def save(fig,name):
        for ext in ['pdf','svg','png']:fig.savefig(dest/(name+'.'+ext),bbox_inches='tight',dpi=180)
        plt.close(fig)
    ncols=2;nrows=math.ceil(len(kernels)/2)
    for domain in ['static','dynamic']:
        for kind in ['runtime','speedup']:
            fig,axes=plt.subplots(nrows,ncols,figsize=(12,4.1*nrows),squeeze=False)
            handles=None
            for ax,kernel in zip(axes.flat,kernels):
                for mode in MODES:
                    rows=sorted([r for r in summary if r['domain']==domain and r['pattern']=='unaligned' and r['kernel']==kernel and r['mode']==mode and int(r['n'])>0],key=lambda r:int(r['n']))
                    xs=[int(r['n']) for r in rows]
                    ys=[float(r['median_ns'])/1000 if kind=='runtime' else float(r['speedup_vs_scalar']) for r in rows]
                    ax.plot(xs,ys,label=LABELS[mode],color=colors[mode],marker='o' if mode=='auto' else '.',markersize=4,linewidth=1.25,linestyle='--' if mode=='gcc_scalar' else '-')
                    if kind=='runtime' and mode=='auto':ax.fill_between(xs,[float(r['q1_ns'])/1000 for r in rows],[float(r['q3_ns'])/1000 for r in rows],color='black',alpha=.12)
                ax.set_xscale('log',base=2);ax.set_yscale('log');ax.grid(alpha=.22);ax.set_title(NAMES[kernel]);ax.set_xlabel('Array length (elements)')
                ax.set_ylabel('Median runtime (µs/call)' if kind=='runtime' else 'Speedup versus generated scalar (×)')
                ax.set_xticks([1,8,128,1024,32768,1048576],['1','8','128','1K','32K','1M'])
                if kind=='speedup':ax.axhline(1,color='gray',linewidth=.7)
                handles=ax.get_legend_handles_labels()
            for ax in list(axes.flat)[len(kernels):]:ax.set_visible(False)
            fig.legend(*handles,loc='lower center',ncol=4,bbox_to_anchor=(.5,-.015),frameon=False)
            fig.tight_layout(rect=[0,.06,1,1]);save(fig,kind if domain=='static' else kind+'_dynamic')
    fig,axes=plt.subplots(nrows,ncols,figsize=(12,3.9*nrows),squeeze=False)
    for ax,kernel in zip(axes.flat,kernels):
        rows=[lookup.get(('static','unaligned',kernel,1024,m)) for m in MODES]
        if not any(rows):
            ax.text(.5,.5,'N=1024 not measured',ha='center');ax.set_axis_off();continue
        xs=np.arange(len(MODES));loads=[int(r['vector_loads']) if r else 0 for r in rows];stores=[int(r['vector_stores']) if r else 0 for r in rows]
        ax.bar(xs,loads,label='Loads (including folded operands)',color='#0072B2')
        ax.bar(xs,stores,bottom=loads,label='Stores',color='#E69F00')
        ax.set_xticks(xs,[LABELS[m] for m in MODES],rotation=35,ha='right');ax.set_ylabel('Static 256-bit memory instructions');ax.set_title(NAMES[kernel]);ax.set_ylim(0,max(a+b for a,b in zip(loads,stores))+2)
        ax.yaxis.set_major_locator(plt.MaxNLocator(integer=True));ax.grid(axis='y',alpha=.2)
    for ax in list(axes.flat)[len(kernels):]:ax.set_visible(False)
    fig.legend(*axes.flat[0].get_legend_handles_labels(),loc='lower center',ncol=2,frameon=False)
    fig.tight_layout(rect=[0,.05,1,1]);save(fig,'memory_instructions')
    fig,axes=plt.subplots(nrows,ncols,figsize=(12,3.7*nrows),squeeze=False)
    for ax,kernel in zip(axes.flat,kernels):
        before,after=EFFECTS[kernel]
        for pattern,color in [('aligned','#0072B2'),('unaligned','#D55E00')]:
            ns=sorted({int(r['n']) for r in summary if r['domain']=='static' and r['kernel']==kernel and r['pattern']==pattern and int(r['n'])>0})
            vals=[float(lookup['static',pattern,kernel,n,before]['median_ns'])/float(lookup['static',pattern,kernel,n,after]['median_ns']) for n in ns]
            ax.plot(ns,vals,marker='o',markersize=3,label=pattern,color=color)
        ax.axhline(1,color='black',linestyle='--',linewidth=.8);ax.set_xscale('log',base=2);ax.grid(alpha=.2)
        ax.set_title(NAMES[kernel]);ax.set_xlabel('Array length (elements)');ax.set_ylabel(f'{LABELS[before]} time / {LABELS[after]} time (×)')
        ax.set_xticks([1,8,128,1024,32768,1048576],['1','8','128','1K','32K','1M'])
    for ax in list(axes.flat)[len(kernels):]:ax.set_visible(False)
    fig.legend(*axes.flat[0].get_legend_handles_labels(),loc='lower center',ncol=2,frameon=False)
    fig.tight_layout(rect=[0,.05,1,1]);save(fig,'optimization_effects')


def main():
    ap=argparse.ArgumentParser();ap.add_argument('--input',type=Path,default=APP/'results');args=ap.parse_args()
    out=args.input.resolve();dest=APP if out==APP/'results' else out
    from verify import verify
    verify(out)
    raw,summary,pred,candidates,instructions,gcc=map(lambda name:read(out,name),['raw','summary','predictions','candidates','instructions','gcc'])
    env=json.loads((out/'environment.json').read_text());cert=json.loads((out/'correctness.json').read_text())
    assert cert['status']=='PASS' and all(r['correctness']=='PASS' for r in raw)
    assert all(sum(r['mode']==m for r in raw)>=env['samples'] for m in MODES)
    kernels=[k for k in NAMES if any(r['kernel']==k for r in summary)]
    lookup={(r['domain'],r['pattern'],r['kernel'],int(r['n']),r['mode']):r for r in summary}
    tables=out/'tables';tables.mkdir(exist_ok=True)
    groups=[];effects=[]
    configurations=sorted({(r['domain'],r['pattern'],r['kernel'],int(r['n'])) for r in summary})
    for domain,pattern,kernel,n in configurations:
        rows=[r for r in raw if (r['domain'],r['pattern'],r['kernel'],int(r['n']))==(domain,pattern,kernel,n)]
        eq={}
        for mode in MODES[:5]:eq.setdefault(lookup[domain,pattern,kernel,n,mode]['code_sha256'],[]).append(mode)
        for code,members in eq.items():
            samples=[float(r['ns_per_call']) for r in rows if r['mode'] in members];q=statistics.quantiles(samples,n=4,method='inclusive')
            groups.append(dict(domain=domain,pattern=pattern,kernel=kernel,n=n,code_sha256=code,modes='+'.join(members),pooled_samples=len(samples),median_ns=statistics.median(samples),q1_ns=q[0],q3_ns=q[2]))
        before,after=EFFECTS[kernel];ratio,low,high=paired(rows,before,after)
        effects.append(dict(domain=domain,pattern=pattern,kernel=kernel,n=n,before=before,after=after,
            ratio_of_medians=float(lookup[domain,pattern,kernel,n,before]['median_ns'])/float(lookup[domain,pattern,kernel,n,after]['median_ns']),
            median_paired_speedup=ratio,bootstrap_low=low,bootstrap_high=high,
            identical_machine_code=lookup[domain,pattern,kernel,n,before]['code_sha256']==lookup[domain,pattern,kernel,n,after]['code_sha256']))
    write_csv(out/'groups.csv',groups);write_csv(out/'effects.csv',effects)
    figures(out,summary,lookup,kernels)
    recorded=datetime.fromisoformat(env['utc']).astimezone(ZoneInfo('Asia/Kolkata')).strftime('%Y-%m-%d %H:%M:%S IST')
    prefix='results/' if dest==APP else ''
    md=['# Application-oriented experimental evaluation','',
        f'Recorded: **{recorded}**. GCC: {env["gcc"]}. Raw timestamps are UTC. Results below are generated from retained measurements.',
        '', '## Objectives and benchmark suitability','',
        'Four simplified pointwise integer kernels evaluate the existing compiler through TinyC → typed TAC → loop/memory analysis → legal candidates → cost selection → assembly → linked native code. These are computational kernels, not complete applications. The vector triad is an integer adaptation of the familiar triad pattern, not the official STREAM benchmark.',
        '', '| Kernel | Computation and optimization exercised |','|---|---|',
        '| Integer vector triad | `C=A+3B`; contiguous AVX2 addition/multiplication, scalar tails and profitability |',
        '| Multi-output | `C=A+B; D=A-B; E=A*B`; repeated input loads and read reuse |',
        '| Two-stage | Local `T=A+B`, followed by `C=3T`; restricted fusion and temporary traffic elimination |',
        '| In-place derived outputs | `A=A+B; C=3A; D=A-B; A=D+2`; stored-A and stored-D forwarding with four ordered stores |',
        '', 'All four source programs were accepted without direct-TAC input or compiler changes. The subset requires int32 arithmetic, declarations before `for`, canonical unit-step loops, exact `base[i]` accesses and at most six register arguments. The multi-output signature uses all six (five pointers and n). Unsupported general C constructs were not assumed available.',
        'The two-stage temporary is a non-escaping local fixed array, with no later observable use. Runtime-bound code reserves 1,048,576 int32 elements (~4 MiB stack); static variants use `max(1,N)`. At N=0 there are zero iterations, not a zero-length C array. Caller allocations must be valid. The in-place benchmark reads after its first A store and its D store; it does not read after the final A store, so it does not independently test forwarding from that final write.',
        '', '## Experimental setup and methodology','',
        f'- CPU: Intel Core i7-1255U, AVX2, WSL Ubuntu; affinity restricted to guest logical CPU **{env["affinity"]}**. Guest core type: `{env["guest_topology"]["core_type"]}`. WSL affinity does not establish physical P-core/E-core placement, fixed frequency or exclusive host scheduling.',
        f'- {len(raw):,} raw samples; {len(summary):,} implementation/configuration medians; {env["samples"]} timed samples per configuration. Five warmup calls. Modes: scalar, simd, reuse, forward, fusion, auto, GCC O3 and GCC O3 with both loop and SLP vectorization disabled.',
        '- GCC uses `-O3 -march=native -fwrapv -fPIC`; the non-vectorized reference adds `-fno-tree-vectorize -fno-tree-slp-vectorize`. The identical specialized C computation is used, without `restrict` or manual SIMD. GCC optimization diagnostics and linked disassembly are retained.',
        f'- Static lengths: `{env["static_sizes"]}`. Dynamic lengths: `{env["dynamic_sizes"]}`. Each dynamic kernel uses one shared binary for all lengths; static specialization changes only the literal bound and local temporary extent, equally for TinyC and GCC.',
        '- Timed layouts are disjoint, with 64-byte-aligned or four-byte-offset bases. Aliasing is validated separately. All compiler runtime guards remain in the measured code.',
        '- Timing uses `steady_clock`; initialization, driver heap allocation, compilation, resets, oracle execution, verification and I/O are outside the interval. Repetition counts are identical across all implementations: `max(8,min(500000,8000000/max(1,N)))`. Function calls, kernel stack-frame setup/probes and return accumulation are included.',
        '- A fixed-seed random permutation is cyclically rotated to balance mode positions in each eight-round block. All five allocations are restored before every sample, including the in-place input. Every timed batch is checked against GCC executing the same number of calls from identical data. External calls plus a consumed return checksum prevent elimination; full output verification happens outside timing.',
        '- Inputs use `std::mt19937` seeds 19, 627 and 2026 for correctness and 2026 for timing, with `int(rng()%201)-100`; allocation elements 0..2 are forced to -100, 0, 100. Wrapping int32 operations match the backend contract and GCC `-fwrapv` even under overlapping or repeated updates.',
        '- Medians, quartiles, minima and maxima are retained. Effect intervals use 2,000 fixed-seed paired bootstrap resamples of sample-round speedup ratios; they are exploratory, without multiple-comparison correction. No model coefficients were fitted or changed.',
        '', '## Correctness and regressions','',
        f'Application verification completed **before accepted timing**: **{cert["executions"]:,} kernel executions**, **{cert["comparisons"]:,} complete-allocation/return comparisons**, including **{cert["path_checks"]} instrumented path checks**. The matrix uses all 16 lengths, three seeds, six alias layouts and eight implementations for both static and dynamic source families.',
        'The alias layouts are disjoint, C=A, B=A, C=A+1, C=A−1, and D=C with E=C+1. Separate instrumented auto binaries count vector batches with capability enabled and forced disabled. They verify the threshold, alias fallback, vector execution and tails; timed binaries contain no counters. CPU-disabled tests do not constitute testing on physical non-AVX2 hardware.',
        'The original suite remains separate and unchanged: `make test-memory` passed 41,400 executions and 34,560 comparisons; `make test-runtime` passed 152 checks. `make build`, the original frontend/backend/vectorization tests, `test-p3-suite` and `test-btp-scalar` also passed. Logs are under `build/application-audit/`. No compiler correctness fix was needed.',
        '', '## Linked machine-code evidence','',
        'The table uses N=1024. Counts are static 256-bit instructions, summing one iteration of each main vector loop. The inspected loops each advance eight int32 elements (GCC increments addresses by 32 bytes). Two separate passes therefore contribute two loop bodies to the sum. Folded memory operands in GCC arithmetic count as loads. Scalar fallback, guards, broadcasts and SSE epilogues are excluded from these loop counts. These counts do not measure dynamic instructions, cache misses or DRAM bytes.',
        '', '| Kernel | Mode | Loads | Stores | Vector arithmetic | Selected plan |','|---|---|---:|---:|---:|---|']
    for kernel in kernels:
        for mode in MODES:
            r=lookup.get(('static','unaligned',kernel,1024,mode))
            if r:md.append(f'| {NAMES[kernel]} | {mode} | {r["vector_loads"]} | {r["vector_stores"]} | {r["vector_arithmetic"]} | {r["selected"]} |')
    md+=['', 'GCC 13.3 vectorizes the four representative N=1024 kernels with 32-byte vectors. Its two-stage code retains two separate vector loops and temporary memory accesses; fusion is not observed there. GCC uses shift/add for multiplication by three. The multi-output GCC loop contains three explicit vector loads plus three folded memory operands (six reads), not three total reads. The non-vectorized GCC variants have no YMM instructions. Small fixed loops may instead be unrolled straight-line code or use SSE; `count_scope`, XMM/YMM presence, region boundaries and GCC diagnostics are recorded rather than treating those as eight-element AVX2 loops.',
        '', f'![Vector memory instructions]({prefix}figures/memory_instructions.png)',
        '', '## Runtime results','',
        'The following tables show preselected small, medium and large lengths, using static bounds and unaligned arrays. Runtime and IQR are in microseconds per call. Speedup is generated-scalar median / implementation median. Full data include all lengths, both alignments and both bound policies. An asterisk marks GCC whole-function straight-line 256-bit counts, which are not per-loop iteration counts. Zero vector counts do not imply no computation.',
        'Read reuse is compared with basic SIMD, forwarding with reuse, and fusion with the unfused reuse mode. Speedups against the TAC scalar backend include removing its stack spills and address temporaries, so they are not a pure SIMD-width effect. GCC provides an additional independently optimized baseline.']
    for kernel in kernels:
        md+=['',f'### {NAMES[kernel]}','', '| N | Implementation | Median µs | IQR µs | Speedup vs scalar | Vector loads/stores | Correctness |','|---:|---|---:|---:|---:|---|---|']
        table=[]
        for n in [8,1024,262144]:
            for mode in MODES:
                r=lookup.get(('static','unaligned',kernel,n,mode))
                if not r:continue
                star='*' if 'whole_function' in r['count_scope'] else ''
                md.append(f'| {n} | {mode} | {float(r["median_ns"])/1000:.4f} | [{float(r["q1_ns"])/1000:.4f}, {float(r["q3_ns"])/1000:.4f}] | {float(r["speedup_vs_scalar"]):.3f}x | {r["vector_loads"]}/{r["vector_stores"]}{star} | PASS |')
                table.append(r)
        if table:write_csv(tables/(kernel+'.csv'),table)
    md+=['',f'![Runtime versus array length]({prefix}figures/runtime.png)','',
        'Static, unaligned medians; log axes. The shaded auto band is its interquartile range. N=0 is retained in CSV/tables but omitted from the logarithmic size plots.',
        '',f'![Speedup versus array length]({prefix}figures/speedup.png)','',
        f'![Runtime-bound runtimes]({prefix}figures/runtime_dynamic.png)','',
        'Runtime-bound, unaligned medians, including GCC stack-frame/probe costs. The two-stage GCC curve for small N includes the fixed 4 MiB probe loop; this is analyzed separately below.',
        '', f'![Runtime-bound speedups]({prefix}figures/speedup_dynamic.png)','',
        f'![Optimization-specific speedups]({prefix}figures/optimization_effects.png)','',
        'Effect plots use the appropriate baseline for each transformation. Ratios below one are slowdowns. Named modes may be identical code; any timing difference between identical implementations is not attributed to the named optimization.',
        '', '## Cost-model assessment','',
        'The original analytical weights remain unchanged. Candidate costs are in analytical cycle-like units, not measured nanoseconds. Static cases use exact bounds; dynamic candidates retain the documented assumed trip count 1024. Non-fused auto plans additionally dispatch using the actual trip count; fused plans retain their existing policy without a runtime threshold.',
        'Exact linked function-byte hashes group equivalent explicit implementations before choosing the fastest pooled-median group. Auto samples are not pooled into those groups. Dynamic dispatch is distinguished from byte identity: the selected branch is matched to the corresponding explicit implementation, while its actual timing still includes dispatch overhead. Correctness/alias checks are identical to those tested above.',
        '', '| Domain | Prediction agreement | Auto within 5% of fastest group | Median runtime ratio auto/best | Maximum ratio |','|---|---:|---:|---:|---:|']
    for domain in ['static','dynamic']:
        rows=[p for p in pred if p['domain']==domain]
        if rows:md.append(f'| {domain} | {sum(p["agrees"]=="True" for p in rows)}/{len(rows)} | {sum(p["within_5_percent"]=="True" for p in rows)}/{len(rows)} | {statistics.median(float(p["regret_ratio"]) for p in rows):.3f} | {max(float(p["regret_ratio"]) for p in rows):.3f} |')
    md+=['', 'Legal-candidate estimates for representative static inputs (separate-loop costs summed; a fused cost shown only when that candidate is legal):','',
        '| Kernel | N | Scalar | SIMD | Reuse | Forward | Fused | Auto plan |','|---|---:|---:|---:|---:|---:|---:|---|']
    for kernel in kernels:
        for n in [1,8,1024,262144]:
            rows=[c for c in candidates if c['kernel']==kernel and c['domain']=='static' and int(c['compile_bound'])==n]
            if not rows:continue
            vals=[]
            for mode in ['scalar','simd','reuse','forward','fusion']:
                costs=[float(c['estimated_cost']) for c in rows if c['candidate']==mode];vals.append(f'{sum(costs):.1f}' if costs else '—')
            auto=lookup.get(('static','unaligned',kernel,n,'auto'))
            md.append(f'| {NAMES[kernel]} | {n} | '+' | '.join(vals)+f' | {auto["selected"] if auto else "not timed"} |')
    md+=['', 'Runtime-bound auto plans (compile-time estimates assume 1024 iterations):','',
        '| Kernel | Compile-time plan | Runtime threshold | Plan at N=7 | Plan at N=1024 |',
        '|---|---|---:|---|---|']
    for kernel in kernels:
        row1024=lookup.get(('dynamic','unaligned',kernel,1024,'auto'))
        row7=lookup.get(('dynamic','unaligned',kernel,7,'auto'))
        if row1024:
            md.append(f'| {NAMES[kernel]} | {row1024["selected"]} | {row1024["threshold"]} | {row7["selected_at_n"] if row7 else "not timed"} | {row1024["selected_at_n"]} |')
    md+=['', 'A threshold of zero denotes the existing disabled/fused policy, not a proof that every trip count is profitable. All original CPU, bounds and pointer-disjointness guards still apply. The selected-at-N field denotes the plan after profitability dispatch, not proof that a vector batch executed. Zero trips execute no vector body, and fused lengths below eight use the transformed scalar tail when entry guards pass. Those small fusion effects are not attributed to AVX2 execution.']
    tiny_fusion=[p for p in pred if p['domain']=='dynamic' and p['kernel']=='two_stage' and p['n']=='8']
    if tiny_fusion:
        ratios=', '.join(f"{p['pattern']}: {float(p['regret_ratio']):.3f}x auto/best" for p in tiny_fusion)
        md+=['', 'For the two-stage dynamic kernel at N=8, '+ratios+'. Fusion is chosen using the assumed trip count 1024 and has no runtime profitability threshold. One identifiable overhead is that the combined path checks all three A/B/C pointer pairs, whereas the separate producer checks A/B and the consumer has only one incoming pointer. The measured result includes that extra guard work and other layout/timing effects; no single causal attribution is claimed.']
    md+=['', 'All prediction failures are retained below, including dynamic and aligned cases. The loss column uses actual auto median / fastest pooled-group median; negative values can arise from layout and sampling variability even when code is equivalent.','',
        '| Domain / layout | Kernel | N | Auto plan at N | Observed group | Auto / best |','|---|---|---:|---|---|---:|']
    for p in pred:
        if p['agrees']=='False':md.append(f'| {p["domain"]} / {p["pattern"]} | {p["kernel"]} | {p["n"]} | {p["selected_at_n"]} | {p["measured_winner"]} | {float(p["regret_ratio"]):.3f} |')
    md+=['', '## Observed effects and slowdowns','', '| Kernel | Comparison | N | Unaligned static speedup | Paired exploratory 95% interval |','|---|---|---:|---:|---|']
    for kernel in kernels:
        for n in [8,1024,32768,262144,1048576]:
            matches=[e for e in effects if (e['domain'],e['pattern'],e['kernel'],e['n'])==('static','unaligned',kernel,n)]
            for e in matches:md.append(f'| {NAMES[kernel]} | {e["before"]} / {e["after"]} | {n} | {e["ratio_of_medians"]:.3f}x | [{e["bootstrap_low"]:.3f}, {e["bootstrap_high"]:.3f}] |')
    forward=[e for e in effects if e['kernel']=='in_place_forward' and e['n']>=8192]
    slow=[e for e in forward if e['ratio_of_medians']<1]
    md+=['', (f'For forwarding at N ≥ 8192, **{len(slow)}/{len(forward)}** measured domain/alignment cases have slower medians than read reuse. Across those cases, reuse/forward ratios range from {min(e["ratio_of_medians"] for e in forward):.3f} to {max(e["ratio_of_medians"] for e in forward):.3f}. The full effect table retains both gains and losses.' if forward else 'No large forwarding configuration is present in this selected dataset.'),
        'Possible influences include cache-resident reloads and hardware store forwarding, extended value lifetimes/dependency chains, code placement, memory bottlenecks and frequency/scheduling variation. Hardware counters were not collected, so these are plausible mechanisms, not established causes. The cost model does not represent all these effects. No weights were changed after inspecting the results.',
        '', '## Limitations and threats to validity','',
        '- Four small computational patterns do not establish performance on complete applications or general C. Local fixed arrays, canonical accesses, conservative disjointness guards and a spill-free 16-YMM allocator remain restrictions.',
        '- Native code is warmed and every timed sample begins after buffer reset. These are warm/repeated-call experiments, not cold-memory bandwidth measurements. The actual used footprint varies: triad 12N bytes, multi-output 20N, in-place 16N, and two-stage 16N before fusion versus 12N of accessed arrays after fusion. Local stack reservation itself is not removed.',
        '- Small kernels include call/guard overhead. Constant-bound GCC may unroll or simplify more aggressively. Runtime-bound and literal-bound results are separate, and GCC receives the same information for each experiment.',
        '- Guest affinity was controlled and CPU IDs are stored per sample. WSL reports virtual topology; physical P/E placement, turbo, temperatures, host activity and memory frequency were not controlled. Aligned/unaligned tests and repeated samples expose some variability, not all.',
        '- Instruction savings are issued-memory-operation savings. Cache misses, memory-controller traffic and energy were not measured. GCC folded operands and separate vector/SSE/scalar regions require care when interpreting counts.',
        '- Decision agreement depends on finite noisy timing samples. Exact equivalent-code groups avoid crediting names with distinct implementations, but byte differences can also reflect dispatch/layout. Bootstrap intervals are exploratory and uncorrected for multiple comparisons.',
        '- All tested outputs passed; this is evidence for the enumerated matrix, not a proof of full compiler correctness. The original regression suite is reported separately.',
        '', '## Reproduce and inspect','',
        'Run from the repository root, serially, without competing workloads. No executable script bits are required. Matplotlib is needed only to regenerate figures.','',
        '```bash','make build','make test-applications','make benchmark-applications','make benchmark-application-stack-control  # supplemental GCC prologue control','make report-applications','```','',
        'Equivalent direct commands:','',
        '```bash','python3 benchmarks/applications/run.py check','python3 benchmarks/applications/run.py bench','python3 benchmarks/applications/report.py','```','',
        'Per-kernel commands use `--kernel vector_triad`, `multi_output`, `two_stage` or `in_place_forward`. The benchmark command verifies a complete correctness certificate for the exact binaries before accepting timing. Use `--output build/my-app-results` to preserve the recorded dataset. See each kernel directory for a directly compilable source and GCC reference.',
        'All compiled configurations remain under `build/applications/{static/<N>,dynamic/shared}/<kernel>/`. Representative N=8,17,1024,262144 and shared dynamic source/TAC/traces/assembly/disassembly/GCC dumps are copied into the retained results artifacts. The runner is a native executable that dynamically loads these linked libraries; no computation is implemented in hand-written SIMD.',
        '',f'[Raw samples]({prefix}raw.csv) · [All medians/dispersion]({prefix}summary.csv) · [Candidates]({prefix}candidates.csv) · [Predictions]({prefix}predictions.csv) · [Code groups]({prefix}groups.csv) · [Effects]({prefix}effects.csv) · [Native counts]({prefix}instructions.csv) · [GCC diagnostics]({prefix}gcc.csv) · [Correctness certificate]({prefix}correctness.json) · [Environment]({prefix}environment.json)','']
    control=out/'stack-probe-control/summary.csv'
    if control.exists():
        controlrows=read(out/'stack-probe-control','summary')
        section=['## GCC stack-probe attribution control','',
            'The Ubuntu GCC driver also enables stack-clash probing, stack protection and CET (visible in retained assembly options). For the dynamic two-stage kernel, its 4 MiB temporary causes a 1024-page probe loop before testing n, even at n=0. The TinyC backend does not emit these protections. Consequently, very large apparent auto/GCC gains for tiny dynamic calls are largely a prologue-policy comparison, not evidence of fusion benefit.',
            'A separate 17-sample control recompiles only the GCC reference with `-fno-stack-clash-protection`, retaining its other flags and the same source. It verifies complete repeated-call outputs, changes no TinyC code or model weights, and is excluded from primary decision accuracy. This experimental flag isolates overhead in a bounded test; it is not a recommendation to disable stack protection in applications.',
            '', '| Runtime N | Default GCC median ns | GCC without page probes median ns |', '|---:|---:|---:|']
        for n in sorted({int(r['n']) for r in controlrows}):
            c={r['mode']:float(r['median_ns']) for r in controlrows if int(r['n'])==n}
            section.append(f'| {n} | {c["gcc"]:.3f} | {c["gcc_noprobe"]:.3f} |')
        section+=['', f'[Supplemental raw samples]({prefix}stack-probe-control/raw.csv) and assembly/disassembly retain the controlled comparison. The main tables preserve the originally requested GCC flags. Static-bound two-stage arrays scale with N, avoiding a 4 MiB allocation for tiny calls; large static cases can still include probe costs.','']
        pos=md.index('## Limitations and threats to validity');md[pos:pos]=section
    (dest/'RESULTS.md').write_text('\n'.join(md))
    academic(dest,out,prefix,env,cert,lookup,kernels,pred,effects)
    print(f'Regenerated {dest/"RESULTS.md"}, {dest/"REPORT_SECTION.tex"}, tables and PDF/SVG/PNG figures from {len(raw)} measured samples.')


def academic(dest,out,prefix,env,cert,lookup,kernels,pred,effects):
    def esc(value):
        return str(value).replace('\\',r'\textbackslash{}').replace('_',r'\_').replace('%',r'\%').replace('&',r'\&').replace('#',r'\#')
    def row(values):return ' & '.join(values)+r' \\'
    tex=[r'% Generated from retained measurements; include without overwriting the BTP report.',
         r'% Requires: \usepackage{booktabs,graphicx,listings}',
         r'% Compile from repository root, or redefine \AppEvalRoot before inclusion.',
         r'\providecommand{\AppEvalRoot}{benchmarks/applications}',
         r'\section{Application-Oriented Experimental Evaluation}',r'\label{sec:application-evaluation}',
         r'\subsection{Experimental setup}',
         'The evaluation examines four pointwise integer computational kernels using the complete TinyC-to-native-code pipeline. The workloads represent weighted channel combination, multi-output transformation, a two-stage processing chain, and an in-place state update. They are simplified kernels rather than complete applications. The integer triad is not the official STREAM benchmark.',
         'The compiler implementation and analytical cost weights were held fixed throughout evaluation. All kernels use canonical counted loops and contiguous 32-bit integer arrays. Signed addition, subtraction and multiplication follow the documented wrapping contract; GCC references were compiled with \\texttt{-fwrapv}. No \\texttt{restrict} assumptions or manually implemented SIMD were introduced.',
         f'Measurements were collected on an Intel Core i7-1255U under WSL Ubuntu, using {esc(env["gcc"])}. The process was pinned to guest logical CPU {env["affinity"][0]}. Physical Performance-core versus Efficient-core placement, frequency and host scheduling could not be established from WSL topology.',
         f'Each configuration contains {env["samples"]} timed samples after five warmup calls. The experiment compares six compiler modes, GCC \\texttt{{-O3 -march=native}}, and GCC with loop and SLP vectorization disabled. Both 64-byte-aligned and four-byte-offset bases were measured. Compilation, driver heap allocation, initialization, buffer resets, reference execution and verification were excluded from timing. Function-call, dispatch and kernel stack-frame costs were included.',
         'Identical repetition counts and initial buffers were used for every implementation. A shuffled order with cyclic rotations balanced method positions. Complete outputs were verified after each timed batch. In particular, mutable inputs were restored before every sample, preventing an in-place kernel from receiving a different workload from its reference.',
         'Static-bound source variants expose the exact trip count to both compilers. Runtime-bound variants share one binary across lengths. Unknown-bound candidate costs retain the existing assumed trip count of 1024; eligible non-fused auto plans additionally apply their runtime profitability threshold.',
         r'\subsection{Benchmark descriptions}',
         "The following listings are the actual TinyC inputs. Unused pointer arguments provide a common six-argument calling convention. The multi-output kernel uses the full six-register argument limit. The two-stage temporary is local, fixed-size, non-escaping and unobserved after its consumer loop. Static variants specialize the bound and temporary extent equally for TinyC and GCC."]
    # Avoid extra textcomp dependency in the prose.
    tex[-1]=tex[-1].replace('backend\\textquotesingle s',"backend's")
    for kernel in kernels:
        tex.append(r'\lstinputlisting[language=C,basicstyle=\ttfamily\small,caption={'+NAMES[kernel]+r'},label={lst:app-'+kernel.replace('_','-')+r'}]{\AppEvalRoot/'+kernel+r'/kernel.c}')
    tex += [r'\subsection{Correctness and generated instructions}',
        f'Before accepting performance data, {cert["executions"]:,} kernel executions completed with {cert["comparisons"]:,} full-allocation and return-value comparisons, including {cert["path_checks"]} instrumented path checks. Validation covered all sixteen lengths from zero to 1,048,576, three deterministic seeds, six alias layouts, static and dynamic bounds, and eight implementations. CPU-disabled and overlapping-pointer paths retained scalar behavior. The pre-existing regression suite separately passed 41,400 executions and 34,560 comparisons, together with 152 runtime checks.',
        'At a representative length of 1024, machine-code inspection establishes the transformations in Table~\\ref{tab:app-counts}. Counts include folded memory operands and exclude guards, scalar fallback and scalar/SSE remainder regions. They are static instructions per eight-element batch, summing the two bodies for an unfused pair. They are not measurements of external memory traffic.',
        r'\begin{table}[htbp]\centering\small',r'\caption{Observed 256-bit load/store counts from linked code at $N=1024$.}',r'\label{tab:app-counts}',r'\begin{tabular}{lrrrrr}\toprule',
        row(['Kernel','Basic SIMD','Reuse','Forward','Fusion','GCC O3']),r'\midrule']
    for kernel in kernels:
        vals=[]
        for mode in ['simd','reuse','forward','fusion','gcc']:
            r=lookup.get(('static','unaligned',kernel,1024,mode));vals.append(r['vector_loads']+'/'+r['vector_stores'] if r else '--')
        tex.append(row([NAMES[kernel]]+vals))
    tex += [r'\bottomrule\end{tabular}\end{table}',
        'Read reuse reduces the multi-output kernel from six to two vector loads. Store forwarding reduces the in-place kernel from four loads after read reuse to two, retaining all four stores. The first stored A value and the stored D value are forwarded; the final A store has no subsequent read in this workload. Fusion removes the internal temporary accesses and reduces the two-stage pair from three loads and two stores to two loads and one store. GCC vectorizes the representative kernels; its two-stage code retains two vector loops. GCC also uses shifts and addition for multiplication by three. These observations are specific to the recorded compiler version, source and flags.',
        r'\subsection{Runtime results}',
        'Tables below provide medians for predetermined medium and large unaligned static-bound configurations. Complete raw samples, interquartile ranges, minima, maxima, all sizes and both alignments are retained in the accompanying CSV files. Ratios use baseline runtime divided by optimized runtime. Comparisons against the generated scalar backend include removal of its TAC-related stack traffic and therefore do not isolate SIMD lane-width gains.']
    for kernel in kernels:
        tex += [r'\begin{table}[htbp]\centering\small',r'\caption{'+NAMES[kernel]+r': measured median runtime in microseconds per call.}',r'\begin{tabular}{lrr}\toprule',row(['Implementation',r'$N=1024$',r'$N=262144$']),r'\midrule']
        for mode in MODES:
            vals=[]
            for n in [1024,262144]:
                r=lookup.get(('static','unaligned',kernel,n,mode));vals.append(f'{float(r["median_ns"])/1000:.4f}' if r else '--')
            tex.append(row([LABELS[mode]]+vals))
        tex += [r'\bottomrule\end{tabular}\end{table}']
    for name,caption in [
            ('runtime','Median runtime versus array length for static, unaligned inputs. Axes are logarithmic; the auto band denotes its interquartile range. Zero-length cases remain in the data tables.'),
            ('speedup','Speedup relative to generated scalar code. Function calls and guards are included; scalar speedups include reduced TAC overhead.'),
            ('memory_instructions','Static vector memory instructions at length 1024, including folded GCC operands. The unfused two-stage count sums two loop bodies.'),
            ('optimization_effects','Transformation-specific speedups using appropriate baselines. Values below one indicate slower execution. Both alignments are included.')]:
        tex += [r'\begin{figure}[htbp]\centering',r'\includegraphics[width=\linewidth]{\AppEvalRoot/results/figures/'+name+r'.pdf}',r'\caption{'+caption+r'}\label{fig:app-'+name.replace('_','-')+r'}',r'\end{figure}']
    tex += [r'\subsection{Cost-model assessment and cross-benchmark discussion}',
        'Exact linked function bytes were used to group equivalent explicit implementations before identifying the measured fastest group. The selected runtime branch and exact code identity were recorded separately, since dispatch instructions may distinguish auto from its underlying explicit candidate. Timing differences between identical code were not attributed to the named optimization.',
        r'\begin{table}[htbp]\centering\small',r'\caption{Cost-model decision agreement and measured performance proximity.}',r'\begin{tabular}{lrrr}\toprule',row(['Bound policy','Agreement',r'Within 5\% of best','Median auto/best']),r'\midrule']
    for domain in ['static','dynamic']:
        rows=[p for p in pred if p['domain']==domain]
        if rows:tex.append(row([domain,f'{sum(p["agrees"]=="True" for p in rows)}/{len(rows)}',f'{sum(p["within_5_percent"]=="True" for p in rows)}/{len(rows)}',f'{statistics.median(float(p["regret_ratio"]) for p in rows):.3f}']))
    tex += [r'\bottomrule\end{tabular}\end{table}',
        'The unchanged model estimates issued loads, stores, arithmetic, setup, register pressure and a working-set proxy. It does not represent every throughput interaction or dependency chain. Prediction failures, measured winners and relative losses are retained for every configuration; no coefficient was adjusted using these evaluation workloads.']
    for kernel in kernels:
        values=[]
        for n in [1024,262144,1048576]:
            match=next((e for e in effects if (e['domain'],e['pattern'],e['kernel'],e['n'])==('static','unaligned',kernel,n)),None)
            if match:values.append(f'{match["ratio_of_medians"]:.3f} times at N={n}')
        before,after=EFFECTS[kernel]
        if values:tex.append(f'For {NAMES[kernel].lower()}, the measured {before}-to-{after} speedup was '+', '.join(values)+'. Ratios below one denote slowdowns.')
    forward=[e for e in effects if e['kernel']=='in_place_forward' and e['n']>=8192]
    if forward:
        tex.append(f'For the in-place kernel at lengths of at least 8192, forwarding had a slower median than reuse in {sum(e["ratio_of_medians"]<1 for e in forward)} of {len(forward)} bound-policy/alignment configurations. Thus, eliminating vector loads did not guarantee a runtime improvement. Cache-resident reloads, hardware store forwarding, dependency chains, code placement and system variability are plausible influences; the experiment does not identify a unique microarchitectural cause.')
    if (out/'stack-probe-control/summary.csv').exists():
        cr=read(out/'stack-probe-control','summary');zero={r['mode']:float(r['median_ns']) for r in cr if r['n']=='0'}
        tex.append(f"The dynamic two-stage kernel exposed a separate compiler-policy effect: default GCC probes the pages of its 4 MiB stack temporary before checking the trip count. In a supplemental controlled run at N=0, the median was {zero['gcc']:.1f} ns with default probing and {zero['gcc_noprobe']:.1f} ns with only stack-clash probing disabled. The TinyC backend does not emit these probes. Therefore the very large tiny-call GCC ratios are not attributed to fusion. This supplemental control changes no model weights and is excluded from candidate decision accuracy.")
    tex += [r'\subsection{Limitations and reproducibility}',
        'The evaluation covers four restricted kernels, not a general-purpose C compiler or complete applications. Warm/repeated calls, buffer resets, guest affinity and uncontrolled physical placement limit generalization. Memory-controller traffic, cache misses, energy and hardware store-buffer behavior were not measured. Small calls include substantial dispatch overhead. Constant-bound GCC may unroll or simplify differently. Bootstrap intervals are exploratory and do not correct for multiple comparisons. The correctness matrix provides empirical coverage rather than a proof of compiler correctness.',
        'The source programs, GCC references, generated TAC, optimization traces, representative assembly/disassembly, raw timing CSV, grouped predictions and figures are retained in the application-evaluation directory. Existing BTP report files are not overwritten. The following commands reproduce correctness, measurement and report generation:',
        r'\begin{lstlisting}[basicstyle=\ttfamily\small]',
        'make test-applications','make benchmark-applications','make benchmark-application-stack-control','make report-applications',r'\end{lstlisting}','']
    (dest/'REPORT_SECTION.tex').write_text('\n\n'.join(tex))


if __name__=='__main__':main()
