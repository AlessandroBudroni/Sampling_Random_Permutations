
#include "djbsort.h"

static inline void uint64_sort_2_values(uint64_t *input_1, uint64_t *input_2) {
    uint64_t c = *input_2 - *input_1;
    c >>= 63;
    c = -c;
    c &= *input_1 ^ *input_2;
    *input_1 ^= c;
    *input_2 ^= c;
}

void uint64_sort(uint64_t *input_output_array, uint64_t size_of_array) {
    uint64_t top, p, q, r, i;

    if (size_of_array < 2) {
        return;
    }
    top = 1;
    while (top < size_of_array - top) {
        top += top;
    }

    for (p = top; p > 0; p >>= 1) {
        for (i = 0; i < size_of_array - p; ++i) {
            if (!(i & p)) {
                uint64_sort_2_values(&input_output_array[i], &input_output_array[i + p]);
            }
        }
        i = 0;
        for (q = top; q > p; q >>= 1) {
            for (; i < size_of_array - q; ++i) {
                if (!(i & p)) {
                    uint64_t a = input_output_array[i + p];
                    for (r = q; r > p; r >>= 1) uint64_sort_2_values(&a, &input_output_array[i + r]);
                    input_output_array[i + p] = a;
                }
            }
        }
    }
}
