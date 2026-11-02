# Store-to-load forwarding measurements

Recorded UTC: 2026-11-01T05:52:25.483774+00:00; affinity [0]; gcc (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0.
6174 raw samples; 686 medians; 98 prediction cases. All timed outputs passed repeated-call, full-allocation GCC checks.

Auto selected the fastest equivalent explicit code group in **83/98** cases; auto runtime was within 5% in **81/98**.
Reuse/forward code differs in 56/98 cases. Of those, paired exploratory 95% bootstrap intervals lie entirely above 1.05x in 8 and below 0.95x in 0 cases. No multiple-comparison correction; nine rounds are not a broad performance guarantee.
Only 40 changed-code cases execute a vector batch (N >= 8); 8 have intervals entirely above 1.05x, 0 entirely below 0.95x. At N < 8 the transformed vector body is not executed.

Median microseconds per call, unaligned buffers. Forward/reuse speedup is reuse time / forward time; below one means slower.

| Workload | N | Scalar | SIMD | Reuse | Forward | Fusion | Auto | GCC | Forward/reuse |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| basicForward | 8 | 0.0316 | 0.0059 | 0.0059 | 0.0060 | 0.0058 | 0.0060 | 0.0023 | 0.989x |
| multipleReads | 8 | 0.0494 | 0.0083 | 0.0082 | 0.0081 | 0.0080 | 0.0082 | 0.0030 | 1.012x |
| multipleStores | 8 | 0.0429 | 0.0060 | 0.0061 | 0.0060 | 0.0061 | 0.0060 | 0.0023 | 1.010x |
| longChain | 8 | 0.0649 | 0.0084 | 0.0089 | 0.0082 | 0.0087 | 0.0086 | 0.0034 | 1.086x |
| repeated | 8 | 0.0353 | 0.0085 | 0.0080 | 0.0079 | 0.0078 | 0.0079 | 0.0027 | 1.014x |
| add | 8 | 0.0230 | 0.0057 | 0.0055 | 0.0054 | 0.0055 | 0.0054 | 0.0017 | 1.016x |
| producer | 8 | 0.0448 | 0.0058 | 0.0054 | 0.0056 | 0.0056 | 0.0059 | 0.0015 | 0.971x |
| basicForward | 1024 | 3.3815 | 0.1665 | 0.1574 | 0.1329 | 0.1588 | 0.1318 | 0.1224 | 1.184x |
| multipleReads | 1024 | 5.7169 | 0.2154 | 0.1913 | 0.1975 | 0.1981 | 0.1735 | 0.1971 | 0.969x |
| multipleStores | 1024 | 4.9312 | 0.2104 | 0.2037 | 0.1491 | 0.2093 | 0.1508 | 0.1122 | 1.366x |
| longChain | 1024 | 9.1577 | 0.3792 | 0.3316 | 0.2306 | 0.3290 | 0.2450 | 0.2294 | 1.438x |
| repeated | 1024 | 3.9302 | 0.1612 | 0.1292 | 0.1346 | 0.1293 | 0.1323 | 0.1431 | 0.960x |
| add | 1024 | 2.1649 | 0.0802 | 0.0827 | 0.0815 | 0.0816 | 0.0875 | 0.0737 | 1.014x |
| producer | 1024 | 4.1813 | 0.1423 | 0.1419 | 0.1408 | 0.0927 | 0.0964 | 0.0852 | 1.008x |
| basicForward | 32768 | 116.3801 | 8.5985 | 10.2687 | 8.2204 | 8.9350 | 8.2923 | 7.7118 | 1.249x |
| multipleReads | 32768 | 199.4281 | 17.3730 | 14.8684 | 15.5849 | 15.0885 | 15.1269 | 15.1963 | 0.954x |
| multipleStores | 32768 | 169.0522 | 10.5753 | 10.5645 | 11.0323 | 10.6093 | 9.6274 | 8.0880 | 0.958x |
| longChain | 32768 | 267.6213 | 15.6334 | 14.2301 | 17.5472 | 14.3340 | 17.4233 | 13.6678 | 0.811x |
| repeated | 32768 | 154.0648 | 14.0523 | 13.9136 | 13.6386 | 13.9734 | 13.8306 | 13.8919 | 1.020x |
| add | 32768 | 77.3711 | 3.6865 | 3.6836 | 3.6768 | 3.7345 | 3.6584 | 3.8808 | 1.002x |
| producer | 32768 | 149.1517 | 6.4175 | 6.7211 | 6.7897 | 3.9721 | 4.0206 | 6.0034 | 0.990x |
| basicForward | 1048576 | 4170.4123 | 667.5145 | 549.0657 | 611.6450 | 643.8773 | 512.2382 | 520.5693 | 0.898x |
| multipleReads | 1048576 | 6705.6760 | 944.8795 | 932.8765 | 859.8842 | 932.2955 | 1086.4485 | 1031.7303 | 1.085x |
| multipleStores | 1048576 | 5736.8355 | 593.2252 | 579.6902 | 634.9950 | 531.4918 | 538.0503 | 531.4807 | 0.913x |
| longChain | 1048576 | 9222.4633 | 957.1667 | 881.1365 | 923.8295 | 814.9987 | 971.2150 | 781.2518 | 0.954x |
| repeated | 1048576 | 5291.5352 | 961.5635 | 880.8590 | 905.5638 | 953.5047 | 903.3655 | 774.4735 | 0.973x |
| add | 1048576 | 2748.0275 | 452.1248 | 465.9655 | 409.2400 | 465.7432 | 548.5365 | 413.5665 | 1.139x |
| producer | 1048576 | 5725.0095 | 993.0930 | 984.7888 | 846.1047 | 448.6650 | 520.1712 | 876.4757 | 1.164x |

## Interpretation and limits

* A/B/C/D native loads per eight elements: SIMD 3/5/4/7; reuse 3/3/4/5; forward 2/2/2/2. Stores stay 2/3/3/4. Counts come from linked vector bodies, excluding guard/fallback/remainder code.
* Repeated-read and add controls have identical reuse/forward code. Timings for identical code reflect measurement and placement variability, not a transformation benefit.
* Fusion keeps its separate applicability: producer has two loads/one store when fused; forwarding preserves all stores and does not fuse loops.
* Scalar is the existing spill-based TAC backend. Speedups over it combine SIMD with removal of scalar TAC overhead; compare GCC as well.
* CPU is the recorded Intel Core i7-1255U under WSL, pinned to one logical CPU. Warm buffers and stateful repeated calls; initialization, resets, I/O and validation excluded. No hardware counters or DRAM-byte measurement.
* Changed-code slowdowns can arise from value dependency chains, cache-resident reloads supported by hardware store forwarding, register lifetimes, layout, turbo or noise. These experiments do not isolate a microarchitectural cause.
* Cost weights are not fitted to these kernels. Cost selection models issued loads and pressure, not all dependencies, throughput overlap or hardware store forwarding. Identical explicit implementations are grouped before judging predictions.
* Raw data, exact weights, CPU details and hashes are retained below. Generated source, TAC, traces, assembly, linked objects and disassembly are under build/forward-benchmarks.

[Raw samples](raw.csv) · [Medians](summary.csv) · [Predictions](predictions.csv) · [Paired comparisons](comparisons.csv) · [Native instruction counts](instruction-counts.csv) · [Environment](environment.json)
