# BTP testing order

From the repository root:

```bash
make clean
make tinyc1
make tinyc2
make tinyc3
make backend
```

Part 3 -> TAC:

```bash
./tinyc_part3/tinyc3 \
  tinyc_part3/tests/scalar.c \
  targets/x86_64_tinyc.conf \
  tinyc_part3/tests/scalar.tac
```

TAC simulator:

```bash
./backend/ir_simulator tinyc_part3/tests/scalar.tac
```

Expected:

```text
Return value: 19
```

x86-64 generation:

```bash
./backend/x86_64_backend \
  tinyc_part3/tests/scalar.tac \
  targets/x86_64_tinyc.conf > /tmp/tinyc_scalar.s

gcc -c /tmp/tinyc_scalar.s -o /tmp/tinyc_scalar.o
gcc /tmp/tinyc_scalar.o -o /tmp/tinyc_scalar
/tmp/tinyc_scalar
echo $?
```

Expected exit status: `19`.

Part 3 translation suite:

```bash
bash tinyc_part3/tests/run_tests.sh
```

The suite continues after an individual translation failure and reports all failures at the end.
