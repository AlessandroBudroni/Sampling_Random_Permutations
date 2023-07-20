//
// Created by Alessandro Budroni on 19/07/2023.
//

#include <stdio.h>

#include "../src/sampling/fisher_yates.h"
#include "../src/composition/standard_composition.h"
#include "../src/composition/djbsort_composition.h"
#include "../src/inversion/djbsort_inversion.h"
#include "test_permutation_utils.h"

static int test_composition_compatibility() {

    // Fisher Yates non-constant time
    uint16_t seed[SEED_BYTES / 2] = {0};
    perm_t p[N_PERMUTATIONS];
    perm_t pnonct, pst, pdjsort;

    printf("Test composition compatibility all ...                     ");
    for (int j = 0; j <1; ++j) {
        for (int i = 0; i < N_PERMUTATIONS; ++i) {
            seed[0] = j;
            perm_set_random_fisher_yates_non_ct(p[i], (uint8_t *) seed);
        }

        perm_compose_djbsort(pdjsort, p[0], p[1]);
        perm_compose_standard(pst, p[0], p[1]);
        perm_compose_standard_non_ct(pnonct, p[0], p[1]);

        for (int i = 2; i < N_PERMUTATIONS; ++i) {
            perm_compose_djbsort(pdjsort, pdjsort, p[i]);
            perm_compose_standard(pst, pst, p[i]);
            perm_compose_standard_non_ct(pnonct, pnonct, p[i]);
        }

        // check that the tree method computed the same permutation
        for (int i = 0; i < PARAM_N; ++i) {
            if(pdjsort[i] != pst[i] || pnonct[i] != pst[i]){
                printf("FAILED - final composed permutations do not match at iteration %d\n", j);
                return EXIT_FAILURE;
            }
        }

    }
    printf("PASSED\n");
    return EXIT_SUCCESS;
}

static int test_djbsort_composition() {

    // Fisher Yates non-constant time
    uint16_t seed[SEED_BYTES / 2] = {0};
    perm_t p[N_PERMUTATIONS];
    perm_t psmart, pdjsort;

    printf("Test composition compatibility two methods of djbsort ...  ");
    for (int j = 0; j <1; ++j) {
        for (int i = 0; i < N_PERMUTATIONS; ++i) {
            seed[0] = j;
            perm_set_random_fisher_yates_non_ct(p[i], (uint8_t *) seed);
        }

        // trivial composition method
        perm_compose_djbsort(pdjsort, p[0], p[1]);
        for (int i = 2; i < N_PERMUTATIONS; ++i) {
            perm_compose_djbsort(pdjsort, pdjsort, p[i]);
        }

        // smart composition method (to save almost half sorting)
        perm_inverse_djbsort(psmart, p[N_PERMUTATIONS-1]);
        for (int i = N_PERMUTATIONS-2; i >= 0; --i) {
            perm_compose_with_inverse_djbsort(psmart, psmart, p[i]);
        }
        perm_inverse_djbsort(psmart, psmart);

        // check that the tree method computed the same permutation
        for (int i = 0; i < PARAM_N; ++i) {
            if(pdjsort[i] != psmart[i]){
                printf("FAILED - final composed permutations do not match at iteration %d\n", j);
                return EXIT_FAILURE;
            }
        }

    }
    printf("PASSED\n");
    return EXIT_SUCCESS;
}


int main() {

    printf("Start test compatibility among the methods.\n\n");fflush(stdout);

    test_composition_compatibility();
    test_djbsort_composition();

    printf("\nDone\n");

    return 0;
}

