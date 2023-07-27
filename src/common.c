//
// Created by Alessandro Budroni on 06/07/2023.
//

#include "common.h"
#include <xkcp/SimpleFIPS202.h>

void sample_random_chunk( uint8_t rnd_buff[CHUNK_RND_BYTES_LENGTH], uint8_t expanded_seed[SEED_BYTES + 2]){
    SHAKE128((uint8_t *)rnd_buff, CHUNK_RND_BYTES_LENGTH, expanded_seed, SEED_BYTES + 2);
}

int set_random_with_bound_for_fisher_yates(perm_t p, const uint16_t rnd_buff[CHUNK_RND_U16_LENGTH]) {

    uint16_t rnd;
    int32_t index = 0;
    uint32_t max;
    int bound;

    for (int i = 0; i < PARAM_N; i++) {
        bound = PARAM_N - i;
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