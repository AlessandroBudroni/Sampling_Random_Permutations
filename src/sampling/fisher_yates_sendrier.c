//
// Created by Alessandro Budroni on 16/11/2022.
//

#include "fisher_yates_sendrier.h"
#include "../common.h"

#include <stdlib.h>
#include <string.h>

static void fisher_yates_shuffle_sendrier(perm_t p) {

    for (int i = PARAM_N - 1; i >= 0; --i) {
        uint16_t pi = p[i];
        for (int j = i + 1; j < PARAM_N; ++j) { // p[j] = (p[j] == p[i]) ? i : p[j];
            int mask = ISZERO(p[j] - pi);
            p[j] = MASKAPPLY(mask-1, i, p[j]);
        }
    }
}

void perm_set_random_sendrier(perm_t p, uint8_t seed[SEED_BYTES]) {
    uint16_t rnd_buff[CHUNK_RND_U16_LENGTH];
    uint8_t expanded_seed[SEED_BYTES + 2];

    memcpy(expanded_seed, seed, SEED_BYTES);
    expanded_seed[SEED_BYTES] = DOMAIN_SEPARATOR_PERM;
    expanded_seed[SEED_BYTES + 1] = 0;

    sample_random_chunk((uint8_t *)rnd_buff, expanded_seed);

    while (set_random_with_bound_for_fisher_yates(p, rnd_buff) != EXIT_SUCCESS) {
        expanded_seed[SEED_BYTES + 1] += 1;
        sample_random_chunk((uint8_t *)rnd_buff, expanded_seed);
    }
    fisher_yates_shuffle_sendrier(p);
    memset(rnd_buff, 0, sizeof(rnd_buff));
}
