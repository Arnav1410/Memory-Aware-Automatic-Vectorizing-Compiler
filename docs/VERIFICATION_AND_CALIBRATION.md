# Verification and measured cost calibration

This report records the verification/calibration revision **before store forwarding**.
Its recorded counts/results are historical and retained unchanged. The current
[forwarding report](STORE_FORWARDING.md) covers the expanded six-mode tests and
new experiment. Rerunning evaluation now also includes the forward explicit mode.

This follow-up strengthens the existing compiler. It does not broaden the TinyC
language or replace loop analysis, load reuse, restricted fusion or the scalar
emitter. The original architecture and legality conditions remain documented in
[MEMORY_AWARE_REPORT.md](MEMORY_AWARE_REPORT.md).

## Reproduce the work

Run serially from the repository root, with other substantial workloads stopped.
Paths containing spaces are supported. Scripts are invoked through their
interpreters; no `chmod +x` is required after source ZIP extraction.

```bash
make clean
make build
make test-memory
make test test-tinyc1 test-tinyc2 test-p3-suite test-btp-scalar
make test-source-archive
make calibrate-memory
make test-runtime
make evaluate-memory
make present-memory
```

Requirements are unchanged: x86-64 Linux/WSL, GCC/G++, Make, Flex, Bison, binutils,
Python 3. The calibration probes execute AVX2 directly and require this target
CPU. Generated compiler output still has runtime capability fallback.

`make calibrate-memory` measures and writes a new fitted profile.
`make evaluate-memory` uses the existing profile; it does not silently refit it.
Both scripts accept `--cpu`, `--samples`, and `--output` arguments; evaluation
also accepts `--weights`, `--sizes`, `--dynamic-sizes`, and `--patterns`.
Check `python3 benchmarks/evaluate.py --help` for the exact interface.

Keep the calibration and evaluation commands sequential: running tests or builds
concurrently with measurements changes their execution conditions. Defaults pin
the measurement process to logical CPU 0. This does not establish physical
P-core placement or a fixed frequency under WSL.

## Regression and archive evidence

The complete original `tests/memory/run.py` was retained without changing cases,
lengths or seeds. It completed **25,344 kernel executions**, including GCC
references, and **21,120 complete-allocation comparisons**. The three seeds remain
19, 627 and 2026. Fixed-bound variants, alias layouts, zero/negative trip counts,
scalar fallbacks, reuse invalidation, fusion rejections and executable calls
remain in the suite.

A clean `make clean` followed by `make build` regenerated the scanners with Flex
and the Part 2/3 parsers with Bison. The original regression targets above passed.
The unchanged full memory suite was rerun after the final threshold implementation.
There were no observed compiler-output regression failures in these runs.

One reproducibility defect was fixed: `backend/Makefile` invoked a shell script
as an executable, which fails when archive extraction removes execute bits. It
now uses `bash`. Smoke/scalar/unit scripts use unique temporary directories and
remove them on exit, avoiding shared `/tmp/tinyc_scalar*` collisions. Source
packagers now exclude all frontend binaries, the build tree, private tool-state
directories and Python caches as well as the previously excluded backend binaries.

`make test-source-archive` constructs a source-only ZIP, extracts it to a temporary
path containing spaces, sets **every file to mode 0644**, then performs a fresh
Flex/Bison build and all original regression targets. This test passed; the
extracted smoke script remained non-executable. Freshly linked binaries have the
permissions assigned by the compiler/linker. For an old archive containing stale
binaries, start with `make clean`.

The default translator sanitizer script **does fail in this sandbox** because
LeakSanitizer refuses the ptrace environment. Its diagnostic is preserved; this
is not presented as a successful leak check. The same ASan/UBSan test passes with
`ASAN_OPTIONS=detect_leaks=0`. The model/analysis unit executable also passed
ASan/UBSan with that setting. The script now respects a caller-specified
`ASAN_OPTIONS` instead of overriding it.

Logs are under `build/revision-audit/`: `clean.log`, `rebuild.log`,
`original-memory.log`, `original-regressions.log`, `final-memory.log`,
`final-regressions.log`, `source-archive.log`, `source-archive-summary.log`,
`translator-sanitizer.log`, `translator-sanitizer-no-leaks.log`,
`model-sanitizer.log` and `runtime.log`.

## Calibration design and measured profile

The original profile is preserved in `benchmarks/default.conf`. Its coefficients
are analytical cycle-like units, not measured cycles. The fitted profile is
`benchmarks/calibration/results/calibrated.conf` and explicitly declares
**nanoseconds**. No clock-frequency conversion is applied. Both use the same
candidate generator and memory/dependence legality checks.

Calibration uses separate workloads from evaluation:

* Assembly probes execute loads, stores, `vpaddd` and `vpmulld` with unroll factors
  1, 4, 8, 16 and 32. Arithmetic has up to eight independent register chains.
  Each measurement executes one million groups, after warmup. Probe order is
  shuffled with a fixed seed, with nine samples. Assembly prevents compiler
  deletion/folding of the operations. Disassembly is retained.
