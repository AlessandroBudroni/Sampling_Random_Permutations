
#include "djbsort.h"

#define int64_MINMAX(a,b) \
do { \
  int64_t ab = b ^ a; \
  int64_t c = b - a; \
  c ^= ab & (c ^ b); \
  c >>= 63; \
  c &= ab; \
  a ^= c; \
  b ^= c; \
} while(0)

void int64_sort(int64_t *x, uint64_t n)
{
    long long top,p,q,r,i,j;

    if (n < 2) return;
    top = 1;
    while (top < n - top) top += top;

    for (p = top;p >= 1;p >>= 1) {
        i = 0;
        while (i + 2 * p <= n) {
            for (j = i;j < i + p;++j)
                int64_MINMAX(x[j],x[j+p]);
            i += 2 * p;
        }
        for (j = i;j < n - p;++j)
            int64_MINMAX(x[j],x[j+p]);

        i = 0;
        j = 0;
        for (q = top;q > p;q >>= 1) {
            if (j != i) for (;;) {
                    if (j == n - q) goto done;
                    int64_t a = x[j + p];
                    for (r = q;r > p;r >>= 1)
                        int64_MINMAX(a,x[j + r]);
                    x[j + p] = a;
                    ++j;
                    if (j == i + p) {
                        i += 2 * p;
                        break;
                    }
                }
            while (i + p <= n - q) {
                for (j = i;j < i + p;++j) {
                    int64_t a = x[j + p];
                    for (r = q;r > p;r >>= 1)
                        int64_MINMAX(a,x[j+r]);
                    x[j + p] = a;
                }
                i += 2 * p;
            }
            /* now i + p > n - q */
            j = i;
            while (j < n - q) {
                int64_t a = x[j + p];
                for (r = q;r > p;r >>= 1)
                    int64_MINMAX(a,x[j+r]);
                x[j + p] = a;
                ++j;
            }

            done: ;
        }
    }
}
