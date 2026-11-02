#!/usr/bin/env bash
set -u
for tool in gcc g++ flex bison make objdump; do
  if command -v "$tool" >/dev/null 2>&1; then
    printf '%-8s OK\n' "$tool"
  else
    printf '%-8s MISSING\n' "$tool"
  fi
done
