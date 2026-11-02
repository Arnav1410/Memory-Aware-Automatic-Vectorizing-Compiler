# Vector Triad

Integer weighted combination of two channels: C=A+3B. This is a STREAM-triad-style integer adaptation, not the official STREAM benchmark. Exercises contiguous AVX2 arithmetic and tails.

`kernel.c` is accepted directly by TinyC. `reference.c` includes that same source
for GCC. Both use int32 wrapping arithmetic; GCC is compiled with `-fwrapv` and
without `restrict`. Unused pointer parameters keep a uniform six-argument ABI.

Inputs use `std::mt19937` seeds 19, 627 and 2026, with values `int(rng()%201)-100`;
the first three allocation elements are explicitly -100, 0, 100. Complete
allocations and return values are compared, including aliases and guard elements.

From the repository root:

```bash
python3 benchmarks/applications/run.py check --kernel vector_triad
python3 benchmarks/applications/run.py bench --kernel vector_triad --output build/app-vector_triad
python3 benchmarks/applications/report.py --input build/app-vector_triad
```
