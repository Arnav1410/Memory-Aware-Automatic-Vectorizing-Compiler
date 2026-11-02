# Safe vector store-to-load forwarding

This is an incremental pass over the compiler's analyzed vector IR. The frontend,
TAC format, scalar implementation, runtime alias guards and allocator are retained.
The initial audit confirmed that this IR already carries the stored value as the
store node's `a` operand. No new memory-access forms or spilling were needed.

## Definition and implementation

Read-to-read reuse substitutes an earlier **load** while memory remains unchanged.
Store-to-load forwarding substitutes the value of the latest **store** to the exact
same region. The store stays in its original position. For example:

```text
Basic/reuse IR                      Forwarded IR (IDs compacted)
0 load a                            0 load a
1 load b                            1 load b
2 add 0,1                           2 add 0,1
3 store a,2                         3 store a,2
4 load a                            4 broadcast 3
5 broadcast 3                       5 multiply 2,4
6 multiply 4,5                      6 store c,5
7 store c,6
```

The implementation is `forwardStoredValues()` in `backend/memory/analysis.cpp`.
It follows the unchanged `reuseLoads()` pass. A fresh map `stored[base] = valueID`
is created for each invocation/body. On a store, its already-remapped `a` operand
becomes the available value. On another store to the same base it is replaced;
on a matching load, the old load ID maps to that value instead of appending a
load. Ordinary loads do not populate this map: read reuse remains a separate pass.

All other nodes, including every store, remain ordered. Operands are remapped as
nodes are copied. Existing dead-node pruning roots **all stores**, removes unused
values and compacts IDs again. `validNodes()` checks supported node signatures,
nonempty memory tokens, references to earlier nodes and prohibition on using a
store as a value. Unsupported operations/memory bases reject forwarding before
any transformation. There is no availability state carried between vector batches,
loop plans, compiler functions or scalar fallback executions.

## Exact legality contract

The pass accepts only IR belonging to a `Loop` that passed existing analysis:

* Straight-line counted loop, invariant bound, nonnegative constant initial
  induction and step one; no additional entry/control path into the body.
* Every memory address is proven by TAC affine analysis to be **base + 4*i**,
  with int32 loads/stores. One vector body denotes eight consecutive iterations.
  Equal base tokens therefore denote exactly equal eight-lane regions, with
  identical types/widths. Tokens alone are not a general pointer-equivalence test.
* All stores/loads in that body use the same region for a given base. The latest
  earlier store supplies the value. Node values are immutable; overwriting memory
  does not alter a previously computed register value.
* No volatile/atomic operations, calls, barriers, arbitrary branches, partial
  stores, unknown side effects, strided/shifted indexing or general pointer
  expressions are represented by this IR. These constructs are outside this
  pass; unsupported scalar language/TAC can still be a compile-time error.
* Distinct local arrays are distinct stack objects. Incoming pointer parameters
  are accepted only when never reassigned in the function; they cannot refer to
  this invocation's local arrays. Local pointer aliases are not vectorized.
* Potentially aliasing incoming bases retain the existing **pairwise full-range
  disjointness guards**, including read/read pairs and unsigned-overflow checks.
  Different pointer names are never assumed disjoint. Overlap takes original
  scalar TAC, preserving sequential behavior, including shifted dependencies.

On this guarded vector path, another base's store cannot invalidate the saved
value. A same-base store updates it. If this IR gains unknown memory operations
in future, this pass must reject them or gain a sound invalidation/alias analysis;
the current opcode validator deliberately rejects them. The CPU/OS-state checks,
local-array bounds checks and int32 byte-offset limits are unchanged. Callers must
still provide valid scalar-program allocations; fallback is not a bounds sanitizer.

## Liveness, candidates and cost

Whole-body use counts are recomputed by the existing emitter, including all
forwarded consumers. Registers are freed only at the last use, not at a store.
`statistics()` recomputes peak live vector values using the allocator's exact
allocate-before-release convention. Candidates above **16 YMM registers** are
excluded; explicit `forward` then uses legal reuse (or basic SIMD if reuse is also
infeasible). No spilling is introduced. The stress fixture reaches peak 20 and
falls back; an eight-value fixture forwards safely at peak 10.

