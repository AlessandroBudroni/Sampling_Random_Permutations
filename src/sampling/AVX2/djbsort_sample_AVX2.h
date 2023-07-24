//
// Created by Alessandro Budroni on 28/10/2022.
//

#ifndef RANDOM_PERMUTATION_SAMPLING_DJBSORT_SAMPLE_AVX2_H
#define RANDOM_PERMUTATION_SAMPLING_DJBSORT_SAMPLE_AVX2_H

#include "../../../test/test_utils.h"
#include "../fisher_yates.h"

void perm_set_random_djbsort_avx2(perm_t p, uint8_t seed[16]);

#endif //RANDOM_PERMUTATION_SAMPLING_DJBSORT_SAMPLE_AVX2_H
