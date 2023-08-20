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

#ifndef RANDOM_PERMUTATION_SAMPLING_MACROS_H
#define RANDOM_PERMUTATION_SAMPLING_MACROS_H

#include <stdint.h>

// constant time "greater than"
#define GT16(b1,b2) ((uint16_t)((uint16_t)(b2) -(uint16_t)(b1)) >> 15 & (uint8_t)0x1)


static inline __attribute__((always_inline)) uint16_t ISZERO(uint16_t n) {
    return (uint16_t)(1u & ((((uint32_t)n) - 1u) >> 31u));
}
// Return 1 if n = 0, 0 otherwise - for uint16_t
//#define ISZERO(n) (1 & ((n - 1) >> 15u)) // ((n == 0) ? 1 : 0)

// Return 1 if n ≠ 0, 0 otherwise - for uint16_t
//#define ISNOTZERO(n) ((((n) | (~(n) + 1)) >> 15) & (uint8_t)0x1)

// Return either a or b according to the mask
#define MASKAPPLY(mask, a, b) ((a) ^ ((mask) & (a ^ b))) //(((mask) & (a)) | ((~(mask)) & (b)))

#endif //RANDOM_PERMUTATION_SAMPLING_MACROS_H
