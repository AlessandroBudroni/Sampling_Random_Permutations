
#include <api.h>
#include "test_utils.h"

#include <xkcp/SimpleFIPS202.h>
#include <stdlib.h>
#include <stdio.h>

int main() {

    printf("Start test permutation sampling, composition and inversion.\nScheme %s\n", scheme_name());
    fflush(stdout);

    // Fisher Yates non-constant time
    uint8_t seed[SEED_BYTES] = {0};
    perm_t p, pinv, id;

    // initialize PRNG
    get_seed(seed);


    for (long long int i = 0; i < N_ITERATIONS; ++i) {
        // sample permutation

        SHAKE128(seed, sizeof(seed), seed, sizeof(seed));
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


