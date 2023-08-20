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

#ifndef RANDOM_PERMUTATION_SAMPLING_VERIFICATION_H
#define RANDOM_PERMUTATION_SAMPLING_VERIFICATION_H

#endif //RANDOM_PERMUTATION_SAMPLING_VERIFICATION_H

#include "../src/definitions.h"

#ifndef SCHEME_NAME
#define SCHEME_NAME "generic"
#endif

static inline char *scheme_name() {
    return SCHEME_NAME;
}

void get_seed(uint8_t seed[SEED_BYTES]);
int validate_permutation(const perm_t p);
int test_non_triviality(const perm_t p);
int test_equality(const perm_t p1, const perm_t p2);