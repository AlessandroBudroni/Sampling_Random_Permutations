//
// Created by Alessandro Budroni on 06/07/2023.
//

#include "common.h"
#include "../fips202/fips202.h"

void sample_random_chunk( uint8_t rnd_buff[CHUNK_RND_BYTES_LENGTH], uint8_t expanded_seed[SEED_BYTES + 2]){
    shake128((uint8_t *)rnd_buff, CHUNK_RND_BYTES_LENGTH, expanded_seed, SEED_BYTES + 2);
}

