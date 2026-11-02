# Application-oriented compiler evaluation

Four pointwise integer kernels are compiled from TinyC to TAC and native code:

| Directory | Kernel | Intended optimization |
|---|---|---|
| [vector_triad](vector_triad/kernel.c) | `C=A+3B` weighted combination | Basic SIMD and scalar tails |
| [multi_output](multi_output/kernel.c) | Sum, difference and product channels | Read-to-read load reuse |
| [two_stage](two_stage/kernel.c) | Internal intermediate followed by scaling | Restricted loop fusion |
| [in_place_forward](in_place_forward/kernel.c) | In-place update with derived channels | Store-to-load forwarding |

These are simplified computational patterns, not complete applications. The
integer triad is not the official STREAM benchmark. Each `reference.c` includes
its corresponding `kernel.c`, giving GCC exactly the same computation. No manual
SIMD, direct-TAC workaround or compiler-model change is used.

Run serially from the repository root:

```bash
make build
make test-applications       # full source-to-executable correctness and native counts
make benchmark-applications  # checks exact-binary certificate before any timing
make benchmark-application-stack-control # isolate GCC local-array probe overhead
make report-applications     # regenerate tables, figures and academic LaTeX
```

Equivalent Python commands (script executable permissions are unnecessary):

```bash
python3 benchmarks/applications/run.py check
python3 benchmarks/applications/run.py bench
python3 benchmarks/applications/report.py
```

Requirements: the existing compiler toolchain, GNU binutils (`objdump`, `nm`,
`objcopy`), Python 3 and Matplotlib for figure generation. The driver uses C++17.
The existing test suites are unchanged and are not substituted by these benchmarks.

The default experiment uses 17 timed samples, both aligned/unaligned buffers,
16 static lengths and 10 runtime lengths, six compiler modes plus GCC O3 and
GCC O3 with vectorization disabled. Correctness uses all 16 lengths in both
source families, three seeds and six alias layouts. All five arrays and the
return value are checked; the in-place buffers are restored outside timing for
every implementation and sample. GCC references use `-fwrapv` and no `restrict`.

To inspect one kernel independently without overwriting recorded measurements:

```bash
python3 benchmarks/applications/run.py check --kernel in_place_forward \
    --output build/application-forward-check
python3 benchmarks/applications/run.py bench --kernel in_place_forward \
    --output build/application-forward-run
python3 benchmarks/applications/report.py --input build/application-forward-run
```

Replace `in_place_forward` with any directory name from the table. To run a
smaller **timing** matrix while retaining the full correctness preflight:

```bash
python3 benchmarks/applications/run.py bench --sizes 8,1024,262144 \
    --dynamic-sizes 1024 --samples 17 --output build/application-short-run
python3 benchmarks/applications/report.py --input build/application-short-run
```

`--cpu` selects an allowed guest logical CPU; the default is the first allowed
CPU. This does not guarantee P-core/E-core placement under WSL. `--samples` must
be at least 15. A physical non-AVX2 machine cannot run the native GCC references
or timing experiment unchanged, although compiler CPU fallback is instrumented
in correctness tests.

Inspect the full pipeline for the representative in-place kernel:

```bash
cat benchmarks/applications/in_place_forward/kernel.c
cat build/applications/static/1024/in_place_forward/kernel.tac
cat build/applications/static/1024/in_place_forward/auto.trace
cat build/applications/static/1024/in_place_forward/forward.s
objdump -d build/applications/static/1024/in_place_forward/forward.so
```

The native driver executable is `build/applications/runner`. For example:

```bash
build/applications/runner check \
    build/applications/static/1024/in_place_forward inPlaceForward 1024
```

All four kernels use five pointer parameters plus `n`; unused pointers keep the
ABI uniform. The function names are `vectorTriad`, `multiOutput`, `twoStage` and
`inPlaceForward`. Runtime-bound two-stage calls require `n <= 1048576` and an
adequate stack (the local temporary reserves about 4 MiB). Static inputs specialize
the bound and temporary extent equally for TinyC and GCC. Accesses remain within
caller-provided allocations; scalar fallback is not a bounds sanitizer.

Artifacts:

- [RESULTS.md](RESULTS.md): generated technical evaluation and all prediction failures.
- [VALIDATION.md](VALIDATION.md): completed tests, evidence locations and changed-file inventory.
- [REPORT_SECTION.tex](REPORT_SECTION.tex): separate academic report material; the existing BTP report is not overwritten.
- `results/raw.csv`: every accepted timing sample and CPU ID.
- `results/summary.csv`: medians, quartiles, extremes, speedups and machine-code hashes.
- `results/candidates.csv`, `predictions.csv`, `groups.csv`: estimates, decisions and distinct implementation groups.
- `results/instructions.csv`, `gcc.csv`: linked-code counts, region boundaries and GCC diagnostics.
- `results/correctness.{json,csv}`: exact-binary validation certificate and counts.
- `results/effects.csv`: transformation-specific comparisons and exploratory paired intervals.
- `results/environment.json`, `weights.conf`: hardware, flags, hashes and unchanged model coefficients.
- `results/compiler-sources.sha256.json`: preserved compiler-source and model-profile hashes.
- `results/figures/`: PDF, SVG and PNG plots; `results/tables/`: representative CSV tables.
- `results/artifacts/`: retained representative source, TAC, all mode traces/assembly/disassembly and GCC optimization dumps.
- `build/applications/`: every generated configuration, linked library and executable driver.

See [AUDIT.md](AUDIT.md) for suitability and [RESULTS.md](RESULTS.md) for the
measured limitations. Static memory-instruction counts are not DRAM byte counts.

The runtime two-stage reference has a fixed 4 MiB local array. The recorded GCC
driver inserts page probes before testing n, unlike the TinyC emitter. The
supplemental `stack_probe.py` experiment keeps the requested main GCC results
and separately disables only GCC stack-clash probing to quantify this overhead.
Its results are not included in model accuracy and are not attributed to fusion.
The extra flag is a controlled experimental variable, not a deployment recommendation.

The LaTeX section requires `booktabs`, `graphicx` and `listings`. Compile the
optional standalone preview from the repository root (if TeX Live is installed):

```bash
mkdir -p build/application-audit
pdflatex -interaction=nonstopmode -halt-on-error \
    -output-directory=build/application-audit benchmarks/applications/REPORT_PREVIEW.tex
```

The figure paths assume inclusion from the repository root. Adapt `\AppEvalRoot`
when integrating `REPORT_SECTION.tex` into a report stored elsewhere.

The full default experiment takes tens of minutes on the recorded machine,
largely because each tiny dynamic two-stage GCC call probes its fixed stack
array. The shorter timing example above avoids that long small-N probe loop
while retaining the full correctness preflight. No samples are removed from the
recorded full experiment.

Reconcile every stored median, quartile, speedup and grouped decision against the
raw measurements (also done automatically by report generation):

```bash
python3 benchmarks/applications/verify.py
# Additionally check that the current binaries/driver/weights match this run:
python3 benchmarks/applications/verify.py --check-binaries
```

`test_metadata.py` checks trace parsing independently, including fusion rejection
reasons, absorbed loops and runtime thresholds. The final audit corrected a
reporting-only substring match between `fusion_not_selected` and `selected`;
saved plan labels and costs were regenerated from the original traces. Raw
timings, native code and all 208 decision-agreement outcomes were unchanged.