| Mode | Behavior |
|---|---|
| scalar | Original typed scalar TAC |
| simd | Basic legal AVX2 |
| reuse | Existing read-to-read reuse, invalidating at writes |
| forward | Reuse followed by store-to-load forwarding |
| fusion | Existing restricted fusion; otherwise existing reuse |
| auto | Minimum estimated-cost legal candidate, now including forward |

Trace fields `reused_loads` and `forwarded_loads` separate the two reductions for
non-fused candidates. `--dump-ir` includes each candidate's numbered graph. The
model uses actual resulting load/store/arithmetic counts and peak registers.
Forwarding has no special discount. Pointer reload/address-setup costs decrease
when an actual load is removed, and the existing soft pressure penalty applies.
A high-pressure-cost fixture selects reuse despite a legal forwarding candidate.

The unique-stream memory term is held equal for **non-fused** variants, based on
the original loop accesses. Reading a freshly written element need not fetch a
new line from memory, so removing that read earns no fictitious streaming-byte
benefit. Fusion continues to account for genuinely removed temporary streams.
The model remains an approximate issued-instruction/working-set model, not a
hardware-counter or store-buffer simulation.

Example default analytical costs for unknown-bound `basicForward` (explicitly
assuming 1024 iterations): SIMD/reuse 1365, forwarding 1237; loads 3 versus 2,
stores 2, peak 3. Auto selects forwarding. For a known trip count of one it
selects scalar. The dynamic forwarding candidate uses the existing profitability
threshold (eight iterations for this example). All non-fused candidates execute
the original scalar remainder, so its cost and unique-stream traffic still cancel
in the threshold formula. Tests instrument both sides, including a high setup cost.

Fusion remains a separate candidate with its existing transformed scalar tail.
Forwarding is **not applied inside fused IR** in this change. Both independent
loops' legal candidate sets now include forwarding when comparing their summed
cost with fusion. Existing `fusion` classification in cost/threshold code is
retained, with no new fused-forward name to misclassify. A producer plus a
store/reload consumer is tested in every mode. Fused runtime thresholds remain
unsupported, as before.

## Executable and machine-code evidence

All results below were obtained by running the compiler, assembling/linking its
output, executing full arrays and comparing return values and all allocation
contents (including guards) to GCC `-O3 -march=native -fwrapv`.

| Positive fixture | SIMD loads | Reuse loads | Forward loads | Stores in all three |
|---|---:|---:|---:|---:|
| basicForward (A) | 3 | 3 | 2 | 2 |
| multipleReads (B) | 5 | 3 | 2 | 3 |
| multipleStores (C) | 4 | 4 | 2 | 3 |
| longChain (D) | 7 | 5 | 2 | 4 |
| localForward (E) | 3 | 3 | 2 | 2 |

These are **native vector-body** counts per eight elements, obtained with
`objdump` on linked libraries compiled with `-Wa,-L`. Retained `.L..._vector`
and `.L..._tail` symbols delimit bodies; scalar guards/fallback/tails are excluded.
`tests/memory/native_counts.py` counts memory-to-YMM and YMM-to-memory `vmovdqu`
separately. It verifies register indices, and the tests compare these native
counts with both expectations and compiler traces. Removed instructions are the
redundant memory-to-YMM `vmovdqu` loads (and their associated pointer setup).
The arithmetic consumes the stored source's still-live register. For D, reuse
first removes two read-to-read loads, then forwarding removes three more.

Verification performed:

| Command | Outcome |
|---|---|
| `make clean` / `make build` | Passed; Flex/Bison regenerated scanner/parser sources |
| `make test test-tinyc1 test-tinyc2 test-p3-suite test-btp-scalar` | Passed original backend/vector/frontend/scalar regressions |
| `make test-memory-unit` | Passed original and new transformation/liveness/IR validation assertions |
| `make test-memory` | Passed: **41,400 kernel executions, 34,560 full-allocation comparisons** |
| `make test-runtime` | Passed: **152** additional complete-array/path checks |
| `make test-source-archive` | Passed source-only ZIP, mode-0644 files, path with spaces, Flex/Bison rebuild/original regressions |
| `make present-memory` | Passed native linked demonstration in all six modes and GCC |
| ASan/UBSan unit executable | Passed; `ASAN_OPTIONS=detect_leaks=0` because LeakSanitizer is unavailable in this sandbox |

