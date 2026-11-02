# Automated Vectorized Compilers - TinyC Reference + BTP Backend

This project is a self-contained reference implementation (v14) for the TinyC laboratory specifications supplied with the BTP. It is intended for an undergraduate student who does not have the original senior-batch TinyC source code.

## Design goal

The compiler is deliberately written in a simple style:

- ordinary C/C++ structs and classes
- raw pointers
- `std::vector` and `std::string`
- explicit `new` / `delete`
- ordinary loops
- no `unique_ptr`, `shared_ptr`, move semantics, lambdas, or other smart-pointer ownership machinery

The ownership rules are written in comments where a pointer is created, transferred, or destroyed.

## Pipeline

```text
TinyC source
    |
    v
Flex lexer
    |
    v
Bison parser / AST
    |
    v
Semantic translation
    |
    v
Machine-independent 3-address code
    |
    +--------------------+
    |                    |
    v                    v
3AC simulator       vectorization prototype
    |                    |
    v                    v
scalar x86-64       Vector IR
backend             (next BTP stage)
    |
    v
x86-64 assembly
    |
    v
GNU assembler/linker
    |
    v
executable
```

TinyC Part 1 and Part 2 are the front-end laboratory stages. Part 3 creates the machine-independent 3-address representation that is useful as the starting point for the BTP backend.

## Directory layout

```text
common/                  Type and target-size information
targets/                 Target configuration files

tinyc_part1/             Part 1: Flex lexer + token/lexeme table
tinyc_part2/             Part 2: Flex + Bison parser + AST
tinyc_part3/             Part 3: Flex + Bison semantic translator + 3AC

backend/                 3AC loader, simulator, scalar x86-64 backend
vectorization/           Small Vector IR / vectorizer prototype

scripts/                 Build checks, smoke tests, packaging
docs/                    Design and audit notes
```

## Build and test TinyC

Use WSL/Ubuntu (or another environment containing Flex, Bison, GCC/G++, and GNU binutils).

Check the toolchain first:

```bash
bash scripts/toolchain_check.sh
```

To build everything:

```bash
make clean
make build
```

For the first BTP experiment, the shortest complete command sequence is:

```bash
make clean
make tinyc3 backend
make test-btp-scalar
```

If you only want an individual TinyC stage:

```bash
make tinyc1
make tinyc2
make tinyc3
```

### Part 1

```bash
./tinyc_part1/tinyc1 tinyc_part1/test.c
```

The lexer prints the token stream and lexical symbol table.

### Part 2

```bash
./tinyc_part2/tinyc2 tinyc_part2/test.c /tmp/tinyc2_output.txt
cat /tmp/tinyc2_output.txt
```

The output is intentionally an indented parse tree. Expressions become deeply indented because the TinyC grammar has separate nonterminals for multiplicative, additive, shift, relational, equality, bitwise, logical, conditional, and assignment expressions. This is normal.

### Part 3

Part 3 takes a TinyC source file and translates it into machine-independent 3-address code (TAC), together with symbol-table and quad-array information.

Build Part 3:

```bash
make tinyc3
```

Translate the simplest test:

```bash
./tinyc_part3/tinyc3 \
    tinyc_part3/tests/scalar.c \
    targets/x86_64_tinyc.conf \
    tinyc_part3/tests/scalar.tac
```

The three arguments are:

1. TinyC input source file
2. Target configuration file
3. TAC output file

The terminal output contains target information, symbol tables, and the human-readable quad array.

Inspect the plain TAC:

```bash
cat tinyc_part3/tests/scalar.tac
```

For `scalar.c`, it should express the computation:

```text
x = 7
y = 2
t0 = x * y
t1 = t0 + 5
z = t1
return z
```

The exact temporary names are not important.

### Build the BTP backend tools

This is a separate build step. `make tinyc3` builds only the TinyC Part 3 translator; it does **not** build the backend executables.

Run:

```bash
make backend
```

This creates:

```text
backend/ir_simulator
backend/x86_64_backend
```

The source files with `.cpp` remain alongside them:

```text
backend/ir_simulator.cpp
backend/x86_64_backend.cpp
```

After `make clean`, the two executables are deleted, so simply run `make backend` again when needed.

### Test the 3AC simulator

First make sure `scalar.tac` exists as shown above. Then:

```bash
./backend/ir_simulator tinyc_part3/tests/scalar.tac
```

Expected result:

```text
Return value: 19
```

This checks:

```text
TinyC source
    ↓
TinyC Part 3 translator
    ↓
3-address code
    ↓
3AC simulator
    ↓
19
```

### Generate x86-64 assembly

```bash
./backend/x86_64_backend \
    tinyc_part3/tests/scalar.tac \
    targets/x86_64_tinyc.conf \
    > /tmp/tinyc_scalar.s
```

Inspect it:

```bash
cat /tmp/tinyc_scalar.s
```

Then assemble and link it:

```bash
gcc -c /tmp/tinyc_scalar.s -o /tmp/tinyc_scalar.o
gcc /tmp/tinyc_scalar.o -o /tmp/tinyc_scalar
```

Run it and inspect its exit status:

```bash
/tmp/tinyc_scalar
echo $?
```

Expected:

```text
19
```

Thus the first end-to-end BTP backend check is:

```text
TinyC
  ↓
3AC
  ↓
3AC simulator = 19

and

3AC
  ↓
x86-64 assembly
  ↓
assembler/linker
  ↓
executable = 19
```

### One-command scalar backend test

Instead of running the individual commands above, you can run:

```bash
make test-btp-scalar
```

This builds Part 3 and the backend, translates `scalar.c`, prints the TAC, runs the simulator, generates x86-64 assembly, assembles/links it, runs the executable, and checks that the final exit status is 19.

### Translate all Part 3 tests

```bash
bash tinyc_part3/tests/run_tests.sh
```

The resulting TAC files are written under:

```text
tinyc_part3/tests/out/
```

The most important BTP test is:

```text
tinyc_part3/tests/vector_add.c
```

because its TAC contains a loop, induction variable, array indexing, loads, addition, and stores. That TAC is what we will eventually analyze for automatic vectorization.

## Current BTP boundary

At this stage the scalar x86-64 backend is deliberately small. It is a proof-of-concept backend for simple integer TAC; it is **not** yet the complete TinyC backend and it does not yet lower vector IR to AVX/AVX2 instructions.

The intended BTP progression is:

```text
TinyC
  ↓
3AC
  ↓
loop/dependence analysis
  ↓
vector IR
  ↓
AVX/AVX2 instruction selection
  ↓
x86-64 assembly
```

The vectorization directory currently demonstrates the intermediate Vector IR stage only.

## Target-dependent type sizes

Part 3 deliberately does not scatter constants such as `4` and `8` throughout the translator. Type sizes are obtained from `TargetInfo` and are supplied by a target configuration file, for example:

```text
char_size=1
int_size=4
float_size=8
pointer_size=8
stack_alignment=16
```

This follows the spirit of the Part 3 assignment, which notes that hard-coded sizes would not remain machine-independent.
