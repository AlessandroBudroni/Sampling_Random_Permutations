
#include "../shuffling/common_shuffle.h"
#include "../api.h"

#include <string.h>
#include <stdlib.h>

void perm_set_random(perm_t p, uint8_t seed[SEED_BYTES]) {
    uint8_t expanded_seed[SEED_BYTES + 1];
    uniform_indexes_t indexes;


    memcpy(expanded_seed, seed, SEED_BYTES);
    expanded_seed[SEED_BYTES] = 0;

    while (common_sample_uniform_index_buffer(indexes, expanded_seed) != EXIT_SUCCESS) {
        expanded_seed[SEED_BYTES] += 1;
    }
    common_shuffle(p, indexes);
}
