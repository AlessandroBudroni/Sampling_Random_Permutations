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

int main() {

    printf("Start test permutation sampling, composition and inversion.\nScheme %s\n", scheme_name());
    fflush(stdout);

    // Fisher Yates non-constant time
    uint8_t seed[SEED_BYTES] = {0};
    perm_t p, pinv, id;

    // initialize PRNG
    get_seed(seed);


    for (long long int i = 0; i < N_ITERATIONS; ++i) {
        // sample permutation

        SHAKE128(seed, sizeof(seed), seed, sizeof(seed));
        perm_set_random(p, (uint8_t *) seed);
        if (validate_permutation(p) != EXIT_SUCCESS) {
            printf("FAILED - Invalid permutation\n");
            return EXIT_FAILURE;
        }
        if (test_non_triviality(p) != EXIT_SUCCESS) {
            printf("FAILED - Trivial permutation\n");
            return EXIT_FAILURE;
        }

        // invert
        perm_inverse(pinv, p);
        // compose with inversion and get identity
        perm_compose(id, p, pinv);
        for (int j = 0; j < PARAM_N; ++j) {
            if (id[j] != j) {
                printf("FAILED - inversion/composition does not give identity\n");
                return EXIT_FAILURE;
            }
        }
    }

    printf("PASSED\n");
    return EXIT_SUCCESS;
}


