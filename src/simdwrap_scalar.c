#include "../include/simdwrap.h"
#include <stddef.h>

void simd_add_scalar(const void *a, const void *b, void *dst, size_t n, size_t elem_size) {
    const unsigned char *pa = a;
    const unsigned char *pb = b;
    unsigned char *pd = dst;
    for (size_t i = 0; i < n * elem_size; i++) {
        pd[i] = pa[i] + pb[i]; /* bytewise add; you can specialize for known elem_size */
    }
}

void simd_mul_scalar(const void *a, const void *b, void *dst, size_t n, size_t elem_size) {
    if (elem_size == sizeof(float)) {
        const float *fa = a, *fb = b; float *fd = dst;
        for (size_t i = 0; i < n; ++i) fd[i] = fa[i] * fb[i];
    } else if (elem_size == sizeof(double)) {
        const double *da = a, *db = b; double *dd = dst;
        for (size_t i = 0; i < n; ++i) dd[i] = da[i] * db[i];
    } else {
        const unsigned char *pa = a, *pb = b;
        unsigned char *pd = dst;
        for (size_t i = 0; i < n * elem_size; i++) pd[i] = pa[i] * pb[i];
    }
}

void simd_fma_scalar(const void *a, const void *b, const void *c, void *dst, size_t n, size_t elem_size) {
    if (elem_size == sizeof(float)) {
        const float *fa = a, *fb = b, *fc = c; float *fd = dst;
        for (size_t i = 0; i < n; ++i) fd[i] = fa[i] * fb[i] + fc[i];
    } else if (elem_size == sizeof(double)) {
        const double *da = a, *db = b, *dc = c; double *dd = dst;
        for (size_t i = 0; i < n; ++i) dd[i] = da[i] * db[i] + dc[i];
    }
}
