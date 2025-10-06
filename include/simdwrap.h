#ifndef SIMDWRAP_H
#define SIMDWRAP_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

    /* Opaque enum describing what backend is used */
    typedef enum {
        SIMD_BACKEND_SCALAR = 0,
        SIMD_BACKEND_SSE,
        SIMD_BACKEND_AVX2,
        SIMD_BACKEND_NEON
    } simd_backend_t;

    /* Function pointer table for each backend implementation */
    typedef struct simd_ops_t {
        void (*add)(const void *a, const void *b, void *dst, size_t n, size_t elem_size);
        void (*mul)(const void *a, const void *b, void *dst, size_t n, size_t elem_size);
        void (*fma)(const void *a, const void *b, const void *c, void *dst, size_t n, size_t elem_size);
    } simd_ops_t;

    /* Global API: dispatch table and init function */
    extern simd_ops_t simd;
    extern simd_backend_t simd_backend;

    /* Detect best backend at runtime (AVX2 > SSE > NEON > scalar) */
    void simd_init(void);

    /* These always dispatch to the selected backend */
    static inline void simd_add(const void *a, const void *b, void *dst, size_t n, size_t elem_size) {
        simd.add(a, b, dst, n, elem_size);
    }
    static inline void simd_mul(const void *a, const void *b, void *dst, size_t n, size_t elem_size) {
        simd.mul(a, b, dst, n, elem_size);
    }
    static inline void simd_fma(const void *a, const void *b, const void *c, void *dst, size_t n, size_t elem_size) {
        simd.fma(a, b, c, dst, n, elem_size);
    }

    /* Get selected backend */
    static inline simd_backend_t simd_get_backend(void) {
        return simd_backend;
    }

#ifdef __cplusplus
}
#endif

#endif /* SIMDWRAP_H */
