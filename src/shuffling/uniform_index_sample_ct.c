
#include "common_shuffle.h"
#include "../macros.h"

#include <xkcp/KeccakHash.h>
#include <xkcp/SimpleFIPS202.h>
#include <stdlib.h>
#include <stdio.h>

#define INDEX_SAMPLE_BUFFER_LENGTH 32 // (1/2)^64 chance to fail.

uint16_t BITMASKS[] = {0x1, 0x3, 0x7, 0xF, 0x1F, 0x3F, 0x7F, 0xFF, 0x1FF, 0x3FF, 0x7FF};

/**
 * Iterate through all rand buffer to find one index, no abort.
 * Index should be smaller or equal to bound.
 * mas is used to guarantee at least 50% chance of success per iteration.
 */
static inline uint16_t sample_single_index(uint16_t rand[INDEX_SAMPLE_BUFFER_LENGTH], uint16_t bound, uint16_t mask) {
    uint16_t ret = 0;
    uint16_t tmp;
    uint16_t condition;
    // Must guarantee (statistically) that at least once tmp is assigned to ret.
    for (int i = 0; i < INDEX_SAMPLE_BUFFER_LENGTH; i++) {
        tmp = rand[i] & mask; // tmp <- rand(0, 2^l)
        condition = GT16(bound, tmp);
        ret = MASKAPPLY(condition - 1, tmp, ret);
    }
    return ret;
}

int common_sample_uniform_index_buffer(uniform_indexes_t output, const uint8_t seed[SEED_BYTES + 1]) {
    int bound;

    uint16_t rnd_buff[INDEX_SAMPLE_BUFFER_LENGTH];
    Keccak_HashInstance ctx;
    Keccak_HashInitialize_SHAKE128(&ctx);
    Keccak_HashUpdate(&ctx, seed, SEED_BYTES * 8);
    Keccak_HashFinal(&ctx, NULL);
    uint16_t bitmask_index = 0;

    for (int i = PARAM_N - 1; i >= 0; --i) {
        bound = PARAM_N - i -1;
        Keccak_HashSqueeze(&ctx, (uint8_t *)rnd_buff, sizeof(rnd_buff) * 8);
        if (bound > BITMASKS[bitmask_index]) {
            bitmask_index++;
        }
        output[i] = i + sample_single_index(rnd_buff, bound, BITMASKS[bitmask_index]);
    }
    return EXIT_SUCCESS;
}
