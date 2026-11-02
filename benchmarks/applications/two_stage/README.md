# Two Stage

A two-pass pointwise processing chain first sums two channels, then scales the intermediate. The fixed local temporary does not escape and has no later observable use, allowing restricted fusion. Runtime n must not exceed 1048576. Static experiments specialize its extent to max(1,n).

`kernel.c` is accepted directly by TinyC. `reference.c` includes that same source
for GCC. Both use int32 wrapping arithmetic; GCC is compiled with `-fwrapv` and
without `restrict`. Unused pointer parameters keep a uniform six-argument ABI.

Inputs use `std::mt19937` seeds 19, 627 and 2026, with values `int(rng()%201)-100`;
the first three allocation elements are explicitly -100, 0, 100. Complete
allocations and return values are compared, including aliases and guard elements.

From the repository root:

```bash
python3 benchmarks/applications/run.py check --kernel two_stage
python3 benchmarks/applications/run.py bench --kernel two_stage --output build/app-two_stage
python3 benchmarks/applications/report.py --input build/app-two_stage
```
