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

#include "../shuffling/common_shuffle.h"
#include "../uniform_sampling/common_uniform_index_sample.h"
#include "../api.h"

#include <string.h>
#include <stdlib.h>

void perm_set_random(perm_t p, uint8_t seed[SEED_BYTES]) {
    uint8_t expanded_seed[SEED_BYTES + 1];
    uniform_indexes_t indexes;


    memcpy(expanded_seed, seed, SEED_BYTES);
    expanded_seed[SEED_BYTES] = 0;

    while (common_sample_uniform_index_buffer(indexes, expanded_seed) != EXIT_SUCCESS) {
        expanded_seed[SEED_BYTES] += 1;
    }
    common_shuffle(p, indexes);
}
