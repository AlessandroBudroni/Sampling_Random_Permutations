//
// Created by Alessandro Budroni on 19/07/2023.
//

#include <stdio.h>

#include "../src/sampling/fisher_yates.h"
#include "../src/sampling/fisher_yates_sendrier.h"
#include "../src/sampling/fisher_yates_natural.h"
#include "../src/sampling/djbsort_sample.h"
#include "../src/sampling/AVX2/fisher_yates_sendrier_AVX2.h"
#include "../src/sampling/AVX2/fisher_yates_natural_AVX2.h"
#include "../src/sampling/AVX2/djbsort_sample_AVX2.h"
#include "../src/composition/standard_composition.h"
#include "../src/composition/djbsort_composition.h"
#include "../src/inversion/djbsort_inversion.h"
#include "test_permutation_utils.h"
#include "test_permutation_utils_AVX2.h"

static int test_composition_compatibility() {

    // Test compositions compatibility among the different methods of composing
    uint16_t seed[SEED_BYTES / 2] = {0};
    perm_t p[N_PERMUTATIONS];
    perm_t pnonct, pst, pdjsort;

    printf("Test composition compatibility all ...                                           ");
    for (int j = 0; j < N_ITERATIONS; ++j) {
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

        // check that the tree methods give the same permutation
        if(EXIT_SUCCESS != test_equality(pdjsort, pst)){
            printf("FAILED\n");
            return EXIT_FAILURE;
        }
        if(EXIT_SUCCESS != test_equality(pnonct, pst)){
            printf("FAILED\n");
            return EXIT_FAILURE;
        }
    }
    printf("PASSED\n");
    return EXIT_SUCCESS;
}

static int test_djbsort_composition() {

    // Test the two method of composing a list of permutations with djbsort
    uint16_t seed[SEED_BYTES / 2] = {0};
    perm_t p[N_PERMUTATIONS];
    perm_t psmart, pdjsort;

    printf("Test composition compatibility two methods of djbsort ...                        ");
    for (int j = 0; j < N_ITERATIONS; ++j) {
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

        // check that the two methods give the same permutation
        if(EXIT_SUCCESS != test_equality(pdjsort, psmart)){
            printf("FAILED\n");
            return EXIT_FAILURE;
        }
    }
    printf("PASSED\n");
    return EXIT_SUCCESS;
}

static int test_sampling_compatibility_fisher_yates_vs_sendrier() {

    // Test that fisher yates and sendrier fisher yates give the same permutation for a given input
    uint16_t seed[SEED_BYTES / 2] = {0};
    perm_t p1[N_PERMUTATIONS];
    perm_t p2[N_PERMUTATIONS];

    printf("Test Fisher Yates vs Sendrier permutation sampling compatibility ...             ");
    for (int j = 0; j < 1; ++j) {
        for (int i = 0; i < N_PERMUTATIONS; ++i) {
            seed[0] = j;
            perm_set_random_fisher_yates_non_ct(p1[i], (uint8_t *) seed);
        }

        for (int i = 0; i < N_PERMUTATIONS; ++i) {
            seed[0] = j;
            perm_set_random_sendrier(p2[i], (uint8_t *) seed);
        }

        // check that the two methods give the same permutation
        for (int i = 0; i < N_PERMUTATIONS; ++i) {
            if (EXIT_SUCCESS != test_equality(p1[i], p2[i])) {
                printf("FAILED\n");
                return EXIT_FAILURE;
            }
        }

    }
    printf("PASSED\n");
    return EXIT_SUCCESS;
}

static int test_sampling_compatibility_fisher_yates_vs_sendrier_AVX2() {

    // Test that Sendrier and Sendrier AVX2 give the same permutation for a given input
    uint16_t seed[SEED_BYTES / 2] = {0};
    perm_t p1[N_PERMUTATIONS];
    permAVX_t p2[N_PERMUTATIONS];

    printf("Test Sendrier vs Sendrier AVX2 permutation sampling compatibility ...            ");
    for (int j = 0; j < 1; ++j) {
        for (int i = 0; i < N_PERMUTATIONS; ++i) {
            seed[0] = j;
            perm_set_random_sendrier(p1[i], (uint8_t *) seed);
        }

        for (int i = 0; i < N_PERMUTATIONS; ++i) {
            seed[0] = j;
            perm_set_random_sendrier_avx2(&p2[i], (uint8_t *) seed);
        }

        // check that the two methods give the same permutation
        for (int i = 0; i < N_PERMUTATIONS; ++i) {
            if (EXIT_SUCCESS != test_equality_avx2(p1[i], p2[i])) {
                printf("FAILED\n");
                return EXIT_FAILURE;
            }
        }

    }
    printf("PASSED\n");
    return EXIT_SUCCESS;
}

static int test_sampling_compatibility_djbsort_vs_djbsort_AVX2() {

    // Test that djbsort and djbsort AVX2 give the same permutation for a given input
    uint16_t seed[SEED_BYTES / 2] = {0};
    perm_t p1[N_PERMUTATIONS];
    perm_t p2[N_PERMUTATIONS];

    printf("Test djbsort vs djbsort AVX2 permutation sampling compatibiility ...             ");
    for (int j = 0; j < 1; ++j) {
        for (int i = 0; i < N_PERMUTATIONS; ++i) {
            seed[0] = j;
            perm_set_random_djbsort(p1[i], (uint8_t *) seed);
        }

        for (int i = 0; i < N_PERMUTATIONS; ++i) {
            seed[0] = j;
            perm_set_random_djbsort_avx2(p2[i], (uint8_t *) seed);
        }

        // check that the two methods give the same permutation
        for (int i = 0; i < N_PERMUTATIONS; ++i) {
            if (EXIT_SUCCESS != test_equality(p1[i], p2[i])) {
                printf("FAILED\n");
                return EXIT_FAILURE;
            }
        }

    }
    printf("PASSED\n");
    return EXIT_SUCCESS;
}

static int test_sampling_compatibility_natural_vs_natural_AVX2() {

    // Test that natural fy and natural fy AVX2 give the same permutation for a given input
    uint16_t seed[SEED_BYTES / 2] = {0};
    perm_t p1[N_PERMUTATIONS];
    perm_t p2[N_PERMUTATIONS];

    printf("Test natural fy vs natural fy AVX2 permutation sampling compatibiility ...       ");
    for (int j = 0; j < 1; ++j) {
        for (int i = 0; i < N_PERMUTATIONS; ++i) {
            seed[0] = j;
            perm_set_random_natural(p1[i], (uint8_t *) seed);
        }

        for (int i = 0; i < N_PERMUTATIONS; ++i) {
            seed[0] = j;
            perm_set_random_natural_avx2(p2[i], (uint8_t *) seed);
        }

        // check that the two methods give the same permutation
        for (int i = 0; i < N_PERMUTATIONS; ++i) {
            if (EXIT_SUCCESS != test_equality(p1[i], p2[i])) {
                printf("FAILED\n");
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
    test_sampling_compatibility_fisher_yates_vs_sendrier();
    test_sampling_compatibility_fisher_yates_vs_sendrier_AVX2();
    test_sampling_compatibility_djbsort_vs_djbsort_AVX2();
    test_sampling_compatibility_natural_vs_natural_AVX2();

    printf("\nDone\n");

    return 0;
}

