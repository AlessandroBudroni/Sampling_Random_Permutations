/**
 * MIT License
 *
 * Copyright (c) 2023 [FULL NAME]
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "common_uniform_index_sample.h"
#include "lemire_constants.h"

#include <xkcp/SimpleFIPS202.h>
#include <xkcp/KeccakHash.h>
#include <stdlib.h>

/**
 * Buffered version of Lemire interval rejection sample.
 * url:  https://arxiv.org/pdf/1805.10941.pdf
 */
int lemire_sample(uint16_t *out, uint16_t s, Keccak_HashInstance *ctx) {
    uint16_t x = 0;
    Keccak_HashSqueeze(ctx, (uint8_t *) &x, sizeof(x) * 8);
    uint32_t m = (uint32_t) x * (uint32_t) s;
    uint16_t l = (uint16_t) m; // m mod 2*^16
    while (l < lemire_table[s]) {
        Keccak_HashSqueeze(ctx, (uint8_t *) &x, sizeof(x) * 8);
        m = x * (uint32_t) s;
        l = (uint16_t) m; // m mod 2*^16
        l = (uint16_t) m; // m mod 2*^16
    }
    *out = m >> 16;
    return EXIT_SUCCESS;
}

int common_sample_uniform_index_buffer(uniform_indexes_t output, const uint8_t seed[SEED_BYTES + 1]) {
//    int32_t rnd_index = 0;
    uint16_t bound;

    Keccak_HashInstance ctx;
    Keccak_HashInitialize_SHAKE128(&ctx);
    Keccak_HashUpdate(&ctx, seed, (SEED_BYTES + 1) * 8);
    Keccak_HashFinal(&ctx, NULL);

    for (int i = 0; i < PARAM_N; i++) {
        bound = PARAM_N - i;
        if (lemire_sample(&output[i], bound, &ctx) != EXIT_SUCCESS) {
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}