* Nonnegative least squares fits `group_time = intercept + unroll * operation_cost`
  using factors 4, 8, 16 and 32. The operation coefficient is the measured slope.
  These are effective throughputs for these loops, not isolated instruction
  latencies or a claim about the CPU's theoretical execution-port limits.
* An empty decrement/branch probe measures loop-control throughput. A streaming
  copy probe spans 1,024 through 2,097,152 elements. Complete destination arrays
  and guard elements are checked. The extra memory coefficient is the mean
  positive excess ns/byte above the small-footprint copy baseline, over the
  larger-footprint regimes. Equal weighting of regimes is specified by the fit;
  it does not pretend there is one real latency for every cache/memory level.
* Generated TinyC **fill, copy, two-output fan-out and three-output fan-out**
  kernels measure scalar versus basic SIMD at trip counts 0, 1, 2, 3, 4, 7,
  256, 4096 and 65536. The first five positive short counts execute no vector
  batches, exposing dispatch/setup and alias-check overhead. A two-variable
  nonnegative fit estimates setup plus cost per pointer pair.
* The generated scalar emitter has overlapping execution of loads, stores and
  arithmetic; summing isolated throughput costs overestimates it. A single
  nonnegative scalar overlap factor is the median measured/predicted body ratio
  across the four generated kernels and the three larger training lengths,
  subtracting the empty-call baseline and leaving streaming penalties unscaled.

Evaluation's add, repeated-input arithmetic, producer-consumer, mixed arithmetic
and stride-2 kernels are not used in these fits. Some lengths overlap between
training and evaluation; there is no per-size lookup table in the compiler.

The recorded calibration on i7-1255U, WSL, logical CPU 0 produced:

| Parameter | Fitted value | Meaning |
|---|---:|---|
| setup | 1.918574 ns | Fixed SIMD-path cost relative to scalar |
| load | 0.116507 ns | Effective operation slope |
| store | 0.185131 ns | Effective operation slope |
| alu | 0.113995 ns | Packed-add operation slope |
| multiply_extra | 0.355644 ns | Packed-multiply slope minus add slope |
| branch | 0.307820 ns | Empty-loop group time |
| guard | 0.435094 ns/pair | Incremental disjointness-check fit |
| memory | 0.019621 ns/byte | Extra streaming penalty above the cache model |
| scalar_scale | 0.475311 | Scalar instruction-overlap factor |
| cache | 49,152 bytes | CPU 0 L1 data-cache size reported through sysfs |
| pressure | 0 | Unidentified soft penalty disabled; hard 16-register limit remains |

These values are a snapshot. Recalibration can change them with frequency,
scheduling and thermal conditions. The exact unrounded values, fit points,
source hashes, CPU/compiler information and all raw samples are in
[`benchmarks/calibration/results`](../benchmarks/calibration/results/fit.json).
The probe has 243 samples and the generated-kernel calibration has 648 samples.

## Cost formula changes

The original formula is preserved when the new coefficients retain their
defaults: `scalar_scale=1` and `multiply_extra=0`. The default configuration remains
analytical; fitted weights are explicitly selected with `--weights`.

The fitted version uses the existing transformed IR counts. It adds
`multiply_extra * packed_multiply_count` to the vector batch estimate and applies
`scalar_scale` to the original TAC instruction subtotal and scalar-tail subtotal.
Unique stream traffic is added separately, so the overlap factor does not scale
array bytes. Reuse reduces issued load instructions, not the unique stream byte
count. Fusion eliminates the temporary's read/write streams. The hard legality
and register-capacity constraints are unchanged.

No fit identifies a meaningful soft register-pressure coefficient; its fitted
value is therefore zero rather than an invented penalty. The original analytical
profile retains its historical value. The cache model is still a single threshold
and extra byte coefficient, not a cache simulator or measured DRAM-traffic model.

## Runtime profitability for unknown trip counts

Auto mode now supports `--runtime-threshold model` (default) and `off`. Explicit
`scalar`, `simd`, `reuse` and `fusion` modes retain their previous policy. Static
bounds retain compile-time selection. Unknown bounds still use the documented
`--assume-trip` value to select a vector candidate; the threshold supplements that
selection. It does not synthesize SIMD if the initial model selected scalar.

The threshold is implemented only for **non-fused** legal loops. Their vector
implementation uses the original scalar TAC for the remainder. Let S be the
scalar cost per iteration, V the vector batch cost and H setup plus guards. With
T = 8B + R, the scalar tail and unique streaming footprint cancel in the cost
difference, leaving a batch saving of `G = 8*S - V`. If G is positive, the first
strictly profitable number of complete batches is:

```
Bmin = floor(H / G) + 1
Tmin = 8 * Bmin
```

