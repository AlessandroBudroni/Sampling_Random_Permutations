//
// Created by Alessandro Budroni on 16/11/2022.
//

#include "../include/fisher_yates.h"
#include "../include/utils.h"
#include "../fips202/fips202.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static inline uint8_t isNotZero(uint16_t n){ // unsigned is safer for bit operations
    return ((n | (~n + 1)) >> 15) & 0x1;
}


#define ISNOTZERO(n) ((((n) | (~(n) + 1)) >> 15) & (uint8_t)0x1)
#define MASKAPPLY(mask, i, pj) (((mask) & (i)) | ((~(mask)) & (pj)))

//void fisher_yates_shuffle_sendrier(perm_t p) {
//    uint16_t mask;
//    uint16_t pi, *pj;
//    for (int16_t i = PARAM_N1 - 1; i >= 0; --i) {
//        pi = p[i];
//        for (int16_t j = i + 1; j < PARAM_N1; ++j) {  //p[j] = p[j] == p[i] ? i : p[j];
//            pj = &p[j];
//            mask = ISNOTZERO(*pj - pi) - 1;
//            *pj = MASKAPPLY(mask, i, *pj); //(mask & i) | (~mask & *pj);
//        }
//    }
//}

void fisher_yates_shuffle_sendrier(perm_t p) {
    uint16_t mask;
    uint16_t *pi;
    for (int16_t i = PARAM_N1 - 1; i >= 0; --i) {
        pi = &p[i];
        mask = 0;
        for (int16_t j = i + 1; j < PARAM_N1; ++j) {  //p[j] = p[j] == p[i] ? i : p[j];
            mask ^= 1-ISNOTZERO(p[j] - *pi);
        }
        mask = -mask;
        *pi = MASKAPPLY(mask, i, *pi);
    }
}

int set_random_with_bound_for_permutation(perm_t p, const uint16_t rnd_buff[CHUNK_RND_U16_LENGTH]) {

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
        p[i] = i + (rnd % bound);
    }
    return EXIT_SUCCESS;
}

void fisher_yates_shuffle_ternary(perm_t p) {
    uint16_t pi;
    for (int16_t i = PARAM_N1 - 1; i >= 0; --i) {
        pi = p[i];
        for (int16_t j = i + 1; j < PARAM_N1; ++j) {  //p[j] = p[j] == p[i] ? i : p[j];
            p[j] = p[j] == pi ? i : p[j];
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

    while (set_random_with_bound_for_permutation(p, rnd_buff) != EXIT_SUCCESS) {
        expanded_seed[SEED_BYTES + 1] += 1;
        sample_random_chunk((uint8_t *)rnd_buff, expanded_seed);
    }
    fisher_yates_shuffle_sendrier(p);
    memset(rnd_buff, 0, sizeof(rnd_buff));
}

void perm_set_random_sendrier_ternary(perm_t p, uint8_t seed[SEED_BYTES]) {
    uint16_t rnd_buff[CHUNK_RND_U16_LENGTH];
    uint8_t expanded_seed[SEED_BYTES + 2];

    memcpy(expanded_seed, seed, SEED_BYTES);
    expanded_seed[SEED_BYTES] = DOMAIN_SEPARATOR_PERM;
    expanded_seed[SEED_BYTES + 1] = 0;

    sample_random_chunk((uint8_t *)rnd_buff, expanded_seed);

    while (set_random_with_bound_for_permutation(p, rnd_buff) != EXIT_SUCCESS) {
        expanded_seed[SEED_BYTES + 1] += 1;
        sample_random_chunk((uint8_t *)rnd_buff, expanded_seed);
    }
    fisher_yates_shuffle_ternary(p);
    memset(rnd_buff, 0, sizeof(rnd_buff));
}