The original matrix is unchanged except adding the `forward` mode: 29,568
executions / 25,344 comparisons. The additive forwarding suite contributes
11,832 / 9,216, including **1,296 instrumented path checks**. Repeated runs are not
counted as additional distinct coverage. GCC reference invocations count as
executions, not comparisons; standalone-main and compile-rejection checks are
additional. Runtime-suite invocations are separate from these totals.

Lengths include -3, 0, 1, 7, 8, 9, 15, 16, 17, 1003 and 1024; seeds remain
19, 627 and 2026. Original fixed-bound variants, all four alias layouts and all
old negative cases are retained. Added cases cover same/shifted input/output
aliases, latest stores, different bases, local arrays, multiple held values,
invalid/forward/store-node references, dead values, pressure rejection, model
rejection, fusion interaction, and threshold boundaries. CPU-disabled tests set
the cached capability flag to zero and prove no vector body executes; this is
not a test on physical non-AVX2 hardware. The original suite covers recurrence,
stride, division, branches, calls, changed bounds and other scalar fallbacks.
Unit lane interpretation also checks wraparound at signed int32 boundaries.

Logs: `build/forward-audit/`. Focused native listings/counts and generated fixtures:
`build/forward-tests/`. The unchanged earlier verification/calibration experiment
is documented in [VERIFICATION_AND_CALIBRATION.md](VERIFICATION_AND_CALIBRATION.md).

## Performance and reproducibility

Run `make benchmark-forward` serially, after correctness tests. The default
experiment uses seven workloads (four forwarding patterns, repeated-read and
addition controls, plus fusion), seven sizes from 1 to 1,048,576, both 64-byte
aligned and four-byte-offset bases, all six compiler modes and GCC, and nine
samples. Source trip counts are static and exact. Calibration is not refitted to
these workloads; `--weights` can explicitly select the existing separate profile.

Stateful input kernels require special care: `benchmarks/forward_runner.cpp`
resets **all four buffers outside timing for every mode/sample**, uses identical
repetition counts, performs five warmups, and compares both a single call and
the final repeated-call result/return checksum against independent repeated GCC
execution. GCC uses `-fwrapv`, so repeated multiplication and addition remain
defined under the stated int32 contract. External function calls and a volatile
return checksum prevent elimination. Initialization, validation and I/O are
excluded. Call/dispatch overhead is included.

See [the measured report](../benchmarks/results-forward/RESULTS.md) for actual
medians, gains, slowdowns, code-equivalence groups, model accuracy and raw CSV.
The recorded i7-1255U run contains **6,174 samples, 686 medians and 98 prediction
cases**. Auto agrees with the fastest equivalent explicit code group in **83/98**
cases and runs within 5% in **81/98**. Representative unaligned observations:

| Kernel / N | Reuse median | Forward median | Reuse / forward |
|---|---:|---:|---:|
| Basic / 1024 | 157.4 ns | 132.9 ns | 1.184x |
| Long chain / 1024 | 331.6 ns | 230.6 ns | 1.438x |
| Long chain / 32768 | 14.230 us | 17.547 us | 0.811x |
| Multiple stores / 1048576 | 579.690 us | 634.995 us | 0.913x |

These are measured medians, including losses, not guaranteed or causal speedups.
The raw paired intervals are in the report. Code-equivalent controls also show
variability. Static instruction savings are established; speedups remain empirical. Hardware
store forwarding and cache hits can make removed loads inexpensive. Longer
value dependencies/register lifetimes can matter. These are possible mechanisms,
not measured causal explanations; no hardware counters are collected. WSL turbo,
thermal state, code placement and scheduling also affect rankings.

TinyC support is deliberately restricted. There is no general pointer analysis,
volatile/atomic vectorization, spill allocator, forwarding across iterations,
inter-block forwarding, or forwarding within the fused candidate. No claim of
full C support, universal speedup, or novelty beyond this compiler integration
is made. Exact demonstration commands are in [DEMONSTRATION.md](DEMONSTRATION.md).
