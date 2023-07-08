//
// Created by Alessandro Budroni on 08/07/2023.
//

#ifndef RANDOM_PERMUTATION_SAMPLING_FISHER_YATES_SENDRIER_AVX2_H
#define RANDOM_PERMUTATION_SAMPLING_FISHER_YATES_SENDRIER_AVX2_H

#include "../definitions.h"
#include "definitions_AVX2.h"

void perm_set_random_sendrier_avx2(permAVX_t *p, uint8_t seed[SEED_BYTES]);


#endif //RANDOM_PERMUTATION_SAMPLING_FISHER_YATES_SENDRIER_AVX2_H
