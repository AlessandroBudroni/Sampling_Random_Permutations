//
// Created by Alessandro Budroni on 15/11/2022.
//

#ifndef RANDOM_PERMUTATION_SAMPLING_DEFINITIONS_H
#define RANDOM_PERMUTATION_SAMPLING_DEFINITIONS_H

#include <stdint.h>

#if !defined(PARAM_N) //|| !defined(BITS_PARAM_N) || !defined(BITS_PARAM_N_MASK)

#define PARAM_N             4096                       // permutation length
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
