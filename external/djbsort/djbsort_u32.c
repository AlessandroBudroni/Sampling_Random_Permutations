
#include "djbsort.h"

void uint32_sort(uint32_t *x, uint64_t n) {
    uint64_t j;
    for (j = 0; j < n; ++j) x[j] ^= 0x80000000;
    int32_sort((int32_t *)x, n);
    for (j = 0; j < n; ++j) x[j] ^= 0x80000000;
}
