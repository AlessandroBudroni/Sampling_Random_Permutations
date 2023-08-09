// =============================================
// ==    Classification: TII CONFIDENTIAL     ==
// =============================================

#include "djbsort.h"

#ifdef __x86_64__
#define int32_MINMAX(a,b) \
do { \
  int32_t temp1; \
  asm( \
    "cmpl %1,%0\n\t" \
    "mov %0,%2\n\t" \
    "cmovg %1,%0\n\t" \
    "cmovg %2,%1\n\t" \
    : "+r"(a), "+r"(b), "=r"(temp1) \
    : \
    : "cc" \
  ); \
} while(0)
#else
#define int32_MINMAX(a,b) \
do { \
  int32_t ab = b ^ a; \
  int32_t c = b - a; \
  c ^= ab & (c ^ b); \
  c >>= 31; \
  c &= ab; \
  a ^= c; \
  b ^= c; \
} while(0)
#endif

void int32_sort(int32_t *x, uint64_t n)
{
    long long top,p,q,r,i,j;

    if (n < 2) return;
    top = 1;
    while (top < n - top) top += top;

    for (p = top;p >= 1;p >>= 1) {
        i = 0;
        while (i + 2 * p <= n) {
            for (j = i;j < i + p;++j)
                int32_MINMAX(x[j],x[j+p]);
            i += 2 * p;
        }
        for (j = i;j < n - p;++j)
            int32_MINMAX(x[j],x[j+p]);

        i = 0;
        j = 0;
        for (q = top;q > p;q >>= 1) {
            if (j != i) for (;;) {
                    if (j == n - q) goto done;
                    int32_t a = x[j + p];
                    for (r = q;r > p;r >>= 1)
                        int32_MINMAX(a,x[j + r]);
                    x[j + p] = a;
                    ++j;
                    if (j == i + p) {
                        i += 2 * p;
                        break;
                    }
                }
            while (i + p <= n - q) {
                for (j = i;j < i + p;++j) {
                    int32_t a = x[j + p];
                    for (r = q;r > p;r >>= 1)
                        int32_MINMAX(a,x[j+r]);
                    x[j + p] = a;
                }
                i += 2 * p;
            }
            /* now i + p > n - q */
            j = i;
            while (j < n - q) {
                int32_t a = x[j + p];
                for (r = q;r > p;r >>= 1)
                    int32_MINMAX(a,x[j+r]);
                x[j + p] = a;
                ++j;
            }

            done: ;
        }
    }
}

