//
// Created by Alessandro Budroni on 07/09/2022.
//

#include "../include/fisher_yates.h"
#include "../include/utils.h"
#include "../fips202/fips202.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void fisher_yates_non_constant_time(perm_t p, const perm_t r) {
    uint16_t tmp;
    for (int i = 0; i < PARAM_N1; ++i) {
        p[i] = i;
    }
    for (int16_t i = PARAM_N1 - 1; i >= 0; --i) {
        tmp = p[i];
        p[i] = p[r[i]];
        p[r[i]] = tmp;
    }
}

int set_random_with_bound_for_permutation_non_ct(perm_t p, const uint16_t rnd_buff[CHUNK_RND_U16_LENGTH]) {

    int size = PARAM_N1;
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

void sample_random_chunk( uint8_t rnd_buff[CHUNK_RND_BYTES_LENGTH], uint8_t expanded_seed[SEED_BYTES + 2]){
    shake128((uint8_t *)rnd_buff, CHUNK_RND_BYTES_LENGTH, expanded_seed, SEED_BYTES + 2);
}


void perm_set_random_non_ct(perm_t p, uint8_t seed[SEED_BYTES]) {
    uint16_t rnd_buff[CHUNK_RND_U16_LENGTH];
    uint8_t expanded_seed[SEED_BYTES + 2];

    perm_t rand;

    memcpy(expanded_seed, seed, SEED_BYTES);
    expanded_seed[SEED_BYTES] = DOMAIN_SEPARATOR_PERM;
    expanded_seed[SEED_BYTES + 1] = 0;

    sample_random_chunk((uint8_t *)rnd_buff, expanded_seed);

    while (set_random_with_bound_for_permutation_non_ct(rand, rnd_buff) != EXIT_SUCCESS) {
        expanded_seed[SEED_BYTES + 1] += 1;
        sample_random_chunk((uint8_t *)rnd_buff, expanded_seed);
    }
    fisher_yates_non_constant_time(p, rand);
    memset(rand, 0, sizeof (perm_t));
}

int verify_permutation(const perm_t p) {

    uint16_t verification[PARAM_N1] = {0};

    for (int i = 0; i < PARAM_N1; ++i) {
        if (p[i] > PARAM_N1){
            printf("Verification failure with index %d and value %d\n", i, p[i]);
            return EXIT_FAILURE;
        }
        verification[p[i]]++;
    }

    for (int i = 0; i < PARAM_N1; ++i) {
        if (verification[i] != 1) {
            printf("Verification failure at %d rep %d\n", i, p[i]);
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}
