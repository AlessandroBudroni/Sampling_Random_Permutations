
#include <stdio.h>
#include <api.h>
#include "test_utils.h"
#include <stdlib.h>

int main() {

    printf("Start test permutation composition and inversion.\n\n");
    fflush(stdout);

    // Fisher Yates non-constant time
    uint8_t seed[SEED_BYTES] = {0};
    uint64_t *b_seed = (uint64_t*)seed;
    perm_t p, pinv, id;

    for (long long int i = 0; i < N_ITERATIONS; ++i) {
        // sample permutation
        *b_seed = (uint64_t)i;
        perm_set_random(p, (uint8_t *) seed);
        if (validate_permutation(p) != EXIT_SUCCESS) {
            printf("FAILED - Invalid permutation\n");
            return EXIT_FAILURE;
        }
        if (test_non_triviality(p) != EXIT_SUCCESS) {
            printf("FAILED - Trivial permutation\n");
            return EXIT_FAILURE;
        }

        // invert
        perm_inverse(pinv, p);
        // compose with inversion and get identity
        perm_compose(id, p, pinv);
        for (int j = 0; j < PARAM_N; ++j) {
            if (id[j] != j) {
                printf("FAILED - inversion/composition does not give identity\n");
                return EXIT_FAILURE;
            }
        }
    }

    printf("PASSED\n");
    return EXIT_SUCCESS;
}


