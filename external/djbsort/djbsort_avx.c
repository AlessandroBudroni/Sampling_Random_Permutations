#include <immintrin.h>

#include "djbsort.h"

typedef __m256i int32x8;
#define int32x8_load(z)     _mm256_loadu_si256((__m256i *)(z))
#define int32x8_store(z, i) _mm256_storeu_si256((__m256i *)(z), (i))
#define int32x8_min         _mm256_min_epi32
#define int32x8_max         _mm256_max_epi32

#define int32_MINMAX(a, b)                  \
    do {                                    \
        int32_t temp1;                      \
        __asm__(                            \
            "cmpl %1,%0\n\t"                \
            "mov %0,%2\n\t"                 \
            "cmovg %1,%0\n\t"               \
            "cmovg %2,%1\n\t"               \
            : "+r"(a), "+r"(b), "=r"(temp1) \
            :                               \
            : "cc");                        \
    } while (0)

#define int32x8_MINMAX(a, b)               \
    do {                                   \
        int32x8 c = int32x8_min((a), (b)); \
        (b) = int32x8_max((a), (b));       \
        (a) = c;                           \
    } while (0)

__attribute__((noinline)) static void minmax_vector(int32_t *x, int32_t *y, int64_t n) {
    if (n < 8) {
        while (n > 0) {
            int32_MINMAX(*x, *y);
            ++x;
            ++y;
            --n;
        }
        return;
    }
    if (n & 7) {
        int32x8 x0 = int32x8_load(x + n - 8);
        int32x8 y0 = int32x8_load(y + n - 8);
        int32x8_MINMAX(x0, y0);
        int32x8_store(x + n - 8, x0);
        int32x8_store(y + n - 8, y0);
        n &= ~7;
    }
    do {
        int32x8 x0 = int32x8_load(x);
        int32x8 y0 = int32x8_load(y);
        int32x8_MINMAX(x0, y0);
        int32x8_store(x, x0);
        int32x8_store(y, y0);
        x += 8;
        y += 8;
        n -= 8;
    } while (n);
}

/* stages 8,4,2,1 of size-16 bitonic merging */
__attribute__((noinline)) static void merge16_finish(int32_t *x, int32x8 *x0, int32x8 *x1, int flagdown) {
    int32x8 b0, b1, c0, c1, mask;

    int32x8_MINMAX(*x0, *x1);

    b0 = _mm256_permute2x128_si256(*x0, *x1, 0x20);
    b1 = _mm256_permute2x128_si256(*x0, *x1, 0x31);

    int32x8_MINMAX(b0, b1);

    c0 = _mm256_unpacklo_epi64(b0, b1);
    c1 = _mm256_unpackhi_epi64(b0, b1);

    int32x8_MINMAX(c0, c1);

    b0 = _mm256_unpacklo_epi32(c0, c1);
    b1 = _mm256_unpackhi_epi32(c0, c1);

    c0 = _mm256_unpacklo_epi64(b0, b1);
    c1 = _mm256_unpackhi_epi64(b0, b1);

    int32x8_MINMAX(c0, c1);

    b0 = _mm256_unpacklo_epi32(c0, c1);
    b1 = _mm256_unpackhi_epi32(c0, c1);

    *x0 = _mm256_permute2x128_si256(b0, b1, 0x20);
    *x1 = _mm256_permute2x128_si256(b0, b1, 0x31);

    if (flagdown) {
        mask = _mm256_set1_epi32(-1);
        *x0 = _mm256_xor_si256(*x0, mask);
        *x1 = _mm256_xor_si256(*x1, mask);
    }

    int32x8_store(&x[0], *x0);
    int32x8_store(&x[8], *x1);
}

/* stages 64,32 of bitonic merging; n is multiple of 128 */
__attribute__((noinline)) static void int32_twostages_32(int32_t *x, uint64_t n) {
    uint64_t i;

    while (n > 0) {
        for (i = 0; i < 32; i += 8) {
            int32x8 x0 = int32x8_load(&x[i]);
            int32x8 x1 = int32x8_load(&x[i + 32]);
            int32x8 x2 = int32x8_load(&x[i + 64]);
            int32x8 x3 = int32x8_load(&x[i + 96]);

            int32x8_MINMAX(x0, x2);
            int32x8_MINMAX(x1, x3);
            int32x8_MINMAX(x0, x1);
            int32x8_MINMAX(x2, x3);

            int32x8_store(&x[i], x0);
            int32x8_store(&x[i + 32], x1);
            int32x8_store(&x[i + 64], x2);
            int32x8_store(&x[i + 96], x3);
        }
        x += 128;
        n -= 128;
    }
}

/* stages 4q,2q,q of bitonic merging */
__attribute__((noinline)) static uint64_t int32_threestages(int32_t *x, uint64_t n, uint64_t q) {
    uint64_t k, i;

    for (k = 0; k + 8 * q <= n; k += 8 * q)
        for (i = k; i < k + q; i += 8) {
            int32x8 x0 = int32x8_load(&x[i]);
            int32x8 x1 = int32x8_load(&x[i + q]);
            int32x8 x2 = int32x8_load(&x[i + 2 * q]);
            int32x8 x3 = int32x8_load(&x[i + 3 * q]);
            int32x8 x4 = int32x8_load(&x[i + 4 * q]);
            int32x8 x5 = int32x8_load(&x[i + 5 * q]);
            int32x8 x6 = int32x8_load(&x[i + 6 * q]);
            int32x8 x7 = int32x8_load(&x[i + 7 * q]);

            int32x8_MINMAX(x0, x4);
            int32x8_MINMAX(x1, x5);
            int32x8_MINMAX(x2, x6);
            int32x8_MINMAX(x3, x7);
            int32x8_MINMAX(x0, x2);
            int32x8_MINMAX(x1, x3);
            int32x8_MINMAX(x4, x6);
            int32x8_MINMAX(x5, x7);
            int32x8_MINMAX(x0, x1);
            int32x8_MINMAX(x2, x3);
            int32x8_MINMAX(x4, x5);
            int32x8_MINMAX(x6, x7);

            int32x8_store(&x[i], x0);
            int32x8_store(&x[i + q], x1);
            int32x8_store(&x[i + 2 * q], x2);
            int32x8_store(&x[i + 3 * q], x3);
            int32x8_store(&x[i + 4 * q], x4);
            int32x8_store(&x[i + 5 * q], x5);
            int32x8_store(&x[i + 6 * q], x6);
            int32x8_store(&x[i + 7 * q], x7);
        }

    return k;
}

