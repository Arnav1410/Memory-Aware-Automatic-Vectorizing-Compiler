# Calibrated-model evaluation

Recorded: 2026-10-31 15:53:11 IST; CPU affinity [0]; gcc (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0. Raw timestamps use UTC.
11610 raw samples; 1290 medians. Raw timing and prediction datasets are linked below.

## Prediction accuracy

Accuracy compares the selected implementation with the fastest explicit code group, pooling samples of identical assembly. Runtime dispatch is a distinct implementation; its chosen branch is reported separately from exact code identity.

| Domain | Model | Agreement | Runtime within 5% of fastest group |
|---|---|---:|---:|
| static | auto-original | 63/70 | 51/70 |
| static | auto-calibrated | 63/70 | 58/70 |
| dynamic | auto-original | 70/100 | 67/100 |
| dynamic | auto-calibrated | 70/100 | 63/100 |
| dynamic | auto-threshold | 93/100 | 70/100 |

## What changed in emitted code

| Comparison | Cases | Different code | Different-code cases with CI entirely >1.05 | Different-code cases with CI entirely <0.95 |
|---|---:|---:|---:|---:|
| auto-original → auto-calibrated | 170 | 0 | 0 | 0 |
| auto-calibrated → auto-threshold | 100 | 60 | 10 | 0 |

Paired speedup is before/after runtime. Bootstrap intervals use nine paired rounds and are exploratory; they do not correct for multiple comparisons. Differences between identical-code models are timing/layout variability, not evidence of model improvement.

## Representative unaligned static timings

Median microseconds per call. Full results include both alignments, all sizes and scalar fallback for stride-2 accesses.

| Workload | N | Scalar | SIMD | Reuse | Fusion | Original auto | Calibrated auto | GCC |
|---|---:|---:|---:|---:|---:|---:|---:|---:|
| add | 8 | 0.0220 | 0.0053 | 0.0056 | 0.0058 | 0.0060 | 0.0057 | 0.0012 |
| repeated | 8 | 0.0366 | 0.0077 | 0.0075 | 0.0077 | 0.0075 | 0.0077 | 0.0022 |
| producer | 8 | 0.0400 | 0.0175 | 0.0175 | 0.0060 | 0.0060 | 0.0057 | 0.0013 |
| mixed | 8 | 0.0508 | 0.0083 | 0.0081 | 0.0088 | 0.0082 | 0.0086 | 0.0028 |
| strided | 8 | 0.0227 | 0.0210 | 0.0209 | 0.0211 | 0.0225 | 0.0212 | 0.0018 |
| add | 1024 | 2.1340 | 0.0867 | 0.0878 | 0.0869 | 0.0844 | 0.0902 | 0.0750 |
| repeated | 1024 | 3.8278 | 0.1601 | 0.1336 | 0.1451 | 0.1319 | 0.1336 | 0.1456 |
| producer | 1024 | 4.1388 | 0.1529 | 0.1445 | 0.1040 | 0.1014 | 0.0968 | 0.1057 |
| mixed | 1024 | 5.7630 | 0.2064 | 0.1367 | 0.1431 | 0.1519 | 0.1445 | 0.1458 |
| strided | 1024 | 2.3142 | 2.4484 | 2.4091 | 2.2352 | 2.3608 | 2.4457 | 0.1032 |
| add | 32768 | 78.0903 | 4.3700 | 4.2952 | 4.3184 | 4.4283 | 4.3492 | 4.2871 |
| repeated | 32768 | 135.6293 | 12.1847 | 12.9467 | 12.8360 | 13.2481 | 13.1331 | 12.0792 |
| producer | 32768 | 152.4636 | 6.9420 | 6.9051 | 3.8745 | 4.1522 | 3.9751 | 6.6919 |
| mixed | 32768 | 191.5224 | 13.1776 | 12.7534 | 13.0165 | 13.0772 | 12.8680 | 13.7463 |
| strided | 32768 | 89.6952 | 86.4000 | 86.6625 | 82.7564 | 85.4802 | 89.0562 | 5.0579 |
| add | 1048576 | 2657.9533 | 327.1354 | 317.6070 | 314.0660 | 328.5921 | 315.6611 | 305.6106 |
| repeated | 1048576 | 4372.6525 | 608.1467 | 624.6576 | 618.8688 | 619.1382 | 628.5605 | 621.5305 |
| producer | 1048576 | 5404.9390 | 741.7016 | 754.0628 | 332.1585 | 345.8317 | 346.7802 | 625.5072 |
| mixed | 1048576 | 6159.1660 | 688.6527 | 664.6979 | 643.8433 | 661.8336 | 671.7494 | 663.2283 |
| strided | 1048576 | 3048.2920 | 3135.2787 | 2852.9755 | 2879.9307 | 3139.7957 | 2901.6973 | 555.5630 |

## Runtime-threshold measurements

First function (`add`) keeps the same function entry offset across variants, reducing code-placement confounding. The check is a cost-derived batch threshold, not a fitted lookup table.

| Alignment | N | Off (ns) | On (ns) | Paired speedup | Bootstrap interval |
|---|---:|---:|---:|---:|---:|
| aligned | 0 | 4.295 | 4.159 | 1.043 | [0.857, 1.121] |
| unaligned | 0 | 4.117 | 4.047 | 0.977 | [0.657, 1.137] |
| aligned | 1 | 7.027 | 5.225 | 1.409 | [1.190, 1.824] |
| unaligned | 1 | 7.946 | 5.040 | 1.627 | [1.359, 1.978] |
| aligned | 3 | 14.174 | 15.097 | 1.081 | [0.939, 1.612] |
| unaligned | 3 | 12.811 | 9.377 | 1.361 | [1.255, 1.540] |
| aligned | 7 | 23.700 | 19.607 | 1.222 | [1.064, 1.939] |
| unaligned | 7 | 20.567 | 19.205 | 1.093 | [0.987, 1.188] |
| aligned | 8 | 5.697 | 5.297 | 1.061 | [1.010, 1.155] |
| unaligned | 8 | 5.584 | 5.305 | 1.057 | [0.936, 1.151] |
| aligned | 9 | 7.178 | 7.508 | 0.956 | [0.873, 1.673] |
| unaligned | 9 | 7.112 | 7.769 | 0.952 | [0.790, 1.018] |
| aligned | 17 | 8.138 | 7.380 | 1.076 | [0.983, 1.147] |
| unaligned | 17 | 7.963 | 7.708 | 1.011 | [0.886, 1.156] |
| aligned | 64 | 9.147 | 10.297 | 0.914 | [0.796, 0.966] |
| unaligned | 64 | 9.814 | 10.656 | 0.954 | [0.873, 1.030] |
| aligned | 1024 | 73.434 | 77.093 | 0.989 | [0.919, 1.062] |
| unaligned | 1024 | 92.252 | 90.786 | 1.001 | [0.552, 1.134] |
| aligned | 32768 | 3159.278 | 3135.882 | 0.996 | [0.900, 1.035] |
| unaligned | 32768 | 4025.097 | 3903.357 | 1.014 | [0.851, 1.197] |

## Limitations

* Calibration and evaluation source kernels are separate. Fitted weights are effective nanoseconds under this pinning/frequency state, not universal instruction latencies or measured DRAM byte counts.
* Single logical CPU on WSL, warm buffers, no hardware-counter attribution. Alignment, code layout, thermal drift and turbo can affect rankings.
* Runtime dispatch is implemented for non-fused loops. Fusion keeps its prior policy because its scalar tail is a different transformed computation.
* The static/dynamic benchmark binaries and traces are under `build/evaluation-v2`. Original historical results remain under `benchmarks/results`.
* This is an exploratory experiment. Do not infer broad superiority over GCC or over the original model from isolated timing rows.

[Raw samples](raw.csv) · [Medians](summary.csv) · [Predictions](predictions.csv) · [Paired comparisons](comparisons.csv) · [Environment and exact weights](environment.json)
