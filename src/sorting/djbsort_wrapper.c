//
// Created by perin on 7/27/23.
//

#include "generic_sort.h"

#include <djbsort/djbsort.h>

void common_sort_uint32(uint32_t *input_output_array, uint64_t size_of_array) {
    uint32_sort(input_output_array, size_of_array);
}

void common_sort_uint64(uint64_t *input_output_array, uint64_t size_of_array) {
    uint64_sort(input_output_array, size_of_array);
}
