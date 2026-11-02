> The integrated scalar/AVX2 backend is available through `x86_64_backend --mode`. See the root README and `docs/MEMORY_AWARE_REPORT.md` for the current pipeline. The instructions below describe the preserved legacy interface.

# Backend tools

This directory contains both **source code** and the **compiled executables**.

Source files:

```text
ir.cpp
ir.hpp
ir_simulator.cpp
x86_64_backend.cpp
```

After running:

```bash
make backend
```

two executable files are created **in this same directory**:

```text
backend/ir_simulator
backend/x86_64_backend
```

There is no `ir_simulator/` directory and no `x86-64/` directory. The `.cpp` files are the programs' source code; the files without `.cpp` are the compiled programs.

`make clean` deliberately removes those two executable files. Therefore, after `make clean`, run:

```bash
make backend
```

before trying to execute them.
