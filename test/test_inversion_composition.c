//
// Created by Alessandro Budroni on 19/07/2023.
//

#include <stdio.h>

#include "../src/sampling/fisher_yates.h"
#include "../src/composition/composition_substitution.h"
#include "../src/inversion/inversion_substitution.h"
#include "../src/sampling/fisher_yates_sendrier.h"
#include "../src/sampling/djbsort_sample.h"
#include "../src/sampling/AVX2/djbsort_sample_AVX2.h"
#include "../src/sampling/AVX2/fisher_yates_natural_AVX2.h"
#include "../src/sampling/AVX2/fisher_yates_sendrier_AVX2.h"
#include "test_permutation_utils_AVX2.h"
#include "test_permutation_utils.h"

static int test_composition_inversion_non_ct(){

    // Fisher Yates non-constant time
    uint16_t seed[SEED_BYTES / 2] = {0};
    perm_t p, pinv, id;

    printf("Test inversion/composition non-constant time ... ");
    for (int i = 0; i < N_ITERATIONS; ++i) {
        // sample permutation
        seed[0] = i;
        perm_set_random_fisher_yates_non_ct(p, (uint8_t *) seed);
        if(validate_permutation(p) != EXIT_SUCCESS){
            printf("FAILED\n");
            return EXIT_FAILURE;
        }
        if (test_non_triviality(p) != EXIT_SUCCESS){
            printf("FAILED - Trivial permutation\n");
            return EXIT_FAILURE;
        }

        // invert
        perm_invert_substitution_non_ct(pinv, p);
        // compose with inversion and get identity
        perm_compose_substitution_non_ct(id, p, pinv);
        for (int j = 0; j < PARAM_N; ++j) {
            if (id[j] != j){
                printf("FAILED - inversion/composition does not give identity\n");
                return EXIT_FAILURE;
            }
        }
    }

    printf("PASSED\n");
    return EXIT_SUCCESS;
}

static int test_composition_inversion(){

    // Fisher Yates non-constant time
    uint16_t seed[SEED_BYTES / 2] = {0};
    perm_t p, pinv, id;

    printf("Test inversion/composition ...                   ");
    for (int i = 0; i < N_ITERATIONS; ++i) {
        // sample permutation
        seed[0] = i;
        perm_set_random_fisher_yates_non_ct(p, (uint8_t *) seed);
        if(validate_permutation(p) != EXIT_SUCCESS){
            printf("FAILED\n");
            return EXIT_FAILURE;
        }
        if (test_non_triviality(p) != EXIT_SUCCESS){
            printf("FAILED - Trivial permutation\n");
            return EXIT_FAILURE;
        }

        // invert
        perm_invert_substitution(pinv, p);
        // compose with inversion and get identity
        perm_compose_substitution(id, p, pinv);
        for (int j = 0; j < PARAM_N; ++j) {
            if (id[j] != j){
                printf("FAILED - inversion/composition does not give identity\n");
                return EXIT_FAILURE;
            }
        }
    }

    printf("PASSED\n");
    return EXIT_SUCCESS;
}


int main() {

    printf("Start test permutation composition and inversion.\n\n");fflush(stdout);

    test_composition_inversion();
    test_composition_inversion_non_ct();

    printf("\nDone\n");

    return 0;
}