/* n is a power of 2; n >= 8; if n == 8 then flagdown */
__attribute__((noinline)) static void int32_sort_2power(int32_t *x, uint64_t n, int flagdown) {
    uint64_t p, q, i, j, k;
    int32x8 mask;

    if (n == 8) {
        int32_t x0 = x[0];
        int32_t x1 = x[1];
        int32_t x2 = x[2];
        int32_t x3 = x[3];
        int32_t x4 = x[4];
        int32_t x5 = x[5];
        int32_t x6 = x[6];
        int32_t x7 = x[7];

        /* odd-even sort instead of bitonic sort */

        int32_MINMAX(x1, x0);
        int32_MINMAX(x3, x2);
        int32_MINMAX(x2, x0);
        int32_MINMAX(x3, x1);
        int32_MINMAX(x2, x1);

        int32_MINMAX(x5, x4);
        int32_MINMAX(x7, x6);
        int32_MINMAX(x6, x4);
        int32_MINMAX(x7, x5);
        int32_MINMAX(x6, x5);

        int32_MINMAX(x4, x0);
        int32_MINMAX(x6, x2);
        int32_MINMAX(x4, x2);

        int32_MINMAX(x5, x1);
        int32_MINMAX(x7, x3);
        int32_MINMAX(x5, x3);

        int32_MINMAX(x2, x1);
        int32_MINMAX(x4, x3);
        int32_MINMAX(x6, x5);

        x[0] = x0;
        x[1] = x1;
        x[2] = x2;
        x[3] = x3;
        x[4] = x4;
        x[5] = x5;
        x[6] = x6;
        x[7] = x7;
        return;
    }

    if (n == 16) {
        int32x8 x0, x1, b0, b1, c0, c1;

        x0 = int32x8_load(&x[0]);
        x1 = int32x8_load(&x[8]);

        mask = _mm256_set_epi32(0, 0, -1, -1, 0, 0, -1, -1);

        x0 = _mm256_xor_si256(x0, mask);
        x1 = _mm256_xor_si256(x1, mask);

        b0 = _mm256_unpacklo_epi32(x0, x1);
        b1 = _mm256_unpackhi_epi32(x0, x1);

        c0 = _mm256_unpacklo_epi64(b0, b1);
        c1 = _mm256_unpackhi_epi64(b0, b1);

        int32x8_MINMAX(c0, c1);

        mask = _mm256_set_epi32(0, 0, -1, -1, -1, -1, 0, 0);
        c0 = _mm256_xor_si256(c0, mask);
        c1 = _mm256_xor_si256(c1, mask);

        b0 = _mm256_unpacklo_epi32(c0, c1);
        b1 = _mm256_unpackhi_epi32(c0, c1);

        int32x8_MINMAX(b0, b1);

        x0 = _mm256_unpacklo_epi64(b0, b1);
        x1 = _mm256_unpackhi_epi64(b0, b1);

        b0 = _mm256_unpacklo_epi32(x0, x1);
        b1 = _mm256_unpackhi_epi32(x0, x1);

        c0 = _mm256_unpacklo_epi64(b0, b1);
        c1 = _mm256_unpackhi_epi64(b0, b1);

        int32x8_MINMAX(c0, c1);

        b0 = _mm256_unpacklo_epi32(c0, c1);
        b1 = _mm256_unpackhi_epi32(c0, c1);

        b0 = _mm256_xor_si256(b0, mask);
        b1 = _mm256_xor_si256(b1, mask);

        c0 = _mm256_permute2x128_si256(b0, b1, 0x20);
        c1 = _mm256_permute2x128_si256(b0, b1, 0x31);

        int32x8_MINMAX(c0, c1);

        b0 = _mm256_permute2x128_si256(c0, c1, 0x20);
        b1 = _mm256_permute2x128_si256(c0, c1, 0x31);

        int32x8_MINMAX(b0, b1);

        x0 = _mm256_unpacklo_epi64(b0, b1);
        x1 = _mm256_unpackhi_epi64(b0, b1);

        b0 = _mm256_unpacklo_epi32(x0, x1);
        b1 = _mm256_unpackhi_epi32(x0, x1);

        c0 = _mm256_unpacklo_epi64(b0, b1);
        c1 = _mm256_unpackhi_epi64(b0, b1);

        int32x8_MINMAX(c0, c1);

        b0 = _mm256_unpacklo_epi32(c0, c1);
        b1 = _mm256_unpackhi_epi32(c0, c1);

        x0 = _mm256_unpacklo_epi64(b0, b1);
        x1 = _mm256_unpackhi_epi64(b0, b1);

        mask = _mm256_set1_epi32(-1);
        if (flagdown) {
            x1 = _mm256_xor_si256(x1, mask);
        } else {
            x0 = _mm256_xor_si256(x0, mask);
        }

        merge16_finish(x, &x0, &x1, flagdown);
        return;
    }

    if (n == 32) {
        int32x8 x0, x1, x2, x3;

        int32_sort_2power(x, 16, 1);
        int32_sort_2power(x + 16, 16, 0);

        x0 = int32x8_load(&x[0]);
        x1 = int32x8_load(&x[8]);
        x2 = int32x8_load(&x[16]);
        x3 = int32x8_load(&x[24]);

        if (flagdown) {
            mask = _mm256_set1_epi32(-1);
            x0 = _mm256_xor_si256(x0, mask);
            x1 = _mm256_xor_si256(x1, mask);
            x2 = _mm256_xor_si256(x2, mask);
            x3 = _mm256_xor_si256(x3, mask);
        }

        int32x8_MINMAX(x0, x2);
        int32x8_MINMAX(x1, x3);

        merge16_finish(x, &x0, &x1, flagdown);
        merge16_finish(x + 16, &x2, &x3, flagdown);
        return;
    }

    p = n >> 3;
    for (i = 0; i < p; i += 8) {
        int32x8 x0 = int32x8_load(&x[i]);
        int32x8 x2 = int32x8_load(&x[i + 2 * p]);
        int32x8 x4 = int32x8_load(&x[i + 4 * p]);
        int32x8 x6 = int32x8_load(&x[i + 6 * p]);

        /* odd-even stage instead of bitonic stage */

        int32x8_MINMAX(x4, x0);
        int32x8_MINMAX(x6, x2);
        int32x8_MINMAX(x2, x0);
        int32x8_MINMAX(x6, x4);
        int32x8_MINMAX(x2, x4);

        int32x8_store(&x[i], x0);
        int32x8_store(&x[i + 2 * p], x2);
        int32x8_store(&x[i + 4 * p], x4);
        int32x8_store(&x[i + 6 * p], x6);

        int32x8 x1 = int32x8_load(&x[i + p]);
        int32x8 x3 = int32x8_load(&x[i + 3 * p]);
        int32x8 x5 = int32x8_load(&x[i + 5 * p]);
        int32x8 x7 = int32x8_load(&x[i + 7 * p]);

        int32x8_MINMAX(x1, x5);
        int32x8_MINMAX(x3, x7);
        int32x8_MINMAX(x1, x3);
        int32x8_MINMAX(x5, x7);
        int32x8_MINMAX(x5, x3);

        int32x8_store(&x[i + p], x1);
        int32x8_store(&x[i + 3 * p], x3);
        int32x8_store(&x[i + 5 * p], x5);
        int32x8_store(&x[i + 7 * p], x7);
    }

    if (n >= 128) {
        int flip, flipflip;

        mask = _mm256_set1_epi32(-1);

        for (j = 0; j < n; j += 32) {
            int32x8 x0 = int32x8_load(&x[j]);
            int32x8 x1 = int32x8_load(&x[j + 16]);
            x0 = _mm256_xor_si256(x0, mask);
            x1 = _mm256_xor_si256(x1, mask);
            int32x8_store(&x[j], x0);
            int32x8_store(&x[j + 16], x1);
        }

        p = 8;
        for (;;) { /* for p in [8, 16, ..., n/16] */
            q = p >> 1;
            while (q >= 128) {
                int32_threestages(x, n, q >> 2);
                q >>= 3;
            }
            if (q == 64) {
                int32_twostages_32(x, n);
                q = 16;
            }
            if (q == 32) {
                q = 8;
                for (k = 0; k < n; k += 8 * q)
                    for (i = k; i < k + q; i += 8) {
                        int32x8 x0 = int32x8_load(&x[i]);
                        int32x8 x1 = int32x8_load(&x[i + q]);
                        int32x8 x2 = int32x8_load(&x[i + 2 * q]);
                        int32x8 x3 = int32x8_load(&x[i + 3 * q]);
                        int32x8 x4 = int32x8_load(&x[i + 4 * q]);
                        int32x8 x5 = int32x8_load(&x[i + 5 * q]);
                        int32x8 x6 = int32x8_load(&x[i + 6 * q]);
                        int32x8 x7 = int32x8_load(&x[i + 7 * q]);

                        int32x8_MINMAX(x0, x4);
                        int32x8_MINMAX(x1, x5);
                        int32x8_MINMAX(x2, x6);
                        int32x8_MINMAX(x3, x7);
                        int32x8_MINMAX(x0, x2);
                        int32x8_MINMAX(x1, x3);
                        int32x8_MINMAX(x4, x6);
                        int32x8_MINMAX(x5, x7);
                        int32x8_MINMAX(x0, x1);
                        int32x8_MINMAX(x2, x3);
                        int32x8_MINMAX(x4, x5);
                        int32x8_MINMAX(x6, x7);

                        int32x8_store(&x[i], x0);
                        int32x8_store(&x[i + q], x1);
                        int32x8_store(&x[i + 2 * q], x2);
                        int32x8_store(&x[i + 3 * q], x3);
                        int32x8_store(&x[i + 4 * q], x4);
                        int32x8_store(&x[i + 5 * q], x5);
                        int32x8_store(&x[i + 6 * q], x6);
                        int32x8_store(&x[i + 7 * q], x7);
                    }
                q = 4;
            }
            if (q == 16) {
                q = 8;
                for (k = 0; k < n; k += 4 * q)
                    for (i = k; i < k + q; i += 8) {
                        int32x8 x0 = int32x8_load(&x[i]);
                        int32x8 x1 = int32x8_load(&x[i + q]);
                        int32x8 x2 = int32x8_load(&x[i + 2 * q]);
                        int32x8 x3 = int32x8_load(&x[i + 3 * q]);

                        int32x8_MINMAX(x0, x2);
                        int32x8_MINMAX(x1, x3);
                        int32x8_MINMAX(x0, x1);
                        int32x8_MINMAX(x2, x3);

                        int32x8_store(&x[i], x0);
                        int32x8_store(&x[i + q], x1);
                        int32x8_store(&x[i + 2 * q], x2);
                        int32x8_store(&x[i + 3 * q], x3);
                    }
                q = 4;
            }
            if (q == 8)
                for (k = 0; k < n; k += q + q) {
                    int32x8 x0 = int32x8_load(&x[k]);
                    int32x8 x1 = int32x8_load(&x[k + q]);

                    int32x8_MINMAX(x0, x1);

                    int32x8_store(&x[k], x0);
                    int32x8_store(&x[k + q], x1);
                }

            q = n >> 3;
            flip = (p << 1 == q);
            flipflip = !flip;
            for (j = 0; j < q; j += p + p) {
                for (k = j; k < j + p + p; k += p) {
                    for (i = k; i < k + p; i += 8) {
                        int32x8 x0 = int32x8_load(&x[i]);
                        int32x8 x1 = int32x8_load(&x[i + q]);
                        int32x8 x2 = int32x8_load(&x[i + 2 * q]);
                        int32x8 x3 = int32x8_load(&x[i + 3 * q]);
                        int32x8 x4 = int32x8_load(&x[i + 4 * q]);
                        int32x8 x5 = int32x8_load(&x[i + 5 * q]);
                        int32x8 x6 = int32x8_load(&x[i + 6 * q]);
                        int32x8 x7 = int32x8_load(&x[i + 7 * q]);

                        int32x8_MINMAX(x0, x1);
                        int32x8_MINMAX(x2, x3);
                        int32x8_MINMAX(x4, x5);
                        int32x8_MINMAX(x6, x7);
                        int32x8_MINMAX(x0, x2);
                        int32x8_MINMAX(x1, x3);
                        int32x8_MINMAX(x4, x6);
                        int32x8_MINMAX(x5, x7);
                        int32x8_MINMAX(x0, x4);
                        int32x8_MINMAX(x1, x5);
                        int32x8_MINMAX(x2, x6);
                        int32x8_MINMAX(x3, x7);

                        if (flip) {
                            x0 = _mm256_xor_si256(x0, mask);
                            x1 = _mm256_xor_si256(x1, mask);
                            x2 = _mm256_xor_si256(x2, mask);
                            x3 = _mm256_xor_si256(x3, mask);
                            x4 = _mm256_xor_si256(x4, mask);
                            x5 = _mm256_xor_si256(x5, mask);
                            x6 = _mm256_xor_si256(x6, mask);
                            x7 = _mm256_xor_si256(x7, mask);
                        }

                        int32x8_store(&x[i], x0);
                        int32x8_store(&x[i + q], x1);
                        int32x8_store(&x[i + 2 * q], x2);
                        int32x8_store(&x[i + 3 * q], x3);
                        int32x8_store(&x[i + 4 * q], x4);
                        int32x8_store(&x[i + 5 * q], x5);
                        int32x8_store(&x[i + 6 * q], x6);
                        int32x8_store(&x[i + 7 * q], x7);
                    }
                    flip ^= 1;
                }
                flip ^= flipflip;
            }

            if (p << 4 == n) break;
            p <<= 1;
        }
    }

    for (p = 4; p >= 1; p >>= 1) {
        int32_t *z = x;
        int32_t *target = x + n;
        if (p == 4) {
            mask = _mm256_set_epi32(0, 0, 0, 0, -1, -1, -1, -1);
            while (z != target) {
                int32x8 x0 = int32x8_load(&z[0]);
                int32x8 x1 = int32x8_load(&z[8]);
                x0 = _mm256_xor_si256(x0, mask);
                x1 = _mm256_xor_si256(x1, mask);
                int32x8_store(&z[0], x0);
                int32x8_store(&z[8], x1);
                z += 16;
            }
        } else if (p == 2) {
            mask = _mm256_set_epi32(0, 0, -1, -1, -1, -1, 0, 0);
            while (z != target) {
                int32x8 x0 = int32x8_load(&z[0]);
                int32x8 x1 = int32x8_load(&z[8]);
                x0 = _mm256_xor_si256(x0, mask);
                x1 = _mm256_xor_si256(x1, mask);
                int32x8 b0 = _mm256_permute2x128_si256(x0, x1, 0x20);
                int32x8 b1 = _mm256_permute2x128_si256(x0, x1, 0x31);
                int32x8_MINMAX(b0, b1);
                int32x8 c0 = _mm256_permute2x128_si256(b0, b1, 0x20);
                int32x8 c1 = _mm256_permute2x128_si256(b0, b1, 0x31);
                int32x8_store(&z[0], c0);
                int32x8_store(&z[8], c1);
                z += 16;
            }
        } else { /* p == 1 */
            mask = _mm256_set_epi32(0, -1, -1, 0, 0, -1, -1, 0);
            while (z != target) {
                int32x8 x0 = int32x8_load(&z[0]);
                int32x8 x1 = int32x8_load(&z[8]);
                x0 = _mm256_xor_si256(x0, mask);
                x1 = _mm256_xor_si256(x1, mask);
                int32x8 b0 = _mm256_permute2x128_si256(x0, x1, 0x20);
                int32x8 b1 = _mm256_permute2x128_si256(x0, x1, 0x31);
                int32x8 c0 = _mm256_unpacklo_epi64(b0, b1);
                int32x8 c1 = _mm256_unpackhi_epi64(b0, b1);
                int32x8_MINMAX(c0, c1);
                int32x8 d0 = _mm256_unpacklo_epi64(c0, c1);
                int32x8 d1 = _mm256_unpackhi_epi64(c0, c1);
                int32x8_MINMAX(d0, d1);
                int32x8 e0 = _mm256_permute2x128_si256(d0, d1, 0x20);
                int32x8 e1 = _mm256_permute2x128_si256(d0, d1, 0x31);
                int32x8_store(&z[0], e0);
                int32x8_store(&z[8], e1);
                z += 16;
            }
        }

        q = n >> 4;
        while (q >= 128 || q == 32) {
            int32_threestages(x, n, q >> 2);
            q >>= 3;
        }
        while (q >= 16) {
            q >>= 1;
            for (j = 0; j < n; j += 4 * q)
                for (k = j; k < j + q; k += 8) {
                    int32x8 x0 = int32x8_load(&x[k]);
                    int32x8 x1 = int32x8_load(&x[k + q]);
                    int32x8 x2 = int32x8_load(&x[k + 2 * q]);
                    int32x8 x3 = int32x8_load(&x[k + 3 * q]);

                    int32x8_MINMAX(x0, x2);
                    int32x8_MINMAX(x1, x3);
                    int32x8_MINMAX(x0, x1);
                    int32x8_MINMAX(x2, x3);

                    int32x8_store(&x[k], x0);
                    int32x8_store(&x[k + q], x1);
                    int32x8_store(&x[k + 2 * q], x2);
                    int32x8_store(&x[k + 3 * q], x3);
                }
            q >>= 1;
        }
        if (q == 8) {
            for (j = 0; j < n; j += 2 * q) {
                int32x8 x0 = int32x8_load(&x[j]);
                int32x8 x1 = int32x8_load(&x[j + q]);

                int32x8_MINMAX(x0, x1);

                int32x8_store(&x[j], x0);
                int32x8_store(&x[j + q], x1);
            }
        }

        q = n >> 3;
        for (k = 0; k < q; k += 8) {
            int32x8 x0 = int32x8_load(&x[k]);
            int32x8 x1 = int32x8_load(&x[k + q]);
            int32x8 x2 = int32x8_load(&x[k + 2 * q]);
            int32x8 x3 = int32x8_load(&x[k + 3 * q]);
            int32x8 x4 = int32x8_load(&x[k + 4 * q]);
            int32x8 x5 = int32x8_load(&x[k + 5 * q]);
            int32x8 x6 = int32x8_load(&x[k + 6 * q]);
            int32x8 x7 = int32x8_load(&x[k + 7 * q]);

            int32x8_MINMAX(x0, x1);
            int32x8_MINMAX(x2, x3);
            int32x8_MINMAX(x4, x5);
            int32x8_MINMAX(x6, x7);
            int32x8_MINMAX(x0, x2);
            int32x8_MINMAX(x1, x3);
            int32x8_MINMAX(x4, x6);
            int32x8_MINMAX(x5, x7);
            int32x8_MINMAX(x0, x4);
            int32x8_MINMAX(x1, x5);
            int32x8_MINMAX(x2, x6);
            int32x8_MINMAX(x3, x7);

            int32x8_store(&x[k], x0);
            int32x8_store(&x[k + q], x1);
            int32x8_store(&x[k + 2 * q], x2);
            int32x8_store(&x[k + 3 * q], x3);
            int32x8_store(&x[k + 4 * q], x4);
            int32x8_store(&x[k + 5 * q], x5);
            int32x8_store(&x[k + 6 * q], x6);
            int32x8_store(&x[k + 7 * q], x7);
        }
    }

    /* everything is still masked with _mm256_set_epi32(0,-1,0,-1,0,-1,0,-1); */
    mask = _mm256_set1_epi32(-1);

    for (i = 0; i < n; i += 64) {
        int32x8 a0 = int32x8_load(&x[i]);
        int32x8 a1 = int32x8_load(&x[i + 8]);
        int32x8 a2 = int32x8_load(&x[i + 16]);
        int32x8 a3 = int32x8_load(&x[i + 24]);
        int32x8 a4 = int32x8_load(&x[i + 32]);
        int32x8 a5 = int32x8_load(&x[i + 40]);
        int32x8 a6 = int32x8_load(&x[i + 48]);
        int32x8 a7 = int32x8_load(&x[i + 56]);

        int32x8 b0 = _mm256_unpacklo_epi32(a0, a1);
        int32x8 b1 = _mm256_unpackhi_epi32(a0, a1);
        int32x8 b2 = _mm256_unpacklo_epi32(a2, a3);
        int32x8 b3 = _mm256_unpackhi_epi32(a2, a3);
        int32x8 b4 = _mm256_unpacklo_epi32(a4, a5);
        int32x8 b5 = _mm256_unpackhi_epi32(a4, a5);
        int32x8 b6 = _mm256_unpacklo_epi32(a6, a7);
        int32x8 b7 = _mm256_unpackhi_epi32(a6, a7);

        int32x8 c0 = _mm256_unpacklo_epi64(b0, b2);
        int32x8 c1 = _mm256_unpacklo_epi64(b1, b3);
        int32x8 c2 = _mm256_unpackhi_epi64(b0, b2);
        int32x8 c3 = _mm256_unpackhi_epi64(b1, b3);
        int32x8 c4 = _mm256_unpacklo_epi64(b4, b6);
        int32x8 c5 = _mm256_unpacklo_epi64(b5, b7);
        int32x8 c6 = _mm256_unpackhi_epi64(b4, b6);
        int32x8 c7 = _mm256_unpackhi_epi64(b5, b7);

        if (flagdown) {
            c2 = _mm256_xor_si256(c2, mask);
            c3 = _mm256_xor_si256(c3, mask);
            c6 = _mm256_xor_si256(c6, mask);
            c7 = _mm256_xor_si256(c7, mask);
        } else {
            c0 = _mm256_xor_si256(c0, mask);
            c1 = _mm256_xor_si256(c1, mask);
            c4 = _mm256_xor_si256(c4, mask);
            c5 = _mm256_xor_si256(c5, mask);
        }

        int32x8 d0 = _mm256_permute2x128_si256(c0, c4, 0x20);
        int32x8 d1 = _mm256_permute2x128_si256(c2, c6, 0x20);
        int32x8 d2 = _mm256_permute2x128_si256(c1, c5, 0x20);
        int32x8 d3 = _mm256_permute2x128_si256(c3, c7, 0x20);
        int32x8 d4 = _mm256_permute2x128_si256(c0, c4, 0x31);
        int32x8 d5 = _mm256_permute2x128_si256(c2, c6, 0x31);
        int32x8 d6 = _mm256_permute2x128_si256(c1, c5, 0x31);
        int32x8 d7 = _mm256_permute2x128_si256(c3, c7, 0x31);

        int32x8_MINMAX(d0, d1);
        int32x8_MINMAX(d2, d3);
        int32x8_MINMAX(d4, d5);
        int32x8_MINMAX(d6, d7);
        int32x8_MINMAX(d0, d2);
        int32x8_MINMAX(d1, d3);
        int32x8_MINMAX(d4, d6);
        int32x8_MINMAX(d5, d7);
        int32x8_MINMAX(d0, d4);
        int32x8_MINMAX(d1, d5);
        int32x8_MINMAX(d2, d6);
        int32x8_MINMAX(d3, d7);

        int32x8 e0 = _mm256_unpacklo_epi32(d0, d1);
        int32x8 e1 = _mm256_unpackhi_epi32(d0, d1);
        int32x8 e2 = _mm256_unpacklo_epi32(d2, d3);
        int32x8 e3 = _mm256_unpackhi_epi32(d2, d3);
        int32x8 e4 = _mm256_unpacklo_epi32(d4, d5);
        int32x8 e5 = _mm256_unpackhi_epi32(d4, d5);
        int32x8 e6 = _mm256_unpacklo_epi32(d6, d7);
        int32x8 e7 = _mm256_unpackhi_epi32(d6, d7);

        int32x8 f0 = _mm256_unpacklo_epi64(e0, e2);
        int32x8 f1 = _mm256_unpacklo_epi64(e1, e3);
        int32x8 f2 = _mm256_unpackhi_epi64(e0, e2);
        int32x8 f3 = _mm256_unpackhi_epi64(e1, e3);
        int32x8 f4 = _mm256_unpacklo_epi64(e4, e6);
        int32x8 f5 = _mm256_unpacklo_epi64(e5, e7);
        int32x8 f6 = _mm256_unpackhi_epi64(e4, e6);
        int32x8 f7 = _mm256_unpackhi_epi64(e5, e7);

        int32x8 g0 = _mm256_permute2x128_si256(f0, f4, 0x20);
        int32x8 g1 = _mm256_permute2x128_si256(f2, f6, 0x20);
        int32x8 g2 = _mm256_permute2x128_si256(f1, f5, 0x20);
        int32x8 g3 = _mm256_permute2x128_si256(f3, f7, 0x20);
        int32x8 g4 = _mm256_permute2x128_si256(f0, f4, 0x31);
        int32x8 g5 = _mm256_permute2x128_si256(f2, f6, 0x31);
        int32x8 g6 = _mm256_permute2x128_si256(f1, f5, 0x31);
        int32x8 g7 = _mm256_permute2x128_si256(f3, f7, 0x31);

        int32x8_store(&x[i], g0);
        int32x8_store(&x[i + 8], g1);
        int32x8_store(&x[i + 16], g2);
        int32x8_store(&x[i + 24], g3);
        int32x8_store(&x[i + 32], g4);
        int32x8_store(&x[i + 40], g5);
        int32x8_store(&x[i + 48], g6);
        int32x8_store(&x[i + 56], g7);
    }

    q = n >> 4;
    while (q >= 128 || q == 32) {
        q >>= 2;
        for (j = 0; j < n; j += 8 * q)
            for (i = j; i < j + q; i += 8) {
                int32x8 x0 = int32x8_load(&x[i]);
                int32x8 x1 = int32x8_load(&x[i + q]);
                int32x8 x2 = int32x8_load(&x[i + 2 * q]);
                int32x8 x3 = int32x8_load(&x[i + 3 * q]);
                int32x8 x4 = int32x8_load(&x[i + 4 * q]);
                int32x8 x5 = int32x8_load(&x[i + 5 * q]);
                int32x8 x6 = int32x8_load(&x[i + 6 * q]);
                int32x8 x7 = int32x8_load(&x[i + 7 * q]);
                int32x8_MINMAX(x0, x4);
                int32x8_MINMAX(x1, x5);
                int32x8_MINMAX(x2, x6);
                int32x8_MINMAX(x3, x7);
                int32x8_MINMAX(x0, x2);
                int32x8_MINMAX(x1, x3);
                int32x8_MINMAX(x4, x6);
                int32x8_MINMAX(x5, x7);
                int32x8_MINMAX(x0, x1);
                int32x8_MINMAX(x2, x3);
                int32x8_MINMAX(x4, x5);
                int32x8_MINMAX(x6, x7);
                int32x8_store(&x[i], x0);
                int32x8_store(&x[i + q], x1);
                int32x8_store(&x[i + 2 * q], x2);
                int32x8_store(&x[i + 3 * q], x3);
                int32x8_store(&x[i + 4 * q], x4);
                int32x8_store(&x[i + 5 * q], x5);
                int32x8_store(&x[i + 6 * q], x6);
                int32x8_store(&x[i + 7 * q], x7);
            }
        q >>= 1;
    }
    while (q >= 16) {
        q >>= 1;
        for (j = 0; j < n; j += 4 * q)
            for (i = j; i < j + q; i += 8) {
                int32x8 x0 = int32x8_load(&x[i]);
                int32x8 x1 = int32x8_load(&x[i + q]);
                int32x8 x2 = int32x8_load(&x[i + 2 * q]);
                int32x8 x3 = int32x8_load(&x[i + 3 * q]);
                int32x8_MINMAX(x0, x2);
                int32x8_MINMAX(x1, x3);
                int32x8_MINMAX(x0, x1);
                int32x8_MINMAX(x2, x3);
                int32x8_store(&x[i], x0);
                int32x8_store(&x[i + q], x1);
                int32x8_store(&x[i + 2 * q], x2);
                int32x8_store(&x[i + 3 * q], x3);
            }
        q >>= 1;
    }
    if (q == 8)
        for (j = 0; j < n; j += q + q) {
            int32x8 x0 = int32x8_load(&x[j]);
            int32x8 x1 = int32x8_load(&x[j + q]);
            int32x8_MINMAX(x0, x1);
            int32x8_store(&x[j], x0);
            int32x8_store(&x[j + q], x1);
        }

    q = n >> 3;
    for (i = 0; i < q; i += 8) {
        int32x8 x0 = int32x8_load(&x[i]);
        int32x8 x1 = int32x8_load(&x[i + q]);
        int32x8 x2 = int32x8_load(&x[i + 2 * q]);
        int32x8 x3 = int32x8_load(&x[i + 3 * q]);
        int32x8 x4 = int32x8_load(&x[i + 4 * q]);
        int32x8 x5 = int32x8_load(&x[i + 5 * q]);
        int32x8 x6 = int32x8_load(&x[i + 6 * q]);
        int32x8 x7 = int32x8_load(&x[i + 7 * q]);

        int32x8_MINMAX(x0, x1);
        int32x8_MINMAX(x2, x3);
        int32x8_MINMAX(x4, x5);
        int32x8_MINMAX(x6, x7);
        int32x8_MINMAX(x0, x2);
        int32x8_MINMAX(x1, x3);
        int32x8_MINMAX(x4, x6);
        int32x8_MINMAX(x5, x7);
        int32x8_MINMAX(x0, x4);
        int32x8_MINMAX(x1, x5);
        int32x8_MINMAX(x2, x6);
        int32x8_MINMAX(x3, x7);

        int32x8 b0 = _mm256_unpacklo_epi32(x0, x4);
        int32x8 b1 = _mm256_unpackhi_epi32(x0, x4);
        int32x8 b2 = _mm256_unpacklo_epi32(x1, x5);
        int32x8 b3 = _mm256_unpackhi_epi32(x1, x5);
        int32x8 b4 = _mm256_unpacklo_epi32(x2, x6);
        int32x8 b5 = _mm256_unpackhi_epi32(x2, x6);
        int32x8 b6 = _mm256_unpacklo_epi32(x3, x7);
        int32x8 b7 = _mm256_unpackhi_epi32(x3, x7);

        int32x8 c0 = _mm256_unpacklo_epi64(b0, b4);
        int32x8 c1 = _mm256_unpacklo_epi64(b1, b5);
        int32x8 c2 = _mm256_unpackhi_epi64(b0, b4);
        int32x8 c3 = _mm256_unpackhi_epi64(b1, b5);
        int32x8 c4 = _mm256_unpacklo_epi64(b2, b6);
        int32x8 c5 = _mm256_unpacklo_epi64(b3, b7);
        int32x8 c6 = _mm256_unpackhi_epi64(b2, b6);
        int32x8 c7 = _mm256_unpackhi_epi64(b3, b7);

        int32x8 d0 = _mm256_permute2x128_si256(c0, c4, 0x20);
        int32x8 d1 = _mm256_permute2x128_si256(c1, c5, 0x20);
        int32x8 d2 = _mm256_permute2x128_si256(c2, c6, 0x20);
        int32x8 d3 = _mm256_permute2x128_si256(c3, c7, 0x20);
        int32x8 d4 = _mm256_permute2x128_si256(c0, c4, 0x31);
        int32x8 d5 = _mm256_permute2x128_si256(c1, c5, 0x31);
        int32x8 d6 = _mm256_permute2x128_si256(c2, c6, 0x31);
        int32x8 d7 = _mm256_permute2x128_si256(c3, c7, 0x31);

        if (flagdown) {
            d0 = _mm256_xor_si256(d0, mask);
            d1 = _mm256_xor_si256(d1, mask);
            d2 = _mm256_xor_si256(d2, mask);
            d3 = _mm256_xor_si256(d3, mask);
            d4 = _mm256_xor_si256(d4, mask);
            d5 = _mm256_xor_si256(d5, mask);
            d6 = _mm256_xor_si256(d6, mask);
            d7 = _mm256_xor_si256(d7, mask);
        }

        int32x8_store(&x[i], d0);
        int32x8_store(&x[i + q], d4);
        int32x8_store(&x[i + 2 * q], d1);
        int32x8_store(&x[i + 3 * q], d5);
        int32x8_store(&x[i + 4 * q], d2);
        int32x8_store(&x[i + 5 * q], d6);
        int32x8_store(&x[i + 6 * q], d3);
        int32x8_store(&x[i + 7 * q], d7);
    }
}

