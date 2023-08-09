
#include "common_shuffle.h"

#include <xkcp/SimpleFIPS202.h>
#include <stdlib.h>

int common_sample_uniform_index_buffer(uniform_indexes_t output, const uint8_t seed[SEED_BYTES + 1]) {
    uint16_t rnd;
    int32_t index = 0;
    uint32_t max;
    int bound;

    uint16_t rnd_buff[CHUNK_RND_U16_LENGTH];
    SHAKE128((uint8_t *)rnd_buff, CHUNK_RND_BYTES_LENGTH, seed, SEED_BYTES + 1);

    for (int i = 0; i < PARAM_N; i++) {
        bound = PARAM_N - i;
        max = ((0x10000 / bound ) * bound);// (2^16 / bound) * bound
        do {
            if (index >= CHUNK_RND_U16_LENGTH -1){
                return EXIT_FAILURE;
            }
            rnd = rnd_buff[index++];
        } while (rnd >= max);
        output[i] = i + (rnd % bound);
    }
    return EXIT_SUCCESS;
}
