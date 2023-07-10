//
// Created by Alessandro Budroni on 10/10/2022.
//

#include "fisher_yates_natural.h"
#include "../common.h"
#include <stdlib.h>
#include <string.h>

static int set_random_with_bound_for_permutation_natural(perm_t p, const uint16_t rnd_buff[CHUNK_RND_U16_LENGTH]) {

    int size = PARAM_N;
    uint16_t rnd;
    int32_t index = 0;
    uint32_t max;
    int bound;

    for (int i = 0; i < size; i++) {
        bound = size - i;
        max = ((0x10000 / bound ) * bound);// (2^16 / bound) * bound
        do {
            if (index >= CHUNK_RND_U16_LENGTH -1){
                return EXIT_FAILURE;
            }
            rnd = rnd_buff[index++];
        } while (rnd >= max);
        p[i] = (rnd % bound);
    }
    return EXIT_SUCCESS;
}

static void fisher_yates_shuffle_natural(perm_t p_out, perm_t p_rand) {

    uint16_t pi, *pj;
    uint16_t tmp;
    uint16_t mask;
    for (uint16_t i = 0; i < PARAM_N; i++) {
        pi = p_rand[i];
        tmp = i;
        for (uint16_t j = 0; j < i; j++) {
            pj = &p_rand[j];
            mask = GT16(*pj, pi);
            tmp -= mask;
            *pj -= mask;
        }
        p_out[i] = pi + tmp;
    }
}

void perm_set_random_natural(perm_t p_out, uint8_t seed[SEED_BYTES]) {
    uint16_t rnd_buff[CHUNK_RND_U16_LENGTH];
    uint8_t expanded_seed[SEED_BYTES + 2];
    perm_t p_rand;
    memcpy(expanded_seed, seed, SEED_BYTES);
    expanded_seed[SEED_BYTES] = DOMAIN_SEPARATOR_PERM;
    expanded_seed[SEED_BYTES + 1] = 0;
    sample_random_chunk((uint8_t *)rnd_buff, expanded_seed);

    while (set_random_with_bound_for_permutation_natural(p_rand, rnd_buff) != EXIT_SUCCESS) {
        expanded_seed[SEED_BYTES + 1] += 1;
        sample_random_chunk((uint8_t *)rnd_buff, expanded_seed);
    }
    fisher_yates_shuffle_natural(p_out, p_rand);
}