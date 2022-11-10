//
// Created by Alessandro Budroni on 07/09/2022.
//

#ifndef PLAYAVX2_UTILS_H
#define PLAYAVX2_UTILS_H

#include <stdint.h>

#ifdef OTHER
#define SEED_BYTES 16
#define PARAM_N1            1238
#define PARAM_N1_HALF       (PARAM_N1/2)
#define CHUNK_RND_BYTES_LENGTH 5000
#define CHUNK_RND_U16_LENGTH (CHUNK_RND_BYTES_LENGTH/2)
#define DOMAIN_SEPARATOR_PERM 0x03
#endif

#define SEED_BYTES          16
#define PARAM_N1            2048
#define CHUNK_RND_BYTES_LENGTH 5000
#define CHUNK_RND_U16_LENGTH (CHUNK_RND_BYTES_LENGTH/2)
#define DOMAIN_SEPARATOR_PERM 0x03

#define	EXIT_FAILURE	1
#define	EXIT_SUCCESS	0

// TESTING
#define N_PERMUTATIONS 2000
#define N_ITERATIONS 10

int64_t cpucycles(void);
double warm_up();

#endif //PLAYAVX2_UTILS_H