The implementation calculates this from the existing estimator, with the equal
memory term removed. It verifies the threshold lies in the supported induction
range. For this measured profile and the evaluation loops, Tmin is eight. A
high-setup test produces a larger threshold; the value is not hardcoded to eight.
The new comparison itself has a small overhead not separately fitted into H.

Loop analysis already proves immediate constant initialization and a single
entry to the optimized prefix. Therefore the emitted signed comparison
`bound < start + Tmin` is equivalent to checking the actual iteration count.
It avoids a subtraction and a duplicate bound load. A small/negative count branches
to the untouched scalar header; a larger count proceeds through the existing
CPU-capability, bound, local-extent and alias checks before vector execution.
Scalar back edges skip the prefix, so there is no repeated threshold dispatch.
The capability helper still runs at function entry; tiny calls therefore retain
some dispatch overhead compared with a scalar-only executable.

Fusion's tail is a transformed scalar computation and its eliminated temporary
changes the byte footprint. The cancellation proof does not apply; fusion keeps
its existing cost selection and guarded scalar fallback. Extending runtime
profitability to fusion is deferred rather than approximating its different tail
with this formula.

`tests/memory/runtime.py` adds **152 complete-array/path-counter checks**. Test-only
assembly instrumentation counts vector body entries without modifying production
or benchmark code. It verifies below/at/above-threshold execution, nonzero starts,
original and fitted profiles, a forced higher threshold, shifted aliases and a
capability cache forced false. The whole original memory suite also passes with
the new default. This tests CPU fallback on the available AVX2 host, not physical
execution on an older CPU.

## Stronger evaluation and interpretation

[`benchmarks/results-v2/RESULTS.md`](../benchmarks/results-v2/RESULTS.md) is generated
from the final paired experiment. It includes all outcomes, including slowdowns.
The experiment contains **11,610 raw samples and 1,290 medians**:

* Static lengths: 1, 7, 8, 17, 1024, 32768, 1048576.
* Dynamic lengths: 0, 1, 3, 7, 8, 9, 17, 64, 1024, 32768. All dynamic lengths use
  the same compiled unknown-bound library; none is specialized to a measured N.
* Five workloads, including a stride-2 case intentionally rejected by this SIMD
  analysis, and both 64-byte-aligned and four-byte-offset bases.
* Scalar, SIMD, reuse, fusion, original auto, calibrated auto and GCC. Dynamic
  tests also include calibrated auto with the runtime threshold enabled.
* Five warmups, nine shuffled paired rounds, adaptive repetitions targeting
  approximately 10 ms per sample, full-array checks before and after timing.
  Initialization and validation are excluded; function-call/dispatch overhead is
  included. GCC uses `-O3 -march=native -fwrapv` without a restrict advantage.

For prediction accuracy, identical explicit-mode function assembly is grouped
and its samples pooled before choosing the fastest group. This avoids giving an
implementation extra chances to win just because multiple mode names produce
it. Exact code identity and runtime branch equivalence are separate CSV fields.
The dynamic cost columns explicitly state their assumed-trip basis; they are not
presented as costs recomputed at the measured runtime length.

Calibration changed the coefficients but **did not change generated choices in
these evaluation cases**. Consequently there is no demonstrated performance or
prediction-accuracy improvement attributable to calibration. Timing differences
between those identical implementations are variability, not a model benefit.

The runtime threshold changes code and can improve short dynamic calls by avoiding
unprofitable vector setup and alias checks. The results table reports paired
speedups and bootstrap intervals, alongside cases where its additional comparison
slows execution. Intervals use 2,000 fixed-seed paired resamples of nine rounds;
they are exploratory and not corrected for multiple comparisons. The first
function (`add`) has the same entry offset across the compared libraries, reducing
one source of code-placement confounding. No universal improvement is claimed.

The original experiment remains in `benchmarks/results`; a first follow-up run
before removing redundant guard work is retained under
`build/revision-audit/initial-evaluation`. No evaluation results were used to fit
the weights or to choose a table of thresholds. Inspecting redundant guard
instructions led to a code simplification, which was then reverified and remeasured.

## Demonstration procedure

See [DEMONSTRATION.md](DEMONSTRATION.md) for the short exact commands and the
native executable walkthrough. `make present-memory` produces source-linked TAC,
analysis/cost traces, assembly, executables, disassemblies and median timings for
reuse, fusion, static cost rejection and dynamic dispatch. Its short timings are
illustrative; the paired experiment above is the evaluation evidence.

Remaining limitations: the existing TinyC subset, conservative aliases, fixed
local arrays, stack-heavy scalar emission, no physical non-AVX2 run, no usable
LeakSanitizer result here, no hardware-counter memory attribution, no calibrated
soft pressure penalty, and no runtime threshold for fused loops. The measured
profile is specific to this environment and does not warrant general claims
about CPU instruction latency, memory bandwidth or superiority over GCC.
