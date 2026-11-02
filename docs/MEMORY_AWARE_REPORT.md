# Memory-aware automatic vectorizing compiler

Current extensions: [safe store-to-load forwarding](STORE_FORWARDING.md) and
[measured calibration/runtime dispatch](VERIFICATION_AND_CALIBRATION.md). The
sections below record the original architecture and initial evaluation; current
mode/test counts and results are in those follow-up reports.

## Architecture and integration

This implementation extends the supplied compiler; the baseline audit is in
[MEMORY_AWARE_AUDIT.md](MEMORY_AWARE_AUDIT.md). It preserves Parts 1/2, Part 3's
parser and semantic translation, the scalar simulator, the legacy backend
invocation, and the standalone vectorization demo.

The integrated path is:

1. Part 3 emits the same four-field TAC plus ignored-by-legacy-readers `#@target`,
   `#@function` and `#@symbol` comments. Metadata includes parameter order,
   int32/pointer/local-array types, extents and compiler-temporary identity.
2. `backend/memory/program.cpp` loads metadata and reuses `loadTac`, checks target,
   function boundaries, names, types and labels. The shared loader now preserves
   unary/address/load operand arity. Shadowed names are rejected because the
   historical TAC does not rename source locals by scope.
3. `analysis.cpp` recognizes counted-loop control flow and symbolically traces
   integer scaling and pointer additions. Its `Loop` records induction, start,
   bound, read/write bases, contiguous strides, legality and required guards.
4. A lane-value IR represents ordered loads/stores, broadcasts and arithmetic;
   operands are integer IDs. Address-calculation temporaries are removed only
   after proving the corresponding `base + 4*i` address. Dead nodes are pruned.
5. The same IR supports load reuse, store-to-load forwarding, restricted producer-consumer fusion,
   liveness/register-pressure measurement and cost estimation. No assembly-text
   substitution or workload-name dispatch is used.
6. `cost.cpp` estimates legal generated candidates; `emitter.cpp` selects and
   lowers them. Unsupported loop shapes retain scalar TAC. Unsupported *scalar
   instructions/types* cause explicit compilation failure rather than skipping
   work. The driver buffers output, so errors do not leave partial assembly.

The new entry point is `backend/x86_64_backend --mode MODE input.tac`. The old
positional target-configuration invocation remains unchanged. Mode selection
therefore participates in the actual compiler/backend rather than running a
separate SIMD demonstration.

## Scalar execution and ABI

The typed emitter implements SysV x86-64 functions with up to six int32/pointer
register arguments, int/void returns, stack frames, address-of, int32 loads and
stores, pointer addition/subtraction, integer arithmetic, comparisons, branches
and calls. Parameters are saved before the optional CPU capability helper call.
Stack alignment is preserved and callee-saved registers are untouched (the
CPUID helper saves/restores RBX). Local arrays occupy distinct stack regions.
Array data never uses 32-bit truncated pointers. Integer byte offsets are sign
extended for pointer addition, matching the existing TAC's int32 offset type.

`+`, `-`, `*` wrap modulo 2^32; GCC references use `-fwrapv`. Division by zero,
INT_MIN/-1 division, invalid shifts, invalid pointers, out-of-bounds memory and
uninitialized values are outside the defined input contract. Tests primarily
use small values; alias-induced recurrences are also compared under wrapping
semantics. The old simulator uses long long scalars, so it remains the reference
only for the old small scalar cases, not array kernels or overflowing arithmetic.

The frontend does not implement full C: no preprocessing, `restrict`, VLAs,
structs or declarations in a for initializer. Its identifiers contain letters
and digits. The new backend rejects floats/chars, multidimensional arrays,
pointer-to-pointer values, globals, stack-passed arguments and shadowed TAC names.
Local arrays must have 1..1,048,576 elements; zero *iterations* are supported.
There is no bounds sanitizer. Even when a SIMD extent check falls back, the
caller must still satisfy the original scalar program's memory contract.

## Loop legality and dependence analysis

Recognized TAC has an immediate constant initialization, header label, signed
`if< i bound body`, exit jump, body label, straight-line body, `i+1` assignment,
back edge and exit label. Initial i is nonnegative; stride is one. Both `for` and
`while` may generate this form. Analysis rejects extra entries, nested control
inside the body, modified/address-exposed induction or bound, observable scalar
assignments and compiler temporaries that remain live outside the loop.

