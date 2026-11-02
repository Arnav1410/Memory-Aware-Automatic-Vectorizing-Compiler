#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"

errors=0

echo "== Tool versions =="
for tool in gcc g++ make flex bison; do
    if command -v "$tool" >/dev/null 2>&1; then
        "$tool" --version | head -n 1
    else
        echo "$tool: NOT FOUND"
        if [[ "$tool" == "flex" || "$tool" == "bison" ]]; then
            echo "  (TinyC front-end build needs this tool)"
        fi
    fi
done

echo
echo "== Simple-code style scan =="
if grep -RInE 'unique_ptr|shared_ptr|make_unique|make_shared|std::move' \
    "$ROOT/tinyc_part2" "$ROOT/tinyc_part3" "$ROOT/common" "$ROOT/backend" "$ROOT/vectorization" \
    --include='*.hpp' --include='*.cpp' --include='*.h' --include='*.cxx' --include='*.y'; then
    errors=1
else
    echo "PASS: compiler source contains no smart-pointer or move-semantics code"
fi

echo
echo "== Bison source declarations =="
if python3 "$ROOT/scripts/check_bison_sources.py"; then
    :
else
    errors=1
fi

if [[ "$errors" -ne 0 ]]; then
    exit 1
fi
