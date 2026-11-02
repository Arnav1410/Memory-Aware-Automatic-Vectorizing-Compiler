#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"

make -C "$ROOT/backend"

[[ -x "$ROOT/backend/ir_simulator" ]] || { echo "backend/ir_simulator was not built" >&2; exit 1; }
[[ -x "$ROOT/backend/x86_64_backend" ]] || { echo "backend/x86_64_backend was not built" >&2; exit 1; }

echo "Backend executables are present:"
echo "  $ROOT/backend/ir_simulator"
echo "  $ROOT/backend/x86_64_backend"
