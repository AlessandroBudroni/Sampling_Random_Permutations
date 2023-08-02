
/**
 * Using modulo to sample uniform integers in range [a,b] does not yield uniform distribution.
 * The purpose of this sampling code is for benchmarking and constant time testing.
 */

#include "common_shuffle.h"

#include <xkcp/SimpleFIPS202.h>
#include <stdlib.h>

int common_sample_uniform_index_buffer(uniform_indexes_t output, const uint8_t seed[SEED_BYTES + 1]) {
    uint16_t rnd;
    int bound;

    for (int i = 0; i < PARAM_N; i++) {
        bound = PARAM_N - i;
        SHAKE128((uint8_t *)&rnd, 2, seed, SEED_BYTES + 1);
        output[i] = i + (rnd % bound);
    }

    return EXIT_SUCCESS;
}
