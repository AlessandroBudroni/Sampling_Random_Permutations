#ifndef RANDOM_PERMUTATION_SAMPLING_VQSORT_CWRAPPER_H
#define RANDOM_PERMUTATION_SAMPLING_VQSORT_CWRAPPER_H

#include <cstddef>
#include <cstdint>

extern "C" void vqsort_avx2_u32_asc(uint32_t* data, size_t len);
extern "C" void vqsort_avx2_u64_asc(uint64_t* data, size_t len);

#endif //RANDOM_PERMUTATION_SAMPLING_VQSORT_CWRAPPER_H