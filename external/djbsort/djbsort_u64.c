
#include "djbsort.h"

void uint64_sort(uint64_t *x, uint64_t n) {
    uint64_t j;
    for (j = 0; j < n; ++j) x[j] ^= 0x8000000000000000;
    int64_sort((int64_t *)x, n);
    for (j = 0; j < n; ++j) x[j] ^= 0x8000000000000000;
}

