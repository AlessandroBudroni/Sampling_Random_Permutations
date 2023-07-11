//
// Created by Alessandro Budroni on 16/11/2022.
//

#include "fisher_yates_sendrier.h"
#include "../common.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>


//void fisher_yates_shuffle_sendrier(perm_t p) {
//    uint16_t mask;
//    uint16_t pi, *pj;
//    for (int16_t i = PARAM_N - 1; i >= 0; --i) {
//        pi = p[i];
//        for (int16_t j = i + 1; j < PARAM_N; ++j) {  //p[j] = p[j] == p[i] ? i : p[j];
//            pj = &p[j];
//            mask = ISNOTZERO(*pj - pi) - 1;
//            *pj = MASKAPPLY(mask, i, *pj); //(mask & i) | (~mask & *pj);
//        }
//    }
//}

static void fisher_yates_shuffle_sendrier_ternary(perm_t p) {
    uint16_t pi;
    for (int i = PARAM_N - 1; i >= 0; --i) {
        pi = p[i];
        for (int j = i + 1; j < PARAM_N; ++j) {
            p[j] = p[j] == pi ? i : p[j];
        }
    }
}

static void fisher_yates_shuffle_sendrier(perm_t p) {
    uint16_t mask;
    uint16_t *pi;
    for (int i = PARAM_N - 1; i >= 0; --i) {
        pi = &p[i];
        mask = 0;
        for (int j = i + 1; j < PARAM_N; ++j) {
            mask |= ISZERO(p[j] - *pi);
        }
        *pi = MASKAPPLY(mask-1, i, *pi);
    }
}

static int set_bounded_random_for_sendrier_shuffle(perm_t p, const uint16_t rnd_buff[CHUNK_RND_U16_LENGTH]) {

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
        p[i] = i + (rnd % bound);
    }
    return EXIT_SUCCESS;
}

void perm_set_random_sendrier(perm_t p, uint8_t seed[SEED_BYTES]) {
    uint16_t rnd_buff[CHUNK_RND_U16_LENGTH];
    uint8_t expanded_seed[SEED_BYTES + 2];

    memcpy(expanded_seed, seed, SEED_BYTES);
    expanded_seed[SEED_BYTES] = DOMAIN_SEPARATOR_PERM;
    expanded_seed[SEED_BYTES + 1] = 0;

    sample_random_chunk((uint8_t *)rnd_buff, expanded_seed);

    while (set_bounded_random_for_sendrier_shuffle(p, rnd_buff) != EXIT_SUCCESS) {
        expanded_seed[SEED_BYTES + 1] += 1;
        sample_random_chunk((uint8_t *)rnd_buff, expanded_seed);
    }
    fisher_yates_shuffle_sendrier(p);
    memset(rnd_buff, 0, sizeof(rnd_buff));
}

void perm_set_random_sendrier_ternary_operator(perm_t p, uint8_t seed[SEED_BYTES]) {
    uint16_t rnd_buff[CHUNK_RND_U16_LENGTH];
    uint8_t expanded_seed[SEED_BYTES + 2];

    memcpy(expanded_seed, seed, SEED_BYTES);
    expanded_seed[SEED_BYTES] = DOMAIN_SEPARATOR_PERM;
    expanded_seed[SEED_BYTES + 1] = 0;

    sample_random_chunk((uint8_t *)rnd_buff, expanded_seed);

    while (set_bounded_random_for_sendrier_shuffle(p, rnd_buff) != EXIT_SUCCESS) {
        expanded_seed[SEED_BYTES + 1] += 1;
        sample_random_chunk((uint8_t *)rnd_buff, expanded_seed);
    }
    fisher_yates_shuffle_sendrier_ternary(p);
    memset(rnd_buff, 0, sizeof(rnd_buff));
}