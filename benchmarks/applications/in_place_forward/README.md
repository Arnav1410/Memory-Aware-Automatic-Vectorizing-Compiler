# In Place Forward

A pointwise integer signal-state update writes A, derives C and D from the updated state, then replaces A again. Exercises latest-store forwarding while preserving ordered stores. This is a computational kernel, not a complete image or signal-processing application.

`kernel.c` is accepted directly by TinyC. `reference.c` includes that same source
for GCC. Both use int32 wrapping arithmetic; GCC is compiled with `-fwrapv` and
without `restrict`. Unused pointer parameters keep a uniform six-argument ABI.

Inputs use `std::mt19937` seeds 19, 627 and 2026, with values `int(rng()%201)-100`;
the first three allocation elements are explicitly -100, 0, 100. Complete
allocations and return values are compared, including aliases and guard elements.

From the repository root:

```bash
python3 benchmarks/applications/run.py check --kernel in_place_forward
python3 benchmarks/applications/run.py bench --kernel in_place_forward --output build/app-in_place_forward
python3 benchmarks/applications/report.py --input build/app-in_place_forward
```