Symbolic values carry an affine `(coefficient, constant)` and optional base or
lane-value ID. A memory access is accepted only when its proven address is
exactly `base + 4*i`. Offset, strided, nonlinear and unknown pointer accesses are
rejected. An induction-dependent *data* expression is not implemented. Constants
and invariant, non-address-exposed integer parameters can be broadcast.

All accesses to a particular base are at the same iteration index. This permits
in-place per-element operations but excludes loop-carried dependencies such as
`a[i] = a[i-1] + b[i]`. Non-temporary scalar assignments such as reductions remain
scalar. Array stores are the only accepted loop side effects. Calls, division,
branches, pointer escapes and other unsupported operations prevent SIMD.

Incoming pointer parameters are analyzable only if they are never reassigned in
the function. This condition is essential to the proof that they cannot refer to
this invocation's local stack arrays. Local pointer aliases are conservatively
unsupported by analysis, though supported by scalar execution. Distinct local
arrays are disjoint. Different incoming pointers may alias: by default SIMD is
conditional on pairwise disjoint complete `[base+4*start, base+4*bound)` ranges.
Unsigned overflow of an interval calculation also rejects the fast path.
Checking even read/read pairs is conservative and may lose legal opportunities.
`--alias-policy reject` disables speculative vectorization instead.

Bounds must remain in the supported int32 byte-offset range (at most 536,870,911).
Dynamic bounds are checked before SIMD; local extents are checked too. Invalid
fast-path bounds return to original scalar TAC. Each selected fast path checks a
cached AVX2 capability flag; initialization tests CPUID AVX/OSXSAVE, XGETBV's
XMM/YMM state bits and CPUID AVX2. No AVX instruction executes on capability
failure. No AVX-512 instructions are emitted.

## SIMD, reuse and fusion

Eight int32 elements form one AVX2 batch. The loop computes an endpoint by
rounding the remaining iteration count down to a multiple of eight, emits
`vmovdqu`, `vpaddd`, `vpsubd`, `vpmulld` and broadcasts as needed, then updates the
scalar induction slot and jumps to the original scalar header for the remainder.
Back edges target that scalar header rather than re-entering vector setup.
Nonzero starts, no iterations and short tails retain the original final i value.
`vzeroupper` executes after the vector body, before any scalar remainder.

Basic SIMD preserves the ordered lane-IR loads. Reuse maintains a base-to-value
availability map and substitutes an existing load ID only while that value is
valid. A same-base write invalidates it. The disjointness proof/guard makes other
base writes harmless. Dead nodes are removed, last uses determine register
lifetime, and a simple allocator uses at most 16 YMM registers. Candidates that
need more registers are rejected rather than spilling incorrectly. Thus reduced
load counts describe real emitted instructions, not a textual peephole rewrite.

Fusion is limited to two adjacent analyzed loops with identical induction
variable, constant start and bound. The producer must write exactly one local
array exactly once per element and must not read it. The consumer must read it
and must not write it or overwrite any known producer input. All references to
the temporary must be local address operations inside the loop pair; later reads,
address escape or other uses reject fusion. Unexpected control-flow entry also
rejects it. Cross-loop incoming pointer aliases require the combined range guard.

The producer store is removed and consumer loads of the temporary are replaced
by its computed value ID. Reuse then runs on the merged IR. The fused vector path
has a scalar remainder lowered from the same fused lane IR, updates i, and jumps
to the second exit. The original two loops remain as the capability/alias fallback.
The temporary stack allocation is retained for that fallback; its *fast-path
memory accesses* are eliminated. General fusion, arbitrary scalar fusion and
eliminating the temporary stack reservation are not claimed.

## Cost model and actual selection

All weights are in approximate cycle units and are configurable in
`benchmarks/default.conf`. These defaults are analytic priors, **not calibrated
measurements of the i7-1255U**. They deliberately make an inspectable baseline:
load and arithmetic weights of 0.5 assume two independent operations per cycle;
store and loop-control weights of 1 charge one cycle. Setup 12 approximates the
cached dispatch and loop-end setup; guard 3 per pointer pair approximates overlap
comparisons in a warm predictable path. These are throughput approximations,
not instruction latencies. Real instruction dependencies, frontend bandwidth,
hybrid-core differences and frequency changes can invalidate those assumptions.

