//
// Created by Alessandro Budroni on 28/10/2022.
//

#include "djbsort_sample.h"
#include "../djbsort/djbsort.h"
#include "../fips202/fips202.h"
#include <stdint.h>
#include <string.h>

static int djbsort_with_given_random_input(perm_t p, uint32_t buffer[PARAM_N1]) {

    // Use 21 bits for randomness
    for (int i = 0; i < PARAM_N1; i++) {
        buffer[i] <<= 11;
        buffer[i] |= i;
    }

    // sort
    uint32_sort(buffer, PARAM_N1);

    // check that no double random values were produced
    for (int i = 1; i < PARAM_N1; i++) {
        if ((buffer[i - 1] >> 11) == (buffer[i] >> 11)) {
            return EXIT_FAILURE;
        }
    }

    // extract permutation from buffer
    for (int i = 0; i < PARAM_N1; i++) {
        p[i] = (uint16_t)(buffer[i] & 0x7FF);
    }
    return EXIT_SUCCESS;
}

/**
 * Apply fisher yates to sample permutation. This function should be called after
 * set_random_with_bound_for_permutation()
 * @param p permutation with random data already filled in
 * @param size of permutation
 * @return EXIT_SUCCESS for success, EXIT_FAILURE for failure in sampling
 */
void perm_set_random_djbsort(perm_t p, uint8_t seed[SEED_BYTES]) {
    uint32_t rnd_buff[PARAM_N1];
    uint8_t expanded_seed[SEED_BYTES + 2];

    memcpy(expanded_seed, seed, SEED_BYTES);
    expanded_seed[SEED_BYTES] = DOMAIN_SEPARATOR_PERM;
    expanded_seed[SEED_BYTES + 1] = 0;
    shake128((uint8_t *)rnd_buff, sizeof(rnd_buff), expanded_seed, sizeof(expanded_seed));

    while (djbsort_with_given_random_input(p, rnd_buff) != EXIT_SUCCESS) {
        expanded_seed[SEED_BYTES + 1] += 1;
        shake128((uint8_t *)rnd_buff, sizeof(rnd_buff), expanded_seed, sizeof(expanded_seed));
    }
    memset(rnd_buff, 0, sizeof(rnd_buff));
}

