//
// Created by Alessandro Budroni on 15/11/2022.
//

#ifndef RANDOM_PERMUTATION_SAMPLING_DEFINITIONS_H
#define RANDOM_PERMUTATION_SAMPLING_DEFINITIONS_H

#include <stdint.h>

#define SEED_BYTES          16
#define PARAM_N             1024                     // permutation length
#define BITS_PARAM_N        11                       // bithlength of PARAM_N
#define BITS_PARAM_N_MASK   0x7FF                    // integer with BITS_PARAM_N ones
#define CHUNK_RND_BYTES_LENGTH 5000
#define CHUNK_RND_U16_LENGTH (CHUNK_RND_BYTES_LENGTH/2)
#define DOMAIN_SEPARATOR_PERM 0x03

#define	EXIT_FAILURE	1
#define	EXIT_SUCCESS	0

// TESTING
#define N_PERMUTATIONS 3
#define N_ITERATIONS 10

typedef uint16_t perm_t[PARAM_N];

#endif //RANDOM_PERMUTATION_SAMPLING_DEFINITIONS_H
