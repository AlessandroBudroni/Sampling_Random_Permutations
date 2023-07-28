// =============================================
// ==    Classification: TII CONFIDENTIAL     ==
// =============================================

#include "djbsort.h"

static inline void int32_sort_2_values(int32_t *input_1, int32_t *input_2) {
    int32_t input_1_xor_2 = *input_2 ^ *input_1;
    int32_t aux_2 = *input_2 - *input_1;
    aux_2 ^= input_1_xor_2 & (aux_2 ^ *input_2);
    aux_2 >>= 31;
    aux_2 &= input_1_xor_2;
    *input_1 ^= aux_2;
    *input_2 ^= aux_2;
}

void int32_sort(int32_t *input_output_array, uint64_t size_of_array) {
    uint64_t top, p, q, r, i;

    if (size_of_array < 2) return;
    top = 1;
    while (top < size_of_array - top) {
        top += top;
    }

    for (p = top; p > 0; p >>= 1) {
        for (i = 0; i < size_of_array - p; ++i) {
            if (!(i & p)) {
                int32_sort_2_values(&input_output_array[i], &input_output_array[i + p]);
            }
        }
        i = 0;
        for (q = top; q > p; q >>= 1) {
            for (; i < size_of_array - q; ++i) {
                if (!(i & p)) {
                    int32_t a = input_output_array[i + p];
                    for (r = q; r > p; r >>= 1) {
                        int32_sort_2_values(&a, &input_output_array[i + r]);
                    }
                    input_output_array[i + p] = a;
                }
            }
        }
    }
}

void uint32_sort(uint32_t *input_output_array, uint64_t size_of_array) {
    uint64_t j;
    for (j = 0; j < size_of_array; ++j) input_output_array[j] ^= 0x80000000;
    int32_sort((int32_t *)input_output_array, size_of_array);
    for (j = 0; j < size_of_array; ++j) input_output_array[j] ^= 0x80000000;
}
