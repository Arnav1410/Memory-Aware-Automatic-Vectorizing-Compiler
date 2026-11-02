# Baseline audit (2026-10-31)

`make build test test-tinyc1 test-tinyc2 test-p3-suite test-btp-scalar`
passes on the supplied WSL Intel i7-1255U machine. Full baseline output is in
`build/audit/baseline.log`. This workspace has no usable Git repository.

* Part 1 scans; Part 2 builds an AST; Part 3 independently translates with
  Flex/Bison semantic actions. Retain all three stages.
* Part 3 handles integer expressions, branches, for/while/do loops, fixed local
  arrays, pointers and calls. Its grammar requires loop variables declared
  before `for` (no declaration inside the for initializer). These translation
  tests establish parsing/TAC generation, not executable correctness.
* TAC has four tab-separated fields: opcode, arg1, arg2, result. Labels and
  explicit `if<`/`goto` branches represent control flow. Array indexing becomes
  integer byte scaling, pointer addition, `=*` loads and `*=` stores. `=&`
  takes an address. There is no executable parameter/type metadata.
* Existing IR loader drops empty fields and misparses unary loads/address-of;
  fix operand arity while retaining legacy text compatibility.
* Simulator supports scalar arithmetic/branches, uses long long values and
  has no addressable array memory or call stack. Keep it for legacy tests;
  validate complete arrays against independently compiled C for memory kernels.
* Existing x86 backend spills scalars, emits one `main`, ignores function and
  parameter markers, and rejects memory operations. It is useful for legacy
  scalar tests, not an ABI-correct array backend.
* Existing vectorization demo constructs standalone Vector IR; preserve it.

## Minimum extension

Emit backwards-compatible `#@` type/parameter comments from Part 3. Add a typed
TAC reader, shared counted-loop/memory analysis, simple lane-value IR, explicit
load reuse and fusion passes, configurable cost estimator, and a typed x86
emitter selected by `x86_64_backend --mode`. Keep the old invocation intact.
Use SysV integer/pointer parameters and local fixed arrays. Generate scalar TAC
for unsupported loop shapes. Reject unsupported types/opcodes explicitly rather
than silently producing an executable. Pointer parameters require an explicit
checked runtime disjointness condition for SIMD; local arrays are distinct.
Fusion requires a non-escaping local temporary and no use outside the pair.

Preserve all existing targets and add executable full-array tests, negative
legality tests and a benchmark harness with GCC reference kernels.

The extra translator ASan/UBSan smoke script was also attempted. LeakSanitizer
cannot operate under this environment's ptrace sandbox; rerunning its generated
binary with `ASAN_OPTIONS=detect_leaks=0` passed. This is not a leak-check pass.
The original lexer also restricts identifiers to letters followed by letters or
digits; examples retain that grammar rather than changing the frontend language.
