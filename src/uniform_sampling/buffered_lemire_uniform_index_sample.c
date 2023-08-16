
#include "common_uniform_index_sample.h"

#include "xkcp/SimpleFIPS202.h"
#include <stdlib.h>

/**
 * Buffered version of Lemire interval rejection sample.
 * url:  https://arxiv.org/pdf/1805.10941.pdf
 */
int lemire_sample(uint16_t *out, uint16_t s, uint16_t const rand[CHUNK_RND_U16_LENGTH], int *rand_index) {
    if (*rand_index >= CHUNK_RND_U16_LENGTH) {
        return EXIT_FAILURE;
    }
    uint32_t x = (uint32_t)rand[(*rand_index)++];
    uint32_t m = x * (uint32_t) s;
    uint16_t l = (uint16_t) m; // m mod 2^16
    if (l < s) {
        uint16_t t = -s % s;
        while (l < t) {
            if (*rand_index >= CHUNK_RND_U16_LENGTH) {
                return EXIT_FAILURE;
            }
            x = (uint32_t)rand[(*rand_index)++];
            m = x * (uint32_t) s;
            l = (uint16_t) m; // m mod 2*^16
        }
    }
    *out = m >> 16;
    return EXIT_SUCCESS;
}

int common_sample_uniform_index_buffer(uniform_indexes_t output, const uint8_t seed[SEED_BYTES + 1]) {
    int32_t rnd_index = 0;
    uint16_t bound;

    uint16_t rnd_buff[CHUNK_RND_U16_LENGTH];
    SHAKE128((uint8_t *) rnd_buff, CHUNK_RND_BYTES_LENGTH, seed, SEED_BYTES + 1);

    for (int i = 0; i < PARAM_N; i++) {
        bound = PARAM_N - i;
        if (lemire_sample(&output[i], bound, rnd_buff, &rnd_index) != EXIT_SUCCESS) {
            return EXIT_FAILURE;
        }
        output[i] += i;
    }
    return EXIT_SUCCESS;
}
