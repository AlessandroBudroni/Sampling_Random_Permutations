
#include "djbsort.h"

#define uint32_MINMAX(a,b) \
do { \
  uint32_t c = b - a; \
  c >>= 31; \
  c = -c; \
  c &= a ^ b; \
  a ^= c; \
  b ^= c; \
} while(0)

void uint32_sort(uint32_t *x,uint64_t n)
{
    long long top,p,q,r,i;

    if (n < 2) return;
    top = 1;
    while (top < n - top) top += top;

    for (p = top;p > 0;p >>= 1) {
        for (i = 0;i < n - p;++i)
            if (!(i & p))
                uint32_MINMAX(x[i],x[i+p]);
        i = 0;
        for (q = top;q > p;q >>= 1) {
            for (;i < n - q;++i) {
                if (!(i & p)) {
                    uint32_t a = x[i + p];
                    for (r = q;r > p;r >>= 1)
                        uint32_MINMAX(a,x[i+r]);
                    x[i + p] = a;
                }
            }
        }
    }
}

