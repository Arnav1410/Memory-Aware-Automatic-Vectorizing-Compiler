# Evaluation suitability audit

All four preferred computations are supported without direct-TAC input or compiler
changes. Source functions use declarations before `for`, camel-case identifiers,
fixed local arrays, six or fewer register arguments, `int` addition/subtraction/
multiplication, and `i=0; i<n; i=i+1`. TinyC does not implement full C, preprocessing,
VLA temporaries, `restrict`, arbitrary strided SIMD or general loop fusion.

The analyzer proves exact `base+4*i` int32 accesses, rejects unsupported side
effects and dependencies, and preserves complete scalar TAC. Local stack arrays
are disjoint from unchanged incoming pointer parameters. Pointer names are not
assumed disjoint: the vector entry checks whole accessed ranges. Every benchmark
keeps those guards. Reference C is compiled without `restrict` and with `-fwrapv`.

`multiOutput` has five pointers plus `n`, exactly the six-parameter ABI limit.
Unused pointer parameters give other kernels the same native-driver interface.
`twoStage` has one internal fixed temporary, identical iteration domains and no
later use/escape. The temporary is not an output; all caller-visible arrays are
checked. The runtime-bound source reserves 1048576 elements (about 4 MiB of stack).
Static-bound experiments replace only `i<n` and the fixed temporary extent with
known constants; GCC receives the identical specialized source.

Representative N=1024 linked code confirms the intended transformations:

| Kernel | Basic loads/stores | Reuse | Forward | Fusion |
|---|---:|---:|---:|---:|
| Integer vector triad | 2/1 | 2/1 | 2/1 | 2/1 (no fusion opportunity) |
| Multi-output | 6/3 | 2/3 | 2/3 | 2/3 (reuse only) |
| Two-stage | 3/2 across two loops | 3/2 | 3/2 | 2/1 in one loop |
| In-place derived outputs | 6/4 | 4/4 | 2/4 | 4/4 (reuse only) |

The compiler's cost model, weights, candidate generator and emitter are unchanged.
The default analytical profile is snapshotted in the results. Literal-trip and
unknown-trip experiments are reported separately. Unknown-trip costs use the
existing assumed count of 1024, with actual runtime dispatch for non-fused auto
plans. The existing fused candidate has a different scalar tail and no runtime
profitability threshold; this is retained and evaluated rather than changed.

Existing tests and historical benchmark outputs are preserved. New driver logic
handles five arrays and mutable inputs; the older four-array runners cannot be
used unchanged. Matplotlib is already available and is used only for figures.
No additional compiler dependencies or handwritten SIMD kernels are introduced.
