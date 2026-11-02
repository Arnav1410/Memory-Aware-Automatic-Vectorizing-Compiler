# Measured results

Recorded UTC: 2026-10-31T08:52:07.463937+00:00. CPU affinity: [0]. Compiler: gcc (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0.

1512 raw samples; 168 workload/size/mode medians; 28 prediction cases.

The model selected code equivalent to the fastest explicit mode in **25/28** cases. Auto ran within 5% of the fastest explicit mode in **20/28** cases. Identical code can have different medians because of code placement and measurement variability.

Runtimes below are medians in **microseconds per kernel call**, including call/dispatch overhead. All modes passed complete-array validation before and after timing.

| Workload | N | Scalar | SIMD | Reuse | Fusion | Auto | GCC | Auto speedup vs GCC |
|---|---:|---:|---:|---:|---:|---:|---:|---:|
| add | 8 | 0.0211 | 0.0053 | 0.0055 | 0.0055 | 0.0054 | 0.0015 | 0.28x |
| repeated | 8 | 0.0353 | 0.0073 | 0.0078 | 0.0076 | 0.0074 | 0.0025 | 0.33x |
| producer | 8 | 0.0426 | 0.0057 | 0.0052 | 0.0055 | 0.0054 | 0.0013 | 0.23x |
| mixed | 8 | 0.0503 | 0.0079 | 0.0076 | 0.0077 | 0.0081 | 0.0029 | 0.37x |
| add | 1024 | 2.1460 | 0.0943 | 0.0884 | 0.0957 | 0.0924 | 0.0891 | 0.96x |
| repeated | 1024 | 4.0578 | 0.1787 | 0.1441 | 0.1488 | 0.1449 | 0.1677 | 1.16x |
| producer | 1024 | 4.2641 | 0.1596 | 0.1426 | 0.1047 | 0.1080 | 0.1085 | 1.01x |
| mixed | 1024 | 5.5233 | 0.2263 | 0.1480 | 0.1495 | 0.1646 | 0.1566 | 0.95x |
| add | 32768 | 76.9136 | 4.1865 | 4.1672 | 4.0525 | 4.3097 | 4.4185 | 1.03x |
| repeated | 32768 | 128.6210 | 11.5144 | 11.9972 | 11.3034 | 12.1154 | 10.7563 | 0.89x |
| producer | 32768 | 151.1290 | 6.8355 | 6.8153 | 4.0904 | 4.0985 | 6.5711 | 1.60x |
| mixed | 32768 | 176.3170 | 11.3284 | 11.5225 | 11.1565 | 12.0185 | 11.3521 | 0.94x |
| add | 1048576 | 2306.6100 | 331.6970 | 326.3940 | 326.6130 | 363.7600 | 315.9610 | 0.87x |
| repeated | 1048576 | 4450.4100 | 691.1670 | 705.7030 | 706.3450 | 691.0060 | 736.0550 | 1.07x |
| producer | 1048576 | 4943.3000 | 722.0830 | 725.6810 | 355.7120 | 339.8500 | 692.8680 | 2.04x |
| mixed | 1048576 | 6031.5200 | 701.3780 | 672.8440 | 702.0160 | 668.4810 | 636.3100 | 0.95x |

## Prediction disagreements

| Workload | N | Predicted | Observed explicit mode | Auto / fastest runtime |
|---|---:|---|---|---:|
| repeated | 8 | reuse | simd | 1.008 |
| producer | 8 | fusion | reuse | 1.036 |
| repeated | 1048576 | reuse | simd | 1.000 |

## Interpretation

* Repeated-input kernels reduce vector loads from 4 to 2 per eight elements; stores remain 2. The larger mixed expression reduces loads from 6 to 2. These are issued vector loads, not measured DRAM bytes.
* Producer-consumer fusion reduces combined vector loads/stores from 3/2 to 2/1 and removes the temporary accesses on the fast path.
* Basic SIMD, reuse and fusion may emit identical code when the corresponding transformation does not apply. Compare the traces and equivalence column.
* The generated scalar backend spills TAC values. Its speedup is not a pure measure of SIMD efficiency; use the GCC comparison as well.
* No weights were fitted to these results. The model is intentionally approximate and does not always identify the measured winner.
* Static GCC operation counts are unavailable in CSV because memory operands may be folded into arithmetic. Complete disassembly is retained under build/benchmarks.
* Small-array comparisons are dominated by call/guard overhead. Large-array results can vary with cache residency, CPU scheduling and turbo behavior.

See [raw samples](raw.csv), [all medians and counts](summary.csv), [predictions](predictions.csv) and [environment](environment.json).