void int32_sort(int32_t *input_output_array, uint64_t size_of_array) {
    uint64_t q, i, j;

    if (size_of_array <= 8) {
        if (size_of_array == 8) {
            int32_MINMAX(input_output_array[0], input_output_array[1]);
            int32_MINMAX(input_output_array[1], input_output_array[2]);
            int32_MINMAX(input_output_array[2], input_output_array[3]);
            int32_MINMAX(input_output_array[3], input_output_array[4]);
            int32_MINMAX(input_output_array[4], input_output_array[5]);
            int32_MINMAX(input_output_array[5], input_output_array[6]);
            int32_MINMAX(input_output_array[6], input_output_array[7]);
        }
        if (size_of_array >= 7) {
            int32_MINMAX(input_output_array[0], input_output_array[1]);
            int32_MINMAX(input_output_array[1], input_output_array[2]);
            int32_MINMAX(input_output_array[2], input_output_array[3]);
            int32_MINMAX(input_output_array[3], input_output_array[4]);
            int32_MINMAX(input_output_array[4], input_output_array[5]);
            int32_MINMAX(input_output_array[5], input_output_array[6]);
        }
        if (size_of_array >= 6) {
            int32_MINMAX(input_output_array[0], input_output_array[1]);
            int32_MINMAX(input_output_array[1], input_output_array[2]);
            int32_MINMAX(input_output_array[2], input_output_array[3]);
            int32_MINMAX(input_output_array[3], input_output_array[4]);
            int32_MINMAX(input_output_array[4], input_output_array[5]);
        }
        if (size_of_array >= 5) {
            int32_MINMAX(input_output_array[0], input_output_array[1]);
            int32_MINMAX(input_output_array[1], input_output_array[2]);
            int32_MINMAX(input_output_array[2], input_output_array[3]);
            int32_MINMAX(input_output_array[3], input_output_array[4]);
        }
        if (size_of_array >= 4) {
            int32_MINMAX(input_output_array[0], input_output_array[1]);
            int32_MINMAX(input_output_array[1], input_output_array[2]);
            int32_MINMAX(input_output_array[2], input_output_array[3]);
        }
        if (size_of_array >= 3) {
            int32_MINMAX(input_output_array[0], input_output_array[1]);
            int32_MINMAX(input_output_array[1], input_output_array[2]);
        }
        if (size_of_array >= 2) {
            int32_MINMAX(input_output_array[0], input_output_array[1]);
        }
        return;
    }

    if (!(size_of_array & (size_of_array - 1))) {
        int32_sort_2power(input_output_array, size_of_array, 0);
        return;
    }

    q = 8;
    while (q < size_of_array - q) q += q;
    /* size_of_array > q >= 8 */

    if (q <= 128) { /* size_of_array <= 256 */
        int32x8 y[32];
        for (i = q >> 3; i < q >> 2; ++i) y[i] = _mm256_set1_epi32(0x7fffffff);
        for (i = 0; i < size_of_array; ++i) i[(int32_t *)y] = input_output_array[i];
        int32_sort_2power((int32_t *)y, 2 * q, 0);
        for (i = 0; i < size_of_array; ++i) input_output_array[i] = i[(int32_t *)y];
        return;
    }

    int32_sort_2power(input_output_array, q, 1);
    int32_sort(input_output_array + q, size_of_array - q);

    while (q >= 64) {
        q >>= 2;
        j = int32_threestages(input_output_array, size_of_array, q);
        minmax_vector(input_output_array + j, input_output_array + j + 4 * q, size_of_array - 4 * q - j);
        if (j + 4 * q <= size_of_array) {
            for (i = j; i < j + q; i += 8) {
                int32x8 x0 = int32x8_load(&input_output_array[i]);
                int32x8 x1 = int32x8_load(&input_output_array[i + q]);
                int32x8 x2 = int32x8_load(&input_output_array[i + 2 * q]);
                int32x8 x3 = int32x8_load(&input_output_array[i + 3 * q]);
                int32x8_MINMAX(x0, x2);
                int32x8_MINMAX(x1, x3);
                int32x8_MINMAX(x0, x1);
                int32x8_MINMAX(x2, x3);
                int32x8_store(&input_output_array[i], x0);
                int32x8_store(&input_output_array[i + q], x1);
                int32x8_store(&input_output_array[i + 2 * q], x2);
                int32x8_store(&input_output_array[i + 3 * q], x3);
            }
            j += 4 * q;
        }
        minmax_vector(input_output_array + j, input_output_array + j + 2 * q, size_of_array - 2 * q - j);
        if (j + 2 * q <= size_of_array) {
            for (i = j; i < j + q; i += 8) {
                int32x8 x0 = int32x8_load(&input_output_array[i]);
                int32x8 x1 = int32x8_load(&input_output_array[i + q]);
                int32x8_MINMAX(x0, x1);
                int32x8_store(&input_output_array[i], x0);
                int32x8_store(&input_output_array[i + q], x1);
            }
            j += 2 * q;
        }
        minmax_vector(input_output_array + j, input_output_array + j + q, size_of_array - q - j);
        q >>= 1;
    }
    if (q == 32) {
        j = 0;
        for (; j + 64 <= size_of_array; j += 64) {
            int32x8 x0 = int32x8_load(&input_output_array[j]);
            int32x8 x1 = int32x8_load(&input_output_array[j + 8]);
            int32x8 x2 = int32x8_load(&input_output_array[j + 16]);
            int32x8 x3 = int32x8_load(&input_output_array[j + 24]);
            int32x8 x4 = int32x8_load(&input_output_array[j + 32]);
            int32x8 x5 = int32x8_load(&input_output_array[j + 40]);
            int32x8 x6 = int32x8_load(&input_output_array[j + 48]);
            int32x8 x7 = int32x8_load(&input_output_array[j + 56]);
            int32x8_MINMAX(x0, x4);
            int32x8_MINMAX(x1, x5);
            int32x8_MINMAX(x2, x6);
            int32x8_MINMAX(x3, x7);
            int32x8_MINMAX(x0, x2);
            int32x8_MINMAX(x1, x3);
            int32x8_MINMAX(x4, x6);
            int32x8_MINMAX(x5, x7);
            int32x8_MINMAX(x0, x1);
            int32x8_MINMAX(x2, x3);
            int32x8_MINMAX(x4, x5);
            int32x8_MINMAX(x6, x7);
            int32x8 a0 = _mm256_permute2x128_si256(x0, x1, 0x20);
            int32x8 a1 = _mm256_permute2x128_si256(x0, x1, 0x31);
            int32x8 a2 = _mm256_permute2x128_si256(x2, x3, 0x20);
            int32x8 a3 = _mm256_permute2x128_si256(x2, x3, 0x31);
            int32x8 a4 = _mm256_permute2x128_si256(x4, x5, 0x20);
            int32x8 a5 = _mm256_permute2x128_si256(x4, x5, 0x31);
            int32x8 a6 = _mm256_permute2x128_si256(x6, x7, 0x20);
            int32x8 a7 = _mm256_permute2x128_si256(x6, x7, 0x31);
            int32x8_MINMAX(a0, a1);
            int32x8_MINMAX(a2, a3);
            int32x8_MINMAX(a4, a5);
            int32x8_MINMAX(a6, a7);
            int32x8 b0 = _mm256_permute2x128_si256(a0, a1, 0x20);
            int32x8 b1 = _mm256_permute2x128_si256(a0, a1, 0x31);
            int32x8 b2 = _mm256_permute2x128_si256(a2, a3, 0x20);
            int32x8 b3 = _mm256_permute2x128_si256(a2, a3, 0x31);
            int32x8 b4 = _mm256_permute2x128_si256(a4, a5, 0x20);
            int32x8 b5 = _mm256_permute2x128_si256(a4, a5, 0x31);
            int32x8 b6 = _mm256_permute2x128_si256(a6, a7, 0x20);
            int32x8 b7 = _mm256_permute2x128_si256(a6, a7, 0x31);
            int32x8 c0 = _mm256_unpacklo_epi64(b0, b1);
            int32x8 c1 = _mm256_unpackhi_epi64(b0, b1);
            int32x8 c2 = _mm256_unpacklo_epi64(b2, b3);
            int32x8 c3 = _mm256_unpackhi_epi64(b2, b3);
            int32x8 c4 = _mm256_unpacklo_epi64(b4, b5);
            int32x8 c5 = _mm256_unpackhi_epi64(b4, b5);
            int32x8 c6 = _mm256_unpacklo_epi64(b6, b7);
            int32x8 c7 = _mm256_unpackhi_epi64(b6, b7);
            int32x8_MINMAX(c0, c1);
            int32x8_MINMAX(c2, c3);
            int32x8_MINMAX(c4, c5);
            int32x8_MINMAX(c6, c7);
            int32x8 d0 = _mm256_unpacklo_epi32(c0, c1);
            int32x8 d1 = _mm256_unpackhi_epi32(c0, c1);
            int32x8 d2 = _mm256_unpacklo_epi32(c2, c3);
            int32x8 d3 = _mm256_unpackhi_epi32(c2, c3);
            int32x8 d4 = _mm256_unpacklo_epi32(c4, c5);
            int32x8 d5 = _mm256_unpackhi_epi32(c4, c5);
            int32x8 d6 = _mm256_unpacklo_epi32(c6, c7);
            int32x8 d7 = _mm256_unpackhi_epi32(c6, c7);
            int32x8 e0 = _mm256_unpacklo_epi64(d0, d1);
            int32x8 e1 = _mm256_unpackhi_epi64(d0, d1);
            int32x8 e2 = _mm256_unpacklo_epi64(d2, d3);
            int32x8 e3 = _mm256_unpackhi_epi64(d2, d3);
            int32x8 e4 = _mm256_unpacklo_epi64(d4, d5);
            int32x8 e5 = _mm256_unpackhi_epi64(d4, d5);
            int32x8 e6 = _mm256_unpacklo_epi64(d6, d7);
            int32x8 e7 = _mm256_unpackhi_epi64(d6, d7);
            int32x8_MINMAX(e0, e1);
            int32x8_MINMAX(e2, e3);
            int32x8_MINMAX(e4, e5);
            int32x8_MINMAX(e6, e7);
            int32x8 f0 = _mm256_unpacklo_epi32(e0, e1);
            int32x8 f1 = _mm256_unpackhi_epi32(e0, e1);
            int32x8 f2 = _mm256_unpacklo_epi32(e2, e3);
            int32x8 f3 = _mm256_unpackhi_epi32(e2, e3);
            int32x8 f4 = _mm256_unpacklo_epi32(e4, e5);
            int32x8 f5 = _mm256_unpackhi_epi32(e4, e5);
            int32x8 f6 = _mm256_unpacklo_epi32(e6, e7);
            int32x8 f7 = _mm256_unpackhi_epi32(e6, e7);
            int32x8_store(&input_output_array[j], f0);
            int32x8_store(&input_output_array[j + 8], f1);
            int32x8_store(&input_output_array[j + 16], f2);
            int32x8_store(&input_output_array[j + 24], f3);
            int32x8_store(&input_output_array[j + 32], f4);
            int32x8_store(&input_output_array[j + 40], f5);
            int32x8_store(&input_output_array[j + 48], f6);
            int32x8_store(&input_output_array[j + 56], f7);
        }
        minmax_vector(input_output_array + j, input_output_array + j + 32, size_of_array - 32 - j);
        goto continue16;
    }
    if (q == 16) {
        j = 0;
    continue16:
        for (; j + 32 <= size_of_array; j += 32) {
            int32x8 x0 = int32x8_load(&input_output_array[j]);
            int32x8 x1 = int32x8_load(&input_output_array[j + 8]);
            int32x8 x2 = int32x8_load(&input_output_array[j + 16]);
            int32x8 x3 = int32x8_load(&input_output_array[j + 24]);
            int32x8_MINMAX(x0, x2);
            int32x8_MINMAX(x1, x3);
            int32x8_MINMAX(x0, x1);
            int32x8_MINMAX(x2, x3);
            int32x8 a0 = _mm256_permute2x128_si256(x0, x1, 0x20);
            int32x8 a1 = _mm256_permute2x128_si256(x0, x1, 0x31);
            int32x8 a2 = _mm256_permute2x128_si256(x2, x3, 0x20);
            int32x8 a3 = _mm256_permute2x128_si256(x2, x3, 0x31);
            int32x8_MINMAX(a0, a1);
            int32x8_MINMAX(a2, a3);
            int32x8 b0 = _mm256_permute2x128_si256(a0, a1, 0x20);
            int32x8 b1 = _mm256_permute2x128_si256(a0, a1, 0x31);
            int32x8 b2 = _mm256_permute2x128_si256(a2, a3, 0x20);
            int32x8 b3 = _mm256_permute2x128_si256(a2, a3, 0x31);
            int32x8 c0 = _mm256_unpacklo_epi64(b0, b1);
            int32x8 c1 = _mm256_unpackhi_epi64(b0, b1);
            int32x8 c2 = _mm256_unpacklo_epi64(b2, b3);
            int32x8 c3 = _mm256_unpackhi_epi64(b2, b3);
            int32x8_MINMAX(c0, c1);
            int32x8_MINMAX(c2, c3);
            int32x8 d0 = _mm256_unpacklo_epi32(c0, c1);
            int32x8 d1 = _mm256_unpackhi_epi32(c0, c1);
            int32x8 d2 = _mm256_unpacklo_epi32(c2, c3);
            int32x8 d3 = _mm256_unpackhi_epi32(c2, c3);
            int32x8 e0 = _mm256_unpacklo_epi64(d0, d1);
            int32x8 e1 = _mm256_unpackhi_epi64(d0, d1);
            int32x8 e2 = _mm256_unpacklo_epi64(d2, d3);
            int32x8 e3 = _mm256_unpackhi_epi64(d2, d3);
            int32x8_MINMAX(e0, e1);
            int32x8_MINMAX(e2, e3);
            int32x8 f0 = _mm256_unpacklo_epi32(e0, e1);
            int32x8 f1 = _mm256_unpackhi_epi32(e0, e1);
            int32x8 f2 = _mm256_unpacklo_epi32(e2, e3);
            int32x8 f3 = _mm256_unpackhi_epi32(e2, e3);
            int32x8_store(&input_output_array[j], f0);
            int32x8_store(&input_output_array[j + 8], f1);
            int32x8_store(&input_output_array[j + 16], f2);
            int32x8_store(&input_output_array[j + 24], f3);
        }
        minmax_vector(input_output_array + j, input_output_array + j + 16, size_of_array - 16 - j);
        goto continue8;
    }
    /* q == 8 */
    j = 0;
continue8:
    for (; j + 16 <= size_of_array; j += 16) {
        int32x8 x0 = int32x8_load(&input_output_array[j]);
        int32x8 x1 = int32x8_load(&input_output_array[j + 8]);
        int32x8_MINMAX(x0, x1);
        int32x8_store(&input_output_array[j], x0);
        int32x8_store(&input_output_array[j + 8], x1);
        int32x8 a0 = _mm256_permute2x128_si256(x0, x1, 0x20);
        int32x8 a1 = _mm256_permute2x128_si256(x0, x1, 0x31);
        int32x8_MINMAX(a0, a1);
        int32x8 b0 = _mm256_permute2x128_si256(a0, a1, 0x20);
        int32x8 b1 = _mm256_permute2x128_si256(a0, a1, 0x31);
        int32x8 c0 = _mm256_unpacklo_epi64(b0, b1);
        int32x8 c1 = _mm256_unpackhi_epi64(b0, b1);
        int32x8_MINMAX(c0, c1);
        int32x8 d0 = _mm256_unpacklo_epi32(c0, c1);
        int32x8 d1 = _mm256_unpackhi_epi32(c0, c1);
        int32x8 e0 = _mm256_unpacklo_epi64(d0, d1);
        int32x8 e1 = _mm256_unpackhi_epi64(d0, d1);
        int32x8_MINMAX(e0, e1);
        int32x8 f0 = _mm256_unpacklo_epi32(e0, e1);
        int32x8 f1 = _mm256_unpackhi_epi32(e0, e1);
        int32x8_store(&input_output_array[j], f0);
        int32x8_store(&input_output_array[j + 8], f1);
    }
    minmax_vector(input_output_array + j, input_output_array + j + 8, size_of_array - 8 - j);
    if (j + 8 <= size_of_array) {
        int32_MINMAX(input_output_array[j], input_output_array[j + 4]);
        int32_MINMAX(input_output_array[j + 1], input_output_array[j + 5]);
        int32_MINMAX(input_output_array[j + 2], input_output_array[j + 6]);
        int32_MINMAX(input_output_array[j + 3], input_output_array[j + 7]);
        int32_MINMAX(input_output_array[j], input_output_array[j + 2]);
        int32_MINMAX(input_output_array[j + 1], input_output_array[j + 3]);
        int32_MINMAX(input_output_array[j], input_output_array[j + 1]);
        int32_MINMAX(input_output_array[j + 2], input_output_array[j + 3]);
        int32_MINMAX(input_output_array[j + 4], input_output_array[j + 6]);
        int32_MINMAX(input_output_array[j + 5], input_output_array[j + 7]);
        int32_MINMAX(input_output_array[j + 4], input_output_array[j + 5]);
        int32_MINMAX(input_output_array[j + 6], input_output_array[j + 7]);
        j += 8;
    }
    minmax_vector(input_output_array + j, input_output_array + j + 4, size_of_array - 4 - j);
    if (j + 4 <= size_of_array) {
        int32_MINMAX(input_output_array[j], input_output_array[j + 2]);
        int32_MINMAX(input_output_array[j + 1], input_output_array[j + 3]);
        int32_MINMAX(input_output_array[j], input_output_array[j + 1]);
        int32_MINMAX(input_output_array[j + 2], input_output_array[j + 3]);
        j += 4;
    }
    if (j + 3 <= size_of_array) int32_MINMAX(input_output_array[j], input_output_array[j + 2]);
    if (j + 2 <= size_of_array) int32_MINMAX(input_output_array[j], input_output_array[j + 1]);
}

void uint32_sort(uint32_t *input_output_array, uint64_t size_of_array) {
    uint64_t j;
    for (j = 0; j < size_of_array; ++j) input_output_array[j] ^= 0x80000000;
    int32_sort((int32_t *)input_output_array, size_of_array);
    for (j = 0; j < size_of_array; ++j) input_output_array[j] ^= 0x80000000;
}
