
#include "common_uniform_index_sample.h"

#include <xkcp/SimpleFIPS202.h>
#include <xkcp/KeccakHash.h>
#include <stdlib.h>

/**
 * Buffered version of Lemire interval rejection sample.
 * url:  https://arxiv.org/pdf/1805.10941.pdf
 */
int lemire_sample(uint16_t *out, uint16_t s, Keccak_HashInstance *ctx) {
    uint32_t x = 0;
    Keccak_HashSqueeze(ctx, (uint8_t *)&x, sizeof(x) * 8);
    uint32_t m = x * (uint32_t) s;
    uint16_t l = (uint16_t) m; // m mod 2*^16
    if (l < s) {
        uint16_t t = -s % s;
        while (l < t) {
            Keccak_HashSqueeze(ctx, (uint8_t *)&x, sizeof(x) * 8);
            m = x * (uint32_t) s;
            l = (uint16_t) m; // m mod 2*^16
            l = (uint16_t) m; // m mod 2*^16
        }
    }
    *out = m >> 16;
    return EXIT_SUCCESS;
}

int common_sample_uniform_index_buffer(uniform_indexes_t output, const uint8_t seed[SEED_BYTES + 1]) {
    int32_t rnd_index = 0;
    uint16_t bound;

    Keccak_HashInstance ctx;
    Keccak_HashInitialize_SHAKE128(&ctx);
    Keccak_HashUpdate(&ctx, seed, (SEED_BYTES +1)* 8);
    Keccak_HashFinal(&ctx, NULL);

    for (int i = 0; i < PARAM_N; i++) {
        bound = PARAM_N - i;
        if (lemire_sample(&output[i], bound, &ctx) != EXIT_SUCCESS) {
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}
