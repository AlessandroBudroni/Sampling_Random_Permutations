//
// Created by Alessandro Budroni on 07/09/2022.
//

#include "../common_fisher_yates.h"
#include "../api.h"

#include <string.h>

static void fisher_yates_non_constant_time(perm_t p, perm_t r) {
    for (int i = PARAM_N - 1; i >= 0; --i){
        p[i] = p[r[i]];
        p[r[i]] = i;
    }
}

void perm_set_random(perm_t p, uint8_t seed[SEED_BYTES]) {
    uint16_t rnd_buff[CHUNK_RND_U16_LENGTH];
    uint8_t expanded_seed[SEED_BYTES + 2];

    perm_t rand;

    memcpy(expanded_seed, seed, SEED_BYTES);
    expanded_seed[SEED_BYTES] = DOMAIN_SEPARATOR_PERM;
    expanded_seed[SEED_BYTES + 1] = 0;

    sample_random_chunk((uint8_t *)rnd_buff, expanded_seed);

    while (set_random_with_bound_for_fisher_yates(rand, rnd_buff) != EXIT_SUCCESS) {
        expanded_seed[SEED_BYTES + 1] += 1;
        sample_random_chunk((uint8_t *)rnd_buff, expanded_seed);
    }
    fisher_yates_non_constant_time(p, rand);
}


