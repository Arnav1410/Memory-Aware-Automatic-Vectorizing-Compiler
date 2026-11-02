> The integrated scalar/AVX2 backend is available through `x86_64_backend --mode`. See the root README and `docs/MEMORY_AWARE_REPORT.md` for the current pipeline. The instructions below describe the preserved legacy interface.

# Backend usage

The backend source files are:

```text
backend/ir_simulator.cpp
backend/x86_64_backend.cpp
```

They become executable programs only after:

```bash
make backend
```

which creates:

```text
backend/ir_simulator
backend/x86_64_backend
```

`make clean` removes these executables by design.

The safest end-to-end command for the first BTP milestone is:

```bash
make test-btp-scalar
```

This performs:

```text
scalar.c
  -> tinyc3
  -> scalar.tac
  -> ir_simulator
  -> x86_64_backend
  -> GNU assembler/linker
  -> executable
```

For the provided scalar test, both execution paths should agree on return value 19.
