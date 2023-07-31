//
// Created by perin on 7/27/23.
//

#include "generic_sort.h"

#include <stddef.h>

extern void vqsort_avx2_u32_asc(uint32_t* data, size_t len);
extern void vqsort_avx2_u64_asc(uint64_t* data, size_t len);

void common_sort_uint32(uint32_t *input_output_array, uint64_t size_of_array) {
    vqsort_avx2_u32_asc(input_output_array, size_of_array);
}

void common_sort_uint64(uint64_t *input_output_array, uint64_t size_of_array) {
    vqsort_avx2_u64_asc(input_output_array, size_of_array);
}
