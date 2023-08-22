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

#ifndef RANDOM_PERMUTATION_SAMPLING_DEFINITIONS_H
#define RANDOM_PERMUTATION_SAMPLING_DEFINITIONS_H

#include <stdint.h>

#if !defined(PARAM_N) //|| !defined(BITS_PARAM_N) || !defined(BITS_PARAM_N_MASK)

#define PARAM_N             1024                       // permutation length
#define BITS_PARAM_N        12                         // bitlength of PARAM_N

#elif (PARAM_N == 32)

#define BITS_PARAM_N        5                        // bitlength of PARAM_N

#elif (PARAM_N == 64)

#define BITS_PARAM_N        6                        // bitlength of PARAM_N

#elif (PARAM_N == 128)

#define BITS_PARAM_N        7                        // bitlength of PARAM_N

#elif (PARAM_N == 256)

#define BITS_PARAM_N        8                        // bitlength of PARAM_N

#elif (PARAM_N == 512)

#define BITS_PARAM_N        9                         // bitlength of PARAM_N

#elif (PARAM_N == 1024)

#define BITS_PARAM_N        10                       // bitlength of PARAM_N

#elif (PARAM_N == 2048)

#define BITS_PARAM_N        11                       // bitlength of PARAM_N

#elif (PARAM_N == 4096)

#define BITS_PARAM_N        12                        // bitlength of PARAM_N

#elif (PARAM_N == 8192)

#define BITS_PARAM_N        13                        // bitlength of PARAM_N

#endif


#define BITS_PARAM_N_MASK ((1 << BITS_PARAM_N) -1)   // integer with BITS_PARAM_N ones
#define LEMIRE_INDEX_SAMPLE_BUFFER_LENGTH 32 // (1/2)^32 chance to fail.
#define CHUNK_RND_BYTES_LENGTH (PARAM_N*2) // P(rejection) = 0.5
#define CHUNK_RND_U16_LENGTH (CHUNK_RND_BYTES_LENGTH/2)
#define DOMAIN_SEPARATOR_PERM 0x03
#define SEED_BYTES          16

// TESTING
#define N_ITERATIONS 1000

typedef uint16_t perm_t[PARAM_N] __attribute__((aligned(32)));
typedef uint16_t uniform_indexes_t[PARAM_N] __attribute__((aligned(32)));

#endif //RANDOM_PERMUTATION_SAMPLING_DEFINITIONS_H
