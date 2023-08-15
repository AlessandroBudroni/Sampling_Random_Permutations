//
// Created by Alessandro Budroni on 28/10/2022.
//

#include "../sorting/common_sort.h"
#include "../api.h"

#include <xkcp/SimpleFIPS202.h>
#include <string.h>
#include <stdlib.h>

static int sort_with_given_random_input(perm_t p, uint32_t *buffer) {

    for (int i = 0; i < PARAM_N; i++) {
        buffer[i] <<= BITS_PARAM_N;
        buffer[i] |= i;
    }

    // sort
    common_sort_uint32(buffer, PARAM_N);

#ifndef SORT_SAMPLING_ABORT_DISABLED
    // check that no double random values were produced
    for (int i = 1; i < PARAM_N; i++) {
        if ((buffer[i - 1] >> BITS_PARAM_N) == (buffer[i] >> BITS_PARAM_N)) {
            return EXIT_FAILURE;
        }
    }
#endif

    // extract permutation from buffer
    for (int i = 0; i < PARAM_N; i++) {
        p[i] = (uint16_t) (buffer[i] & BITS_PARAM_N_MASK);
    }
    return EXIT_SUCCESS;
}

/**
 * Apply DjbSort to sample permutation.
 * @param out permutation with random data already filled in
 * @param size of permutation
 * @return EXIT_SUCCESS for success, EXIT_FAILURE for failure in sampling
 */
void perm_set_random(perm_t out, uint8_t seed[SEED_BYTES]) {
    uint32_t rnd_buff[PARAM_N];
    uint8_t expanded_seed[SEED_BYTES + 1];

    memcpy(expanded_seed, seed, SEED_BYTES);
    expanded_seed[SEED_BYTES] = 0;
    SHAKE128((uint8_t *) rnd_buff, sizeof(rnd_buff), expanded_seed, sizeof(expanded_seed));

#ifndef SORT_SAMPLING_ABORT_DISABLED
    while (sort_with_given_random_input(out, rnd_buff) != EXIT_SUCCESS) {
        expanded_seed[SEED_BYTES] += 1;
        SHAKE128((uint8_t *) rnd_buff, sizeof(rnd_buff), expanded_seed, sizeof(expanded_seed));
    }
#else
    (void)sort_with_given_random_input(out, rnd_buff);
#endif

}

