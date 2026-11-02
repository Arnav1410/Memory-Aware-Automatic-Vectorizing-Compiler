# Multi Output

Three pointwise derived channels (sum, difference and product) share two input channels. Exercises read-to-read load reuse. Five pointer arguments plus n use all six supported SysV register arguments.

`kernel.c` is accepted directly by TinyC. `reference.c` includes that same source
for GCC. Both use int32 wrapping arithmetic; GCC is compiled with `-fwrapv` and
without `restrict`. Unused pointer parameters keep a uniform six-argument ABI.

Inputs use `std::mt19937` seeds 19, 627 and 2026, with values `int(rng()%201)-100`;
the first three allocation elements are explicitly -100, 0, 100. Complete
allocations and return values are compared, including aliases and guard elements.

From the repository root:

```bash
python3 benchmarks/applications/run.py check --kernel multi_output
python3 benchmarks/applications/run.py bench --kernel multi_output --output build/app-multi_output
python3 benchmarks/applications/report.py --input build/app-multi_output
```
