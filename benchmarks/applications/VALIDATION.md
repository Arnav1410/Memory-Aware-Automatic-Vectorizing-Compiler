# Validation and change inventory

The compiler, frontend, optimization passes and cost-model coefficients were not
modified for this evaluation. Twelve source/profile hashes captured before the
work still match; they are retained in `results/compiler-sources.sha256.json`.
Historical benchmark directories and original test cases are preserved.

## Checks completed

| Check | Outcome | Evidence |
|---|---|---|
| Build and original frontend/backend/vectorization tests | PASS | `build/application-audit/build.log`, `regressions.log` |
| Part 3 translation suite and scalar end-to-end pipeline | PASS, repeated after integration | `build/application-audit/final-regressions.log` |
| Original memory suite, including forwarding | PASS: 41,400 executions, 34,560 comparisons | Same final regression log |
| Original runtime suite | PASS: 152 full-array/path checks | Same final regression log |
| New application matrix | PASS: 19,968 executions, 16,896 comparisons, including 768 path checks | `results/correctness.json`, `correctness.csv` |
| Linked vector instruction checks | PASS for all compiled configurations | `results/instructions.csv`, retained disassembly and manifests |
| Timed batches | PASS: every one of 28,288 batches verified after timing | `results/raw.csv` |
| Supplemental GCC stack-probe control | PASS: 306 verified timing samples | `results/stack-probe-control/` |
| Trace-reader regression tests | PASS: 3 tests | `python3 benchmarks/applications/test_metadata.py` |
| Independent data reconciliation | PASS: all samples, 1,664 summaries and 208 predictions | `python3 benchmarks/applications/verify.py --check-binaries` |
| Academic report preview | PASS: seven pages, no undefined references or overfull boxes | `build/application-audit/REPORT_PREVIEW.pdf` and `.log` |

The final trace-reader audit found that an unanchored `selected=` match could
read `fusion_not_selected=mode` as a chosen candidate. The parser now requires
the actual field at the start of a diagnostic line. Three focused tests cover
this case, absorbed consumers and runtime thresholds. Re-reading the original
traces corrected labels/costs in 210 summary rows. No timing, compiled function,
or decision-agreement outcome changed. The integrity checker also compares
saved plan metadata with current traces when `--check-binaries` is requested.

## Files added or extended

- Root `Makefile`: four application check/benchmark/report/control targets.
- Root `README.md`: link to the separate application evaluation.
- Each of `vector_triad/`, `multi_output/`, `two_stage/`, `in_place_forward/`:
  `kernel.c`, `reference.c`, `README.md`.
- `run.py`, `runner.cpp`: full-source compilation, correctness, path checks,
  balanced timing and raw records.
- `native.py`: linked-code regions, memory/arithmetic counts and identity hashes.
- `report.py`, `verify.py`, `test_metadata.py`: derived results, independent data
  checks and parser regressions.
- `stack_probe.py`, `stack_probe.cpp`: isolated GCC stack-probing control.
- `AUDIT.md`, `README.md`, `RESULTS.md`, `REPORT_SECTION.tex`,
  `REPORT_PREVIEW.tex`, this file: suitability, reproduction and academic material.
- `results/`: raw/derived CSVs, environment and hash records, six figure families
  in PDF/SVG/PNG, four representative tables, source/TAC/traces/assembly/
  disassembly and the separate control results.

The reported checks provide empirical coverage, not a formal correctness proof.
The complete methodology, negative results and limits are in [RESULTS.md](RESULTS.md).