For scalar, the model traverses the actual original TAC body, counting operand
slot reads, result spills, address operations, arithmetic and control. This is
necessary: this educational scalar backend spills aggressively and is not an
optimized scalar compiler. For vector candidates the model uses transformed IR
counts plus base-pointer slot loads, invariant loads, two broadcast instructions
per broadcast and loop-control operations.

Let T be trip count, B=floor(T/8), R=T mod 8. The vector estimate is

```
setup + guard * number_of_pointer_pairs
+ B * [load * (vector_loads + address_loads + invariant_loads)
       + store * vector_stores
       + alu * (vector_alu + 2*broadcasts + 3)
       + branch + pressure * max(0, peak_live_vectors - 12)
       + memory_access_penalty]
+ R * scalar_remainder_cost
```

The model rejects over-16-register candidates outright. Its soft penalty above
12 is a conservative heuristic for reduced overlap of live values across
iterations; it is not a measured spill count. The 32 KiB working-set threshold is
a configurable conservative cache assumption, not automatic cache discovery.
The extra memory coefficient defaults to 1/32 cycle/byte. It is a rough traffic
penalty and does not model cache-line allocation, writeback, prefetch or DRAM.
Above the threshold the penalty per batch is `32 * (unique read bases + unique
written bases) * memory`; scalar iterations use 4 instead of 32. Repeated loads
from the same element therefore increase issued-instruction cost without
pretending to increase the unique streaming footprint. Fusion reduces the
temporary's read/write streams. This remains an estimate of memory-system work,
not a measurement of DRAM traffic.

For constant bounds T is exact; otherwise the default assumption is 1024 and is
printed in diagnostics. `--assume-trip N` changes that assumption. Runtime guards
handle correctness, not dynamic model recalibration. The benchmark supplies
literal bounds in the input source, so benchmark selections use exact counts;
no benchmark sizes or function names are embedded in the optimizer.

Each loop compares scalar, basic SIMD and legal reuse. A legal fused pair is
compared against the sum of the independently cheapest first and second loop
implementations, avoiding the mistake of comparing two loops with only one.
Equal costs prefer the earlier, simpler candidate. Explicit modes force their
requested transformation when legal. Auto selects the minimum estimate and that
choice directly controls emitted assembly. Short constant loops and a high setup
weight demonstrate rejection of vectorization/fusion. Diagnostics report bounds,
accesses, legality, rejected fusion opportunities, each cost, selected variant,
vector load/store counts and live-register estimates.

## Reproducible validation

`make test-memory` runs separate analysis/IR/cost unit tests and compiles real
TinyC sources into five sets of assembled libraries. It compares return values
and **every element of all four backing allocations**, including untouched input
and guard elements, against independently compiled GCC C. Tests cover negative,
zero, 1, 7, 8, 9, 15, 16, 17, 1003 and 1024 trip counts; three fixed seeds; unaligned
bases; independent, identical and shifted overlapping pointers; add/subtract/
multiply; load invalidation; temporary fusion; escaping and later-used temporaries;
different domains; external temporaries; recurrences; stride; branches; calls;
reductions; division; changing bounds; nonzero starts; and pointer rebinding.
Constant-length variants exercise auto selection. High setup weights must remove
AVX2 from emitted code. Invalid settings must fail without assembly output.

A generated assembly variant with its capability cache preset to false tests the
CPU fallback branch on this AVX2 machine; this is not a claim to have executed on
physical non-AVX2 hardware. Standalone generated executables also call generated
functions and return checked values. `objdump` confirms actual AVX2 instructions
in linked libraries. The analysis/unit tests were additionally run under
ASan/UBSan with leak detection disabled because LeakSanitizer is incompatible with
this sandbox's ptrace environment. Existing regression targets remain passing.

## Benchmark protocol

`python3 benchmarks/run.py` builds four kernels (add, repeated-input arithmetic,
producer-consumer, and a more complex mixed expression) at 1, 7, 8, 17, 1024,
32768 and 1048576 elements. Each mode and GCC uses the same generated input source.
GCC flags are `-O3 -march=native -fwrapv -shared -fPIC`; there is no `restrict`
contract advantage for either compiler. GCC may optimize the same loops further.
The mixed expression is an additional evaluation workload; no coefficients were
fitted using it or the other workloads, so this is not a calibration/holdout study.

