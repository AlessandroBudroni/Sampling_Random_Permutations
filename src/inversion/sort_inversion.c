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

#include "../definitions.h"
#include "../sorting/common_sort.h"

void perm_inverse(perm_t o, const perm_t p) {
    uint32_t i;
    uint32_t buffer[PARAM_N];

    // set buffer as p[i] || i, for i=0...PARAM_N1
    for (i = 0; i < PARAM_N; i++) {
        buffer[i] = p[i];
        buffer[i] <<= 16;
        buffer[i] |= i;
    }

    // sort
    common_sort_uint32(buffer, PARAM_N);

    // extract permutation from buffer
    for (i = 0; i < PARAM_N; i++) {
        o[i] = (uint16_t)(buffer[i]);
    }

}

