# simdwrap

A small C99 library for SIMD math operations (add, multiply, FMA) with runtime backend selection.

## Overview

This library provides:

- A runtime-detected SIMD backend (Scalar, SSE, AVX2, NEON)
- Function pointers for basic SIMD operations: `add`, `mul`, `fma`
- Simple C API with no third-party dependencies

## ⚠️⚠️⚠️⚠️⚠️⚠️ Disclaimer

This library is **not production-ready**. It was created purely as an experiment to avoid relying on harder to use third-party libraries.

If you need a robust SIMD library, you should look elsewhere (e.g., [xsimd](https://github.com/xtensor-stack/xsimd) or compiler intrinsics directly).

Use this only for learning, experimentation, or personal projects.

I might occasionally improve this project.

## Example usage

```c
#include "simdwrap.h"

int main() {
    simd_init();

    float a[4] = {1,2,3,4};
    float b[4] = {5,6,7,8};
    float c[4];

    simd_add(a, b, c, 4, sizeof(float));
    return 0;
}
```