#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
TEST_OUT="$(mktemp -d)"
trap 'rm -rf "$TEST_OUT"' EXIT

make -C "$ROOT/backend"
make -C "$ROOT/vectorization"

printf '%s\n' '--- IR simulator ---'
"$ROOT/backend/ir_simulator" "$ROOT/backend/sample_tac.txt"

printf '%s\n' '--- x86-64 assembly generation ---'
"$ROOT/backend/x86_64_backend" "$ROOT/backend/sample_tac.txt" "$ROOT/targets/x86_64_tinyc.conf" > "$TEST_OUT/scalar.s"

gcc -c "$TEST_OUT/scalar.s" -o "$TEST_OUT/scalar.o"
gcc "$TEST_OUT/scalar.o" -o "$TEST_OUT/scalar"
set +e
"$TEST_OUT/scalar"
rc=$?
set -e
if [[ "$rc" -ne 19 ]]; then
  echo "Unexpected generated executable status: $rc (expected 19)" >&2
  exit 1
fi
printf '%s\n' '--- vectorizer demo ---'
"$ROOT/vectorization/vectorizer_demo"

printf '%s\n' 'Smoke tests completed. TinyC Flex/Bison builds additionally require Flex and Bison.'
