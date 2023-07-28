#ifndef RANDOM_PERMUTATION_SAMPLING_API_H
#define RANDOM_PERMUTATION_SAMPLING_API_H

#include "definitions.h"

void perm_set_random(perm_t out, uint8_t seed[SEED_BYTES]);

void perm_compose(perm_t out, const perm_t in1, const perm_t in2);

void perm_inverse(perm_t out, const perm_t in);

#endif //RANDOM_PERMUTATION_SAMPLING_API_H
