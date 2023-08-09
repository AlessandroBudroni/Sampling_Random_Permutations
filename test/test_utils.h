//
// Created by Alessandro Budroni on 06/07/2023.
//

#ifndef RANDOM_PERMUTATION_SAMPLING_VERIFICATION_H
#define RANDOM_PERMUTATION_SAMPLING_VERIFICATION_H

#endif //RANDOM_PERMUTATION_SAMPLING_VERIFICATION_H

#include "../src/definitions.h"

#ifndef SCHEME_NAME
#define SCHEME_NAME "generic"
#endif

static inline char *scheme_name() {
    return SCHEME_NAME;
}

void get_seed(uint8_t seed[SEED_BYTES]);
int validate_permutation(const perm_t p);
int test_non_triviality(const perm_t p);
int test_equality(const perm_t p1, const perm_t p2);