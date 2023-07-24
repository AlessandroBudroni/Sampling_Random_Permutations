//
// Created by Alessandro Budroni on 28/10/2022.
//

#ifndef RANDOM_PERMUTATION_SAMPLING_DJBSORT_SAMPLE_H
#define RANDOM_PERMUTATION_SAMPLING_DJBSORT_SAMPLE_H

#include "../../test/test_utils.h"
#include "fisher_yates.h"

void perm_set_random_djbsort(perm_t p, uint8_t seed[SEED_BYTES]);

#endif //RANDOM_PERMUTATION_SAMPLING_DJBSORT_SAMPLE_H