A C++ runner loads the real kernels with `dlopen` and validates complete arrays
before and after timing. Initialization, validation and file I/O are outside the
timed interval. Timing uses `steady_clock`, five warmups, adaptive repetitions
(target approximately 10 ms per sample), nine samples and a fixed shuffled method
order. Indirect external calls cannot be optimized away; return values feed a
volatile checksum outside the timed interval. Function-call overhead remains in
all measurements and is significant for tiny arrays. CPU affinity is pinned to
one permitted logical CPU when available and recorded in environment.json.

The input buffers and output buffers are reused across calls, making this a warm
steady-state workload. Small sets fit in L1; medium sets exceed the modeled L1;
large arrays plus the producer temporary span many MiB and can exceed the shared
cache. It is not a controlled cold-memory or hardware-counter experiment.
WSL scheduling, hybrid CPUs, power limits, thermal drift and turbo cause variation.
No governor changes or privileged performance counters are required.

CSV output contains every sample, median runtimes, speedups vs the generated
scalar and GCC, static lane-IR operations, emitted vector loads/stores, model
selection and observed winner. Counts are per vector batch, summed over separate
loops where applicable, not total dynamic memory traffic. The original fallback
instructions remain in the binary. GCC counts are `NA`: GCC may fold memory
operands into arithmetic, so counting `vmovdqu` alone would be misleading.
Identical compiler modes are detected by their emitted function assembly when
reporting prediction agreement; a second column reports whether auto is within
5% of the fastest explicit mode. These are descriptive results, not confidence
intervals or proof of statistical significance.

## Observed results and conclusions

The completed i7-1255U/WSL experiment is rendered directly from measured CSV in
[Measured results](../benchmarks/results/RESULTS.md). That report contains the
runtime comparison table, prediction disagreements and agreement counts; it is
regenerated with each benchmark run. The raw dataset has 1,512 samples: four
workloads, seven lengths, six modes and nine samples per combination.

Both memory transformations reduce actual emitted vector instructions: repeated
inputs go from four loads/two stores to two loads/two stores, while the producer
pair goes from three loads/two stores to two loads/one store. The mixed expression
also benefits from reuse (six input loads become two). Runtime results do not
uniformly follow these static reductions. In particular, basic SIMD sometimes
wins despite additional loads, and GCC is substantially better on tiny fixed
trip counts. Fusion can improve large producer-consumer workloads by eliminating
the temporary's passes. These claims are limited to the measured inputs and warm
execution protocol; the CSV retains contrary results rather than omitting them.

Auto comparisons distinguish code-equivalent variants from noisy timing winners.
The model is useful and affects generated code, but it is not a calibrated CPU
simulator or a guarantee of fastest execution. The default model keeps unique
stream traffic separate from repeated instruction loads. Future work should
measure core-specific throughput and bandwidth, fit setup/guard costs separately,
validate predictions on untouched kernels, improve scalar register allocation,
and add larger legal subsets only with corresponding dependence proofs.

## Milestone completion and changed modules

* **A:** baseline audit and existing targets passed; shared typed TAC/loop analysis
  added in `backend/memory/compiler.hpp`, `program.cpp`, `analysis.cpp`; metadata
  added in `tinyc_part3/translator.*`, operand loading corrected in `backend/ir.cpp`.
* **B:** ABI-correct scalar/AVX2 lowering, CPU/alias guards and remainders added in
  `backend/memory/emitter.cpp`, integrated through `x86_64_backend.cpp` and Makefiles.
* **C:** load availability, invalidation and register liveness added in
  `analysis.cpp`; unit and full-array tests demonstrate the load-count reduction.
* **D:** restricted fusion and temporary forwarding added in `analysis.cpp`, with
  fused tails and original-loop fallback in `emitter.cpp`; negative tests reject
  escape, later use, incompatible domains and external temporaries.
* **E:** configurable costs in `cost.cpp` and `benchmarks/default.conf`; traces and
  actual candidate selection in `emitter.cpp`; small-loop and forced-cost tests pass.
* **F:** `tests/memory/`, `benchmarks/`, `examples/memory/`, Makefile demo targets,
  concise README, audit and this report. The executable suite covers 25,344 kernel
  executions (including GCC references), giving 21,120 generated-code/full-array
  reference comparisons, plus standalone executable and rejection checks.

The unimplemented areas are the explicit subset limits above, not silently
accepted transforms. Original tests and the simulator remain available.
