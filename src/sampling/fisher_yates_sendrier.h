//
// Created by Alessandro Budroni on 06/07/2023.
//

#ifndef RANDOM_PERMUTATION_SAMPLING_FISHER_YATES_SENDRIER_H
#define RANDOM_PERMUTATION_SAMPLING_FISHER_YATES_SENDRIER_H

#include "../definitions.h"

void perm_set_random_sendrier(perm_t p, uint8_t seed[SEED_BYTES]);
void perm_set_random_sendrier_ternary_operator(perm_t p, uint8_t seed[SEED_BYTES]);

#endif //RANDOM_PERMUTATION_SAMPLING_FISHER_YATES_SENDRIER_H
