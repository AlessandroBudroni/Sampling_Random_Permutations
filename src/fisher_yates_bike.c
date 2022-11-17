//
// Created by Alessandro Budroni on 15/11/2022.
//

#include <stdint.h>
#include <string.h>
#include "../include/definitions.h"
#include "../include/utils.h"
#include "../include/fisher_yates.h"

void fisher_yates_shuffle_bike(perm_t pout, const perm_t pin) {

    for (size_t i = PARAM_N1; i-- > 0;) {
        uint32_t l = pin[i];

        // Loop over (the end of) the output array to determine if l is a duplicate
        uint32_t is_dup = 0;
        for (size_t j = i + 1; j < PARAM_N1; ++j) {
            is_dup ^= secure_cmp32(l, pout[j]);
        }
        uint32_t mask =- is_dup;
        pout[i] = (mask & i) ^ (~mask & l);
    }
}

void perm_set_random_bike(perm_t pout, uint8_t seed[SEED_BYTES]) {
    uint16_t rnd_buff[CHUNK_RND_U16_LENGTH];
    uint8_t expanded_seed[SEED_BYTES + 2];

    memcpy(expanded_seed, seed, SEED_BYTES);
    expanded_seed[SEED_BYTES] = DOMAIN_SEPARATOR_PERM;
    expanded_seed[SEED_BYTES + 1] = 0;

    perm_t prand;
    sample_random_chunk((uint8_t *)rnd_buff, expanded_seed);

    while (set_random_with_bound_for_permutation(prand, rnd_buff) != EXIT_SUCCESS) {
        expanded_seed[SEED_BYTES + 1] += 1;
        sample_random_chunk((uint8_t *)rnd_buff, expanded_seed);
    }
    fisher_yates_shuffle_bike(pout, prand);
    memset(rnd_buff, 0, sizeof(rnd_buff));
}


