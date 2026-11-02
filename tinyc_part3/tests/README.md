# TinyC Part 3 tests

**TinyC identifier note:** the assignment grammar allows letters and digits in identifiers but does not include `_`. Therefore the test programs deliberately use names such as `addOne`, `incValue`, and `vecAdd`, rather than C identifiers containing underscores.

Run from the repository root.

## Translate all test programs

```bash
make tinyc3
bash tinyc_part3/tests/run_tests.sh
```

Each source file is translated to a `.tac` file under `tinyc_part3/tests/out/`.

## First BTP test: scalar.c

Translate it:

```bash
./tinyc_part3/tinyc3 \
    tinyc_part3/tests/scalar.c \
    targets/x86_64_tinyc.conf \
    tinyc_part3/tests/scalar.tac
```

Inspect TAC:

```bash
cat tinyc_part3/tests/scalar.tac
```

Build the backend executables:

```bash
make backend
```

Run the 3AC simulator:

```bash
./backend/ir_simulator tinyc_part3/tests/scalar.tac
```

Expected:

```text
Return value: 19
```

Generate and run x86-64 code:

```bash
./backend/x86_64_backend \
    tinyc_part3/tests/scalar.tac \
    targets/x86_64_tinyc.conf \
    > /tmp/tinyc_scalar.s

gcc -c /tmp/tinyc_scalar.s -o /tmp/tinyc_scalar.o
gcc /tmp/tinyc_scalar.o -o /tmp/tinyc_scalar
/tmp/tinyc_scalar
echo $?
```

The final exit status should be `19`.

Or use one command:

```bash
make test-btp-scalar
```

## BTP vectorization test

`vector_add.c` is the important next test. Its TAC should expose the loop, induction variable, array-index address calculations, loads, addition, and stores. That TAC will later be analyzed by the automatic vectorization pass.
