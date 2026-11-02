# Professor demonstration: exact commands

Run from the repository root. No script execute permission is required.

## 1. Rebuild from sources and verify

```bash
make clean
make build
make test-memory
make test-runtime
```

The expanded memory suite checks 34,560 complete generated-output/reference
comparisons (all original cases retained, plus forwarding). Runtime tests additionally count executed vector batches. To verify
archive extraction and the original lab stages:

```bash
make test-source-archive
make test test-tinyc1 test-tinyc2 test-p3-suite test-btp-scalar
```

## 2. One-command native demonstration

```bash
make present-memory
```

Input: `examples/memory/presentation.c`. The driver checks every element of all
four arrays, including guards, before timing and after every sample, including stateful forwarding outputs.
Initial buffers are restored outside timing for each sample.
The command creates and runs actual linked executables for scalar, SIMD, reuse,
forward, fusion, auto and GCC. All artifacts are under `build/presentation/`.

Open these together during the presentation:

1. `examples/memory/presentation.c`: supported TinyC loops.
2. `build/presentation/presentation.tac`: emitted TAC, including original address
   calculations, labels and type metadata.
3. `build/presentation/simd.trace` and `reuse.trace`: legal loop/access analysis,
   candidate estimates and **four versus two vector loads** in `reuseExample`.
4. `build/presentation/fusion.trace`: `fusionExample`'s internal temporary is
   eliminated; the pair has **two loads/one store**, versus three/two separately.
5. `build/presentation/forward.trace`: `forwardExample` shows basic/reuse IR
   with three vector loads and forwarding IR with two, keeping both stores.
   Every trace includes numbered candidate nodes (`--dump-ir`).
6. `build/presentation/auto.trace`: `shortExample` has trip count one and selects
   scalar. `dynamicExample` has an unknown bound and reports a model-derived
   runtime threshold. Its N=7 and N=17 calls exercise both sides.
7. `build/presentation/auto.s` and `auto.objdump`: emitted and linked AVX2
   instructions plus scalar fallback. `build/presentation/auto` is the executable.
8. `build/presentation/measurements.csv`: nine-sample medians for each mode/kernel.
   `instruction-counts.csv` comes from linked vector-body disassembly, not the trace.

Manual commands for the same pipeline:

```bash
mkdir -p build/manual
./tinyc_part3/tinyc3 examples/memory/presentation.c \
    targets/x86_64_tinyc.conf build/manual/program.tac > build/manual/frontend.log
./backend/x86_64_backend --mode auto build/manual/program.tac \
    --weights benchmarks/calibration/results/calibrated.conf \
    --dump-ir --trace build/manual/auto.trace > build/manual/auto.s
cat build/manual/auto.trace
g++ -std=c++17 -O2 examples/memory/presentation_driver.cpp \
    build/manual/auto.s -o build/manual/program
objdump -d build/manual/program > build/manual/auto.objdump
./build/manual/program auto
```

Replace `auto` with `scalar`, `simd`, `reuse`, `forward` or `fusion` to force a legal mode.
`--runtime-threshold off` restores the previous unknown-trip profitability policy.
CPU and alias checks are still present. The recorded profile is available in the
repository; use `make calibrate-memory` to remeasure it on this machine.

## 3. Calibration and full experiment

```bash
make calibrate-memory
make evaluate-memory
```

This sequence measures separate training probes, fits coefficients, then evaluates
held-out arithmetic, producer-consumer and stride-2 kernels. It takes several
minutes. Run serially without concurrent builds/tests. Exact profiles, CPU
information, raw samples, median times, code-equivalence groups, prediction
accuracy, gains and losses are retained in:

* `benchmarks/calibration/results/`: measured probes and fitted weights.
* `benchmarks/results-v2/RESULTS.md`: the generated evaluation report.
* `benchmarks/results-v2/{raw,summary,predictions,comparisons}.csv`: raw evidence.
* `build/evaluation-v2/`: generated source/TAC/assembly/traces/disassemblies/libraries.

The calibrated model did not change choices on the recorded evaluation inputs;
there is no measured calibration-driven improvement to claim. Runtime dispatch
has separately measured benefits and overheads. GCC is much faster on some small
and unsupported-stride cases. Explain these results alongside the optimizations,
not as exceptions to omit.

## 4. Dedicated forwarding evidence

```bash
make test-forward
make benchmark-forward
```

The focused suite executes A–E, chains/latest stores, local arrays, pressure stress,
alias layouts, CPU fallback and threshold boundaries. Read
`build/forward-tests/instruction-counts.csv` and the matching `.objdump` files.
`pressure` rejects forwarding at 20 live registers; `costPressure` has a legal
14-register candidate but selects reuse with `pressure=1000` (a test of
configurable model behavior, not a fitted hardware coefficient).

For a manual forwarding trace with both IR stages and disassembly:

```bash
mkdir -p build/forward-demo
./tinyc_part3/tinyc3 examples/memory/forwarding.c targets/x86_64_tinyc.conf \
    build/forward-demo/kernels.tac > build/forward-demo/frontend.log
./backend/x86_64_backend --mode forward build/forward-demo/kernels.tac \
    --dump-ir --trace build/forward-demo/forward.trace > build/forward-demo/forward.s
gcc -shared -fPIC -Wa,-L build/forward-demo/forward.s -o build/forward-demo/forward.so
objdump -d build/forward-demo/forward.so > build/forward-demo/forward.objdump
```

`make test-forward` supplies the executable full-array driver for these kernels;
the library by itself has no `main`. Open [the measured forwarding report](../benchmarks/results-forward/RESULTS.md)
and its raw CSV alongside the demonstration. A shorter separate timing run is:

```bash
python3 benchmarks/forward.py --sizes 8,1024,32768 --samples 5 \
    --output build/forward-quick-results
```

`forward` preserves stores; `fusion` removes an eligible internal temporary.
They have different applicability. Reducing issued loads does not establish
reduced DRAM bandwidth or a universal runtime improvement.
