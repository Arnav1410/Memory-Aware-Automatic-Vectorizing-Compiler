# Files changed for store-to-load forwarding

Compared with the snapshot taken immediately before this incremental task. Earlier
verification/calibration work was preserved. Build outputs are excluded; generated
compiler binaries were rebuilt from source. There is no usable Git metadata in this workspace.

| File | Change |
|---|---|
| [Makefile](../Makefile) | Modified |
| [README.md](../README.md) | Modified |
| [backend/memory/analysis.cpp](../backend/memory/analysis.cpp) | Modified |
| [backend/memory/compiler.hpp](../backend/memory/compiler.hpp) | Modified |
| [backend/memory/cost.cpp](../backend/memory/cost.cpp) | Modified |
| [backend/memory/emitter.cpp](../backend/memory/emitter.cpp) | Modified |
| [benchmarks/evaluate.py](../benchmarks/evaluate.py) | Modified |
| [benchmarks/forward.py](../benchmarks/forward.py) | Added |
| [benchmarks/forward_runner.cpp](../benchmarks/forward_runner.cpp) | Added |
| [benchmarks/results-forward/RESULTS.md](../benchmarks/results-forward/RESULTS.md) | Added |
| [benchmarks/results-forward/comparisons.csv](../benchmarks/results-forward/comparisons.csv) | Added |
| [benchmarks/results-forward/environment.json](../benchmarks/results-forward/environment.json) | Added |
| [benchmarks/results-forward/instruction-counts.csv](../benchmarks/results-forward/instruction-counts.csv) | Added |
| [benchmarks/results-forward/predictions.csv](../benchmarks/results-forward/predictions.csv) | Added |
| [benchmarks/results-forward/raw.csv](../benchmarks/results-forward/raw.csv) | Added |
| [benchmarks/results-forward/summary.csv](../benchmarks/results-forward/summary.csv) | Added |
| [benchmarks/run.py](../benchmarks/run.py) | Modified |
| [benchmarks/runner.cpp](../benchmarks/runner.cpp) | Modified |
| [benchmarks/summarize.py](../benchmarks/summarize.py) | Modified |
| [docs/FORWARDING_CHANGES.md](FORWARDING_CHANGES.md) | Added (this inventory) |
| [docs/DEMONSTRATION.md](../docs/DEMONSTRATION.md) | Modified |
| [docs/MEMORY_AWARE_REPORT.md](../docs/MEMORY_AWARE_REPORT.md) | Modified |
| [docs/STORE_FORWARDING.md](../docs/STORE_FORWARDING.md) | Added |
| [docs/VERIFICATION_AND_CALIBRATION.md](../docs/VERIFICATION_AND_CALIBRATION.md) | Modified |
| [examples/memory/forwarding.c](../examples/memory/forwarding.c) | Added |
| [examples/memory/presentation.c](../examples/memory/presentation.c) | Modified |
| [examples/memory/presentation_driver.cpp](../examples/memory/presentation_driver.cpp) | Modified |
| [scripts/present_memory.py](../scripts/present_memory.py) | Modified |
| [tests/memory/forward.py](../tests/memory/forward.py) | Added |
| [tests/memory/forward_unit.hpp](../tests/memory/forward_unit.hpp) | Added |
| [tests/memory/native_counts.py](../tests/memory/native_counts.py) | Added |
| [tests/memory/run.py](../tests/memory/run.py) | Modified |
| [tests/memory/unit.cpp](../tests/memory/unit.cpp) | Modified |

The clean rebuild also regenerated frontend binaries and Flex/Bison files. Their
source inputs were not changed for forwarding. Logs, test fixtures, native binaries
and disassemblies are under `build/forward-audit`, `build/forward-tests`,
`build/presentation` and `build/forward-benchmarks`. Raw measured datasets are
listed above and retained outside the build directory.
