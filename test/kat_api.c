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

#include <api.h>
#include "test_utils.h"

#include <xkcp/SimpleFIPS202.h>
#include <stdlib.h>
#include <stdio.h>

void print_buffer(char * msg, uint8_t kat[SEED_BYTES]) {
    printf("%s ", msg);
    for (int i = 0; i < SEED_BYTES; i++) {
        printf("%02X", kat[i]);
    }
    printf("\n");
}

int main() {

    printf("Simple KAT test for scheme %s\n", scheme_name());
    fflush(stdout);

    // Fisher Yates non-constant time
    uint8_t seed[SEED_BYTES] = {0};
    uint8_t kat[SEED_BYTES] = {0};
    perm_t p;

    // sample permutation
    SHAKE128(seed, sizeof(seed), seed, sizeof(seed));

    perm_set_random(p, seed);
    if (validate_permutation(p) != EXIT_SUCCESS) {
        printf("FAILED - Invalid permutation\n");
        return EXIT_FAILURE;
    }
    SHAKE128(kat, sizeof(kat), (uint8_t *)p, sizeof(p));
    print_buffer("kat: ", kat);

    printf("PASSED\n");
    return EXIT_SUCCESS;
}


