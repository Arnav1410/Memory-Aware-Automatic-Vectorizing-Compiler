#include <stdio.h>

#if defined(__GNUC__) || defined(__clang__)
#define HAS_CPU_SUPPORT(x) __builtin_cpu_supports(x)
#else
#define HAS_CPU_SUPPORT(x) 0
#endif

int main(void) {
    printf("SSE2    : %s\n", HAS_CPU_SUPPORT("sse2") ? "yes" : "no");
    printf("AVX     : %s\n", HAS_CPU_SUPPORT("avx") ? "yes" : "no");
    printf("AVX2    : %s\n", HAS_CPU_SUPPORT("avx2") ? "yes" : "no");
    printf("AVX512F : %s\n", HAS_CPU_SUPPORT("avx512f") ? "yes" : "no");
    return 0;
}
