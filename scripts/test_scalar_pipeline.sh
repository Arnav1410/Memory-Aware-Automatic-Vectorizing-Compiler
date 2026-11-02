#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
SRC="$ROOT/tinyc_part3/tests/scalar.c"
CFG="$ROOT/targets/x86_64_tinyc.conf"
OUTDIR="$ROOT/tinyc_part3/tests/out"
TAC="$OUTDIR/scalar.tac"
FULL="$OUTDIR/scalar.full.out"
TEST_OUT="$(mktemp -d)"
trap 'rm -rf "$TEST_OUT"' EXIT
ASM="$TEST_OUT/scalar.s"
OBJ="$TEST_OUT/scalar.o"
BIN="$TEST_OUT/scalar"

mkdir -p "$OUTDIR"

# tinyc3 and backend are expected to have been built by the Makefile target.
"$ROOT/tinyc_part3/tinyc3" "$SRC" "$CFG" "$TAC" > "$FULL"

printf '%s\n' '=== Generated TAC ==='
cat "$TAC"
printf '%s\n' ''

printf '%s\n' '=== 3AC simulator ==='
"$ROOT/backend/ir_simulator" "$TAC"
printf '%s\n' ''

printf '%s\n' '=== x86-64 assembly ==='
"$ROOT/backend/x86_64_backend" "$TAC" "$CFG" > "$ASM"
sed -n '1,220p' "$ASM"
printf '%s\n' ''

printf '%s\n' '=== Assemble + link + run ==='
gcc -c "$ASM" -o "$OBJ"
gcc "$OBJ" -o "$BIN"
set +e
"$BIN"
rc=$?
set -e
printf 'Program exit status: %s\n' "$rc"

if [[ "$rc" -ne 19 ]]; then
    echo 'ERROR: expected scalar.c to return 19.' >&2
    exit 1
fi

echo 'Scalar end-to-end pipeline: PASS'
