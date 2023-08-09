#ifndef RANDOM_PERMUTATION_SAMPLING_COMMON_SHUFFLE_H
#define RANDOM_PERMUTATION_SAMPLING_COMMON_SHUFFLE_H

#include "../definitions.h"

typedef uint16_t uniform_indexes_t[PARAM_N] __attribute__((aligned(32)));

int common_sample_uniform_index_buffer(uniform_indexes_t output, const uint8_t seed[SEED_BYTES + 1]);

void common_shuffle(perm_t output, uniform_indexes_t input);

#endif //RANDOM_PERMUTATION_SAMPLING_COMMON_SHUFFLE_H
