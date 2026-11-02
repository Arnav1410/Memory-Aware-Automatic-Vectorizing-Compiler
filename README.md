# Memory-aware automatic vectorizing TinyC compiler

A B.Tech compiler project with an executable pipeline:

**TinyC → typed TAC → loop/dependence analysis → legal candidates → memory-aware
cost selection → scalar/AVX2 assembly → native executable.**

The existing scanner, AST stage, simulator and standalone vector demo remain
available. The integrated compiler consumes Part 3 TAC; it does not match sample
names or substitute handwritten kernels.

## Build, test and demonstrate

Requirements: x86-64 Linux/WSL, GCC/G++, GNU binutils, Make, Flex, Bison, Python 3.
The compiler uses C++17; the old frontend remains C++11. No Python packages are
required. Run commands from the repository root; scripts support paths with spaces.

```bash
make build
make test test-tinyc1 test-tinyc2 test-p3-suite test-btp-scalar
make test-memory        # unit analysis + assembled full-array comparisons with GCC
make test-runtime
make test-source-archive # source-only ZIP with non-executable scripts
make present-memory     # short native demonstration, all six modes and GCC
make benchmark-forward  # stateful forwarding workloads; raw CSV and native counts
```

For the complete professor demonstration:

```bash
make demo-memory
```

The benchmark measures actual generated kernels, excludes initialization and
validation, compares all six modes against GCC `-O3 -march=native`, and records
medians, speedups and model prediction agreement. It takes approximately a few
minutes including tests and compilation. Results are machine-dependent.

## Compile your own kernel

```bash
mkdir -p build/demo
./tinyc_part3/tinyc3 examples/memory/kernels.c targets/x86_64_tinyc.conf build/demo/kernels.tac
./backend/x86_64_backend --mode auto build/demo/kernels.tac \
    --trace build/demo/auto.trace > build/demo/kernels.s
gcc -shared -fPIC build/demo/kernels.s -o build/demo/kernels.so
cat build/demo/auto.trace
objdump -d build/demo/kernels.so
```

Functions use the normal x86-64 SysV ABI and can be called from C. For a TinyC
program defining `main`, link its assembly with `gcc program.s -o program`.
The supplied array examples are kernels, so they are linked as a library here;
`make test-memory` also assembles and runs standalone executables.

| Mode | Implementation |
|---|---|
| `scalar` | Typed scalar TAC, including arrays, pointers and calls |
| `simd` | Legal contiguous loops, eight int32 lanes, scalar remainder |
| `reuse` | SIMD plus load reuse with write invalidation |
| `forward` | Reuse plus exact-address vector store-to-load forwarding |
| `fusion` | Reuse plus eligible adjacent producer-consumer fusion |
| `auto` | Minimum estimated-cost legal implementation, including forwarding; pair comparison for fusion |

Additional options:

```bash
./backend/x86_64_backend --mode auto build/demo/kernels.tac \
    --weights benchmarks/default.conf --assume-trip 1024 \
    --alias-policy check --dump-ir --trace build/demo/model.trace > build/demo/model.s
```

Unknown trip counts use the explicitly reported `--assume-trip` estimate, not
an invented exact bound. In `auto`, a model-derived runtime trip threshold also
selects original scalar TAC for short non-fused loops (`--runtime-threshold off`
disables that profitability check). `--alias-policy check` (default) emits runtime range
checks; overlapping pointer ranges execute the original scalar loops.
`--alias-policy reject` rejects SIMD when pointer disjointness is unproven.
AVX2/OS state is checked at runtime and unavailable SIMD falls back to scalar.

## Supported subset and boundaries

* SIMD: canonical `i = constant; i < bound; i = i + 1`, straight-line `int`
  loads/stores at `A[i]`, addition, subtraction, multiplication and invariant
  integer parameters/constants. `while` with the same TAC shape also works.
