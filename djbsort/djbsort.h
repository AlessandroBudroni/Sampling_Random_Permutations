
#ifndef DJB_SORT_H
#define DJB_SORT_H

#include <stdint.h>

void uint16_sort(uint16_t *input_output_array, uint64_t size_of_array);

void int32_sort(int32_t *input_output_array, uint64_t size_of_array);

void uint32_sort_avx(uint32_t *input_output_array, uint64_t size_of_array);

void uint32_sort(uint32_t *input_output_array, uint64_t size_of_array);

void uint64_sort(uint64_t *input_output_array, uint64_t size_of_array);

#endif
