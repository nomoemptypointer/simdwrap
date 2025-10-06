#include "../include/simdwrap.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

simd_ops_t simd;
simd_backend_t simd_backend = SIMD_BACKEND_SCALAR;

/* Forward decls of backends */
extern void simd_add_scalar(const void*, const void*, void*, size_t, size_t);
extern void simd_mul_scalar(const void*, const void*, void*, size_t, size_t);
extern void simd_fma_scalar(const void*, const void*, const void*, void*, size_t, size_t);

#if defined(__x86_64__) || defined(_M_X64)
extern void simd_add_sse(const void*, const void*, void*, size_t, size_t);
extern void simd_mul_sse(const void*, const void*, void*, size_t, size_t);
extern void simd_fma_sse(const void*, const void*, const void*, void*, size_t, size_t);

extern void simd_add_avx2(const void*, const void*, void*, size_t, size_t);
extern void simd_mul_avx2(const void*, const void*, void*, size_t, size_t);
extern void simd_fma_avx2(const void*, const void*, const void*, void*, size_t, size_t);
#endif

#if defined(__ARM_NEON) || defined(__aarch64__)
extern void simd_add_neon(const void*, const void*, void*, size_t, size_t);
extern void simd_mul_neon(const void*, const void*, void*, size_t, size_t);
extern void simd_fma_neon(const void*, const void*, const void*, void*, size_t, size_t);
#endif

/* CPU feature detection (x86 only) */
#if defined(__x86_64__) || defined(_M_X64)
#include <cpuid.h>

static int cpu_has_avx2(void) {
    unsigned int eax, ebx, ecx, edx;
    if (__get_cpuid_max(0, NULL) < 7) return 0;
    __cpuid_count(7, 0, eax, ebx, ecx, edx);
    return (ebx & (1 << 5)) != 0; /* AVX2 bit */
}
static int cpu_has_sse(void) {
    unsigned int eax, ebx, ecx, edx;
    if (!__get_cpuid_max(1, NULL)) return 0;
    __cpuid(1, eax, ebx, ecx, edx);
    return (edx & (1 << 25)) != 0; /* SSE */
}
#endif

void simd_init(void) {
    simd_backend_t best = SIMD_BACKEND_SCALAR;

#if defined(__x86_64__) || defined(_M_X64)
    if (cpu_has_avx2()) {
        best = SIMD_BACKEND_AVX2;
    } else if (cpu_has_sse()) {
        best = SIMD_BACKEND_SSE;
    }
#elif defined(__ARM_NEON) || defined(__aarch64__)
    best = SIMD_BACKEND_NEON;
#endif

    simd_backend = best;

    switch (best) {
        case SIMD_BACKEND_AVX2:
#if defined(__x86_64__)
            simd.add = simd_add_avx2;
            simd.mul = simd_mul_avx2;
            simd.fma = simd_fma_avx2;
            //printf("[simdwrap] Using AVX2 backend\n");
            break;
#endif
        case SIMD_BACKEND_SSE:
#if defined(__x86_64__)
            simd.add = simd_add_sse;
            simd.mul = simd_mul_sse;
            simd.fma = simd_fma_sse;
            //printf("[simdwrap] Using SSE backend\n");
            break;
#endif
        case SIMD_BACKEND_NEON:
#if defined(__ARM_NEON) || defined(__aarch64__)
            simd.add = simd_add_neon;
            simd.mul = simd_mul_neon;
            simd.fma = simd_fma_neon;
            //printf("[simdwrap] Using NEON backend\n");
            break;
#endif
        default:
            simd.add = simd_add_scalar;
            simd.mul = simd_mul_scalar;
            simd.fma = simd_fma_scalar;
            //printf("[simdwrap] Using scalar backend\n");
            break;
    }
}
