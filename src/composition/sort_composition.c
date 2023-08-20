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

#include "../api.h"
#include "../sorting/common_sort.h"

void perm_compose(perm_t out, const perm_t in1, const perm_t in2) {
    uint32_t i;
    uint32_t buffer[PARAM_N];
    perm_t tmp;

    perm_inverse(tmp, in2);
    // set buffer as tmp[i] || i, for i=0...PARAM_N1
    for (i = 0; i < PARAM_N; i++) {
        buffer[i] = tmp[i];
        buffer[i] <<= 16;
        buffer[i] |= in1[i];
    }

    // sort
    common_sort_uint32(buffer, PARAM_N);

    // extract permutation from buffer
    for (i = 0; i < PARAM_N; i++) {
        out[i] = (uint16_t)(buffer[i]);
    }
}

// compose in1 with inv(in2)
void perm_compose_with_inverse_djbsort(perm_t out, const perm_t in1, const perm_t in2) {
    uint32_t i;
    uint32_t buffer[PARAM_N];

    // set buffer as tmp[i] || i, for i=0...PARAM_N1
    for (i = 0; i < PARAM_N; i++) {
        buffer[i] = in2[i];
        buffer[i] <<= 16;
        buffer[i] |= in1[i];
    }

    // sort
    common_sort_uint32(buffer, PARAM_N);

    // extract permutation from buffer
    for (i = 0; i < PARAM_N; i++) {
        out[i] = (uint16_t)(buffer[i]);
    }
}
