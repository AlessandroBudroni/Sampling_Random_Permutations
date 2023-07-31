//
// Created by Alessandro Budroni on 15/11/2022.
//

#ifndef RANDOM_PERMUTATION_SAMPLING_DEFINITIONS_H
#define RANDOM_PERMUTATION_SAMPLING_DEFINITIONS_H

#include <stdint.h>

//#define PARAM_N             64                      // permutation length
//#define BITS_PARAM_N        7                        // bithlength of PARAM_N
//#define BITS_PARAM_N_MASK   0x7F                     // integer with BITS_PARAM_N ones

//#define PARAM_N             2048                     // permutation length
//#define BITS_PARAM_N        12                       // bithlength of PARAM_N
//#define BITS_PARAM_N_MASK   0xFFF                    // integer with BITS_PARAM_N ones

#define PARAM_N             1024                     // permutation length
#define BITS_PARAM_N        11                       // bithlength of PARAM_N
#define BITS_PARAM_N_MASK   0x7FF                    // integer with BITS_PARAM_N ones

#define CHUNK_RND_BYTES_LENGTH 5000
#define CHUNK_RND_U16_LENGTH (CHUNK_RND_BYTES_LENGTH/2)
#define DOMAIN_SEPARATOR_PERM 0x03
#define SEED_BYTES          16

// TESTING
#define N_PERMUTATIONS 100
#define N_ITERATIONS 10000

typedef uint16_t perm_t[PARAM_N];

#endif //RANDOM_PERMUTATION_SAMPLING_DEFINITIONS_H
