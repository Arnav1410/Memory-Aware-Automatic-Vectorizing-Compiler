#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
OUT="${1:-$ROOT/../BTP_AutomatedVectorizedCompilers_v11_simple.tar.gz}"
mkdir -p "$(dirname "$OUT")"
rm -f "$OUT"
cd "$ROOT/.."
NAME="$(basename "$ROOT")"
tar --exclude="$NAME/.git" \
    --exclude="$NAME/.agents" \
    --exclude="$NAME/.codex" \
    --exclude="$NAME/.aws" \
    --exclude="$NAME/build" \
    --exclude='*/__pycache__' \
    --exclude="$NAME/*.log" \
    --exclude="$NAME/**/lex.yy.c" \
    --exclude="$NAME/**/tinyc2.tab.c" \
    --exclude="$NAME/**/tinyc2.tab.h" \
    --exclude="$NAME/**/tinyc3.tab.c" \
    --exclude="$NAME/**/tinyc3.tab.h" \
    --exclude="$NAME/**/*.o" \
    --exclude="$NAME/backend/ir_simulator" \
    --exclude="$NAME/backend/x86_64_backend" \
    --exclude="$NAME/vectorization/vectorizer_demo" \
    --exclude="$NAME/tinyc_part1/tinyc1" \
    --exclude="$NAME/tinyc_part2/tinyc2" \
    --exclude="$NAME/tinyc_part3/tinyc3" \
    -czf "$OUT" "$NAME"
printf 'Created %s\n' "$OUT"