* Scalar fallback: other loop shapes, recurrences, strides, conditionals,
  division and supported calls. Full arrays and return values are checked.
* Fusion: adjacent loops with the same induction variable/domain; one internal
  fixed array temporary, non-escaping, no use outside the pair. General fusion
  and a separate scalar-fusion candidate are not implemented.
* int32 arithmetic uses two's-complement wrapping for `+`, `-`, `*`; validation
  uses GCC `-fwrapv`. Valid pointer bounds and initialized data remain required.
  Byte-offset arithmetic follows the original TAC's int32 scaling.
* Up to six integer/pointer parameters, int/void returns and fixed one-dimensional
  local int arrays. Unsupported types, globals, shadowed names and unsupported
  opcodes are compile-time errors, not silently skipped computations.
* The existing TinyC grammar requires declarations before `for`; identifiers
  contain letters/digits, and there is no preprocessor, VLA or `restrict` syntax.
* The new backend requires metadata from the current Part 3 translator. The old
  `x86_64_backend file.tac targets/x86_64_tinyc.conf` invocation remains available
  for the original scalar exercises.

See [the technical report](docs/MEMORY_AWARE_REPORT.md) for exact legality,
cost assumptions, measured results and limitations, and
[the baseline audit](docs/MEMORY_AWARE_AUDIT.md) for the original capabilities.
[The original README](docs/LEGACY_README.md) preserves the previous lab commands.

See the [forwarding report](docs/STORE_FORWARDING.md) for its exact proof, liveness
handling, test counts and machine instructions, the [changed-file inventory](docs/FORWARDING_CHANGES.md), and the [professor walkthrough](docs/DEMONSTRATION.md)
for source → TAC → IR stages → costs → native execution.

## Evaluation artifacts

* [Application-oriented evaluation](benchmarks/applications/README.md): four source-to-native
  kernels, full-array validation, 17-sample measurements, GCC comparisons, cost-model
  assessment, figures and separate academic report material. Run `make test-applications`,
  `make benchmark-applications`, then `make report-applications`.

* [Forwarding measurements](benchmarks/results-forward/RESULTS.md): raw samples,
  native counts, speedups/slowdowns and code-group prediction accuracy.
* [Calibration and runtime dispatch](docs/VERIFICATION_AND_CALIBRATION.md), with
  the [recorded earlier evaluation](benchmarks/results-v2/RESULTS.md).
* `make calibrate-memory` remeasures the separate training probes. The resulting
  profile is opt-in with `--weights benchmarks/calibration/results/calibrated.conf`.
* `make evaluate-memory` compares original/calibrated models and runtime dispatch;
  calibration results are not refitted to forwarding benchmarks.

* [Measured results](benchmarks/results/RESULTS.md): runtime table and model disagreements.
* `benchmarks/results/raw.csv`: every measured sample and repetition count.
* `benchmarks/results/summary.csv`: median runtime, speedups and static IR counts.
* `benchmarks/results/predictions.csv`: predicted and observed winners, equivalence
  between identical generated modes, and timing differences.
* `benchmarks/results/environment.json`: CPU, OS, compiler, flags and CPU affinity.
* `build/benchmarks/<N>/`: source, TAC, all assembly, traces and disassemblies.
* `build/memory-tests/`: complete-array fixtures, binaries and rejection traces.

To run a shorter repeat without overwriting the recorded full results:

```bash
python3 benchmarks/run.py --sizes 8,1024,32768 --samples 5 --output build/quick-results
```

The default cost weights are explained analytic starting assumptions, not fitted
hardware measurements. The separate opt-in fitted profile is empirical and machine-specific;
its historical evaluation did not change choices, so no calibration-driven
improvement was established. The scalar emitter spills TAC values, so speedups against
it include improvements beyond SIMD lane width. Compare with GCC as well; fewer
vector loads do not by themselves establish lower DRAM traffic or faster code.
Run benchmarks without concurrent tests/builds; use serial `make demo-memory`.
