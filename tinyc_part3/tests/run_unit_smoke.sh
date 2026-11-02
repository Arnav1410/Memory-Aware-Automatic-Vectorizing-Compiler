#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
TEST_OUT="$(mktemp -d)"
trap 'rm -rf "$TEST_OUT"' EXIT
g++ -std=c++11 -Wall -Wextra -Wpedantic -O1 -g -fsanitize=address,undefined \
  -I"$ROOT/../common" -I"$ROOT" \
  "$ROOT/tests/translator_unit_smoke.cpp" \
  "$ROOT/translator.cpp" "$ROOT/../common/target_info.cpp" \
  -o "$TEST_OUT/translator_unit_smoke"
ASAN_OPTIONS="${ASAN_OPTIONS:-detect_leaks=1}" "$TEST_OUT/translator_unit_smoke"
