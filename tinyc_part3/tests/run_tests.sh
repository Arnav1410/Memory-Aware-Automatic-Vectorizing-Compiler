#!/usr/bin/env bash
set -u
ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
OUT="$ROOT/tinyc_part3/tests/out"
mkdir -p "$OUT"

if ! make -C "$ROOT/tinyc_part3" >/dev/null; then
    echo "ERROR: tinyc3 did not build."
    exit 1
fi

failures=0
for src in "$ROOT"/tinyc_part3/tests/*.c; do
    base="$(basename "$src" .c)"
    out="$OUT/$base.tac"
    full="$OUT/$base.full.out"
    echo "=== $base ==="
    if "$ROOT/tinyc_part3/tinyc3" "$src" "$ROOT/targets/x86_64_tinyc.conf" "$out" > "$full" 2>&1; then
        echo "PASS: translation succeeded"
        echo "TAC: $out"
        cat "$out"
    else
        echo "FAIL: translation failed"
        cat "$full"
        failures=$((failures + 1))
    fi
    echo

done

if [ "$failures" -ne 0 ]; then
    echo "TinyC Part 3 tests: $failures failure(s)"
    exit 1
fi

echo "All TinyC Part 3 translation tests passed."
