# Automated Vectorized Compilers

A TinyC compiler that generates scalar x86-64 or AVX2 code for supported integer-array loops. It analyzes loop safety, applies memory optimizations, and uses a cost model to choose among available implementations.

**Compilation pipeline**

```text
TinyC source
    → lexer / parser → typed three-address code (TAC)
    → loop, dependence and memory analysis
    → legal scalar and SIMD candidates
    → memory-aware cost estimation and selection
    → x86-64 assembly → native executable
```

The integrated vectorizing backend consumes TAC produced by `tinyc_part3`. Earlier scanner, AST, TAC-simulator and standalone vectorization components are also retained in the repository.

## Requirements

- **Platform:** x86-64 Linux or WSL Ubuntu.
- **Build tools:** GCC/G++ (C++17 support), GNU Make, GNU binutils, Flex, Bison, and Bash.
- **Testing:** Python 3. The compiler and correctness-test scripts do not require third-party Python packages.
- **Plots and report tables:** Python 3 with **Matplotlib and NumPy**, needed for `make report-applications`.
- **Native SIMD benchmarking:** AVX2-capable CPU. Generated code includes a runtime AVX2 check and scalar fallback, but the supplied native benchmarking setup expects AVX2 for its GCC comparison binaries.

All commands below run from the **repository root**. A complete build is:

```bash
make build
```

> `make` without a target builds the backend and standalone vectorization tools; use `make build` to build the TinyC stages as well.

## Quick start

Build and run the main correctness tests:

```bash
make build
make test                 # existing frontend/backend/vectorization tests
make test-p3-suite        # TinyC to TAC translation tests
make test-btp-scalar      # scalar TAC and native-code pipeline
make test-memory          # loop/memory analysis and full-array correctness
make test-runtime         # runtime guards, thresholds and fallbacks
make test-applications    # four application-oriented kernels
```

The tests use generated inputs, fixed random seeds, boundary sizes and selected aliasing cases. Results are checked against GCC-generated reference implementations. Full-array comparisons check the output, rather than only checking a small sample of elements. Test scripts and input generators are included in the repository.

### Compile and inspect a TinyC kernel

```bash
mkdir -p build/demo

# Generate typed TAC from TinyC.
./tinyc_part3/tinyc3 examples/memory/kernels.c \
    targets/x86_64_tinyc.conf build/demo/kernels.tac

# Select optimizations and generate x86-64 assembly.
./backend/x86_64_backend --mode auto build/demo/kernels.tac \
    --trace build/demo/auto.trace > build/demo/kernels.s

# Assemble and link the kernel functions into a shared library.
gcc -shared -fPIC build/demo/kernels.s -o build/demo/kernels.so

# Inspect decisions and generated machine code.
cat build/demo/auto.trace
objdump -d build/demo/kernels.so
```

Kernel functions follow the x86-64 System V calling convention and can be called from C. To compile a source program that defines `main`, link its generated assembly into an executable using `gcc program.s -o program`.

## Optimization modes

| Mode | Behavior |
|---|---|
| `scalar` | Generate scalar x86-64 code for supported TAC operations. |
| `simd` | Vectorize supported loops with eight 32-bit integer elements per AVX2 vector; handle remaining elements with scalar code. |
| `reuse` | Reuse previously loaded vector values when intervening writes cannot invalidate them. |
| `forward` | Additionally forward vector values from earlier stores to compatible later reads. |
| `fusion` | Combine eligible adjacent producer-consumer loops, avoiding unnecessary temporary-array accesses. |
| `auto` | Use the memory-aware cost model to select a legal implementation, including eligible fusion and forwarding. |

The cost model considers estimated instruction and memory costs, loop length, register pressure, guards and other overheads. It makes an estimated choice; it does not guarantee that the selected implementation is the fastest on every input.

To inspect its assumptions and decisions:

```bash
./backend/x86_64_backend --mode auto build/demo/kernels.tac \
    --weights benchmarks/default.conf --assume-trip 1024 \
    --alias-policy check --dump-ir \
    --trace build/demo/model.trace > build/demo/model.s
```

- `--assume-trip` provides an estimate when a loop bound is not known at compile time; it is **not** an exact runtime bound.
- With `--alias-policy check` (the default), runtime checks fall back to scalar execution when pointer ranges overlap in ways that prevent safe vectorization. `--alias-policy reject` conservatively rejects vectorization when disjointness cannot be established.
- For eligible non-fused loops, `auto` can use a runtime trip-count threshold to avoid vectorization on small inputs. This can be disabled with `--runtime-threshold off`.
- If AVX2 or the required operating-system state is unavailable at runtime, generated SIMD paths fall back to scalar code.

## Application benchmarks

Four integer-array kernels are compiled through the **TinyC → TAC → native-code** pipeline and checked against equivalent GCC reference code. These are computational kernels, not full applications or the official STREAM benchmark.

| Kernel | Computation | Main optimization | Recorded result at `N = 1024` |
|---|---|---|---|
| [Integer vector triad](benchmarks/applications/vector_triad/kernel.c) | `C[i] = A[i] + 3*B[i]` | Basic AVX2 | **25.18×** vs the compiler's scalar backend |
| [Multi-output transform](benchmarks/applications/multi_output/kernel.c) | Compute sum, difference and product from the same inputs | Read reuse | **1.33×** vs basic SIMD |
| [Two-stage transform](benchmarks/applications/two_stage/kernel.c) | Produce an intermediate array and then scale it | Loop fusion | **1.35×** vs unfused reuse |
| [In-place transform](benchmarks/applications/in_place_forward/kernel.c) | Update an array and compute values from the updates | Store-to-load forwarding | **1.24×** vs reuse |

**Measurement context:** Intel Core i7-1255U under WSL Ubuntu, static loop bounds, unaligned arrays, `N = 1024`. These measurements are hardware- and workload-dependent. The 25.18× scalar-to-SIMD comparison includes overhead from the project's scalar code generator and is **not** a pure AVX2-width speedup. Forwarding also showed slowdowns at larger input sizes.

To reproduce the evaluation:

```bash
make test-applications
make benchmark-applications
make report-applications
```

The benchmark driver compares the six compiler modes with GCC `-O3 -march=native` and a GCC non-vectorized reference. It records multiple timing samples, summarizes runtimes and checks full outputs for correctness. **Run benchmarks without concurrent builds or tests**, as competing workloads can affect timings. Running the commands above may regenerate tracked files under `benchmarks/applications/results/`.

For a shorter experiment without overwriting the recorded results:

```bash
python3 benchmarks/applications/run.py bench \
    --sizes 8,1024,262144 --dynamic-sizes 1024 \
    --samples 17 --output build/application-short-run
python3 benchmarks/applications/report.py \
    --input build/application-short-run
```

See the [application benchmark guide](benchmarks/applications/README.md) for the complete procedure, and the [measured results](benchmarks/applications/RESULTS.md) for timings, instruction counts, cost-model decisions, plots, negative results and limitations.

## Other experiments

| Command | Purpose |
|---|---|
| `make present-memory` | Generate a concise source-to-native walkthrough across compiler modes. |
| `make demo-memory` | Run the larger combined test and benchmark workflow. |
| `make benchmark-memory` | Benchmark supported memory-aware optimization modes. |
| `make benchmark-forward` | Measure store-to-load forwarding, including cases where it slows execution. |
| `make calibrate-memory` | Measure training probes and generate an optional machine-specific cost profile. |
| `make evaluate-memory` | Compare analytical and calibrated weights and runtime selection. |
| `make test-source-archive` | Check creation and contents of a source-only archive. |

The default weights in `benchmarks/default.conf` are analytical estimates, not measurements fitted to a processor. Calibration produces an optional profile that can be selected with `--weights benchmarks/calibration/results/calibrated.conf`. In the recorded calibration evaluation, the fitted profile **did not change the chosen candidates**; calibration therefore has not been shown to improve selection quality. Improving the model and validating it on more discriminating workloads are open directions.

For a smaller run of the original memory benchmarks, saving output outside the recorded results:

```bash
python3 benchmarks/run.py --sizes 8,1024,32768 \
    --samples 5 --output build/quick-results
```

## Supported language and limitations

- SIMD supports simple counted loops with an increasing-by-one induction variable, contiguous `int32` accesses such as `A[i]`, straight-line bodies, and supported integer `+`, `-` and `*` operations. Equivalent canonical `while` loops also work.
- Other loop forms, dependencies, strides, conditions, division and supported calls use the scalar backend where possible; the system is **not** a general-purpose C or SIMD compiler.
- Fusion is restricted to qualifying adjacent loops sharing an iteration domain, with a non-escaping fixed-size local temporary array. General loop fusion is not implemented.
- Supported functions have up to six integer/pointer parameters and `int`/`void` returns; fixed one-dimensional local integer arrays are supported. Unsupported types, globals or opcodes can produce compile-time errors.
- TinyC is a restricted C-like language: declarations before `for`, no preprocessor, variable-length arrays or `restrict` syntax.
- The arithmetic contract uses wrapping 32-bit signed `+`, `-` and `*`, compared with GCC references compiled using `-fwrapv`. Callers must still provide valid pointers, sufficient array bounds and initialized data.
- The project uses conservative legality checks. Passing extensive tests supports correctness within the tested subset but is not a formal proof for arbitrary input programs.
- Static vector-instruction counts do not directly measure cache misses, memory bandwidth or DRAM traffic. Cost estimates and measured performance can disagree.

## Repository documentation

- [Architecture and compiler pipeline](docs/ARCHITECTURE.md)
- [Memory-aware optimization design and limitations](docs/MEMORY_AWARE_REPORT.md)
- [Build and usage details](docs/BACKEND_USAGE.md)
- [Step-by-step compilation walkthrough](docs/DEMONSTRATION.md)
- [Store-to-load forwarding](docs/STORE_FORWARDING.md)
- [Verification, calibration and runtime dispatch](docs/VERIFICATION_AND_CALIBRATION.md)
- [Application benchmark methodology and reproduction](benchmarks/applications/README.md)
- [Application benchmark results](benchmarks/applications/RESULTS.md)
- [Original project commands](docs/LEGACY_README.md)

Generated objects, executables and temporary benchmark files belong under `build/` or other ignored output locations. Keep source code, test scripts and representative result files in version control so the experiments remain reproducible.
