
#include <stdio.h>

#include "../src/sampling/fisher_yates.h"
#include "../src/sampling/fisher_yates_sendrier.h"
#include "../src/sampling/djbsort_sample.h"
#include "../src/sampling/AVX2/djbsort_sample_AVX2.h"
#include "../src/sampling/AVX2/fisher_yates_natural_AVX2.h"
#include "../src/sampling/AVX2/fisher_yates_sendrier_AVX2.h"
#include "../src/verification_AVX2.h"
#include "../src/verification.h"

static int test_non_triviality(const perm_t p){

    int sum = 1;
    for (int i = 0; i < PARAM_N; ++i) {
        sum &= (p[i] == i);
    }
    return (sum == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int test_non_triviality_avx2(const permAVX_t p){

    int sum = 1;
    for (int i = 0; i < PARAM_N; ++i) {
        sum &= (p.i[i] == i);
    }
    return (sum == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int test_fisher_yates_non_ct(){

    // Fisher Yates non-constant time
    uint16_t seed[SEED_BYTES / 2] = {0};
    perm_t p[N_PERMUTATIONS];

    printf("Test Fisher-Yates non-constant time ... ");
    for (int j = 0; j <N_ITERATIONS; ++j) {
        for (int i = 0; i < N_PERMUTATIONS; ++i) {
            seed[0] = i;
            perm_set_random_fisher_yates_non_ct(p[i], (uint8_t *) seed);
        }
        for (int i = 0; i < N_PERMUTATIONS; ++i) {
            if(verify_permutation(p[i])!= EXIT_SUCCESS){
                printf("FAILED\n");
                return EXIT_FAILURE;
            }
            if (test_non_triviality(p[i]) != EXIT_SUCCESS){
                printf("FAILED - Trivial permutation\n");
                return EXIT_FAILURE;
            }
        }
    }

    printf("PASSED\n");
    return EXIT_SUCCESS;
}

static int test_djbsort(){

    // Djbsort
    uint16_t seed[SEED_BYTES / 2] = {0};
    perm_t p[N_PERMUTATIONS];

    printf("Test Djbsort ...                        ");
    for (int j = 0; j <N_ITERATIONS; ++j) {
        for (int i = 0; i < N_PERMUTATIONS; ++i) {
            seed[0] = i;
            perm_set_random_djbsort(p[i], (uint8_t *) seed);
        }
        for (int i = 0; i < N_PERMUTATIONS; ++i) {
            if(verify_permutation(p[i])!= EXIT_SUCCESS){
                printf("FAILED\n");
                return EXIT_FAILURE;
            }
            if (test_non_triviality(p[i]) != EXIT_SUCCESS){
                printf("FAILED - Trivial permutation\n");
                return EXIT_FAILURE;
            }
        }
    }

    printf("PASSED\n");
    return EXIT_SUCCESS;
}

static int test_fisher_yates_sendrier(){

    // Sendrier Fisher-Yates
    uint16_t seed[SEED_BYTES / 2] = {0};
    perm_t p[N_PERMUTATIONS];

    printf("Test Fisher-Yates Sendrier ...          ");
    for (int j = 0; j <N_ITERATIONS; ++j) {
        for (int i = 0; i < N_PERMUTATIONS; ++i) {
            seed[0] = i;
            perm_set_random_sendrier(p[i], (uint8_t *) seed);
        }
        for (int i = 0; i < N_PERMUTATIONS; ++i) {
            if(verify_permutation(p[i])!= EXIT_SUCCESS){
                printf("FAILED\n");
                return EXIT_FAILURE;
            }
            if (test_non_triviality(p[i]) != EXIT_SUCCESS){
                printf("FAILED - Trivial permutation\n");
                return EXIT_FAILURE;
            }
        }
    }

    printf("PASSED\n");
    return EXIT_SUCCESS;
}

static int test_fisher_yates_natural_FY(){

    // NATURAL Fisher Yates
    perm_t p[N_PERMUTATIONS];
    uint16_t seed[SEED_BYTES / 2] = {0};

    printf("Test Fisher-Yates Natural ...           ");
    for (int j = 0; j <N_ITERATIONS; ++j) {

        for (int i = 0; i < N_PERMUTATIONS; ++i) {
            seed[0] = i;
            perm_set_random_natural(p[i], (uint8_t *) seed);
        }
        for (int i = 0; i < N_PERMUTATIONS; ++i) {
            if(verify_permutation(p[i])!= EXIT_SUCCESS){
                printf("FAILED\n");
                return EXIT_FAILURE;
            }
            if (test_non_triviality(p[i]) != EXIT_SUCCESS){
                printf("FAILED - Trivial permutation\n");
                return EXIT_FAILURE;
            }
        }
    }

    printf("PASSED\n");
    return EXIT_SUCCESS;
}

static int test_djbsort_AVX2(){

    // Djbsort AVX2
    perm_t p[N_PERMUTATIONS];
    uint16_t seed[SEED_BYTES / 2] = {0};

    printf("Test Djbsort AVX2 ...                   ");
    for (int j = 0; j <N_ITERATIONS; ++j) {

        for (int i = 0; i < N_PERMUTATIONS; ++i) {
            seed[0] = i;
            perm_set_random_djbsort_avx2(p[i], (uint8_t *) seed);
        }

        for (int i = 0; i < N_PERMUTATIONS; ++i) {
            if(verify_permutation(p[i])!= EXIT_SUCCESS){
                printf("FAILED\n");
                return EXIT_FAILURE;
            }
            if (test_non_triviality(p[i]) != EXIT_SUCCESS){
                printf("FAILED - Trivial permutation\n");
                return EXIT_FAILURE;
            }
        }
    }

    printf("PASSED\n");
    return EXIT_SUCCESS;
}

static int test_fisher_yates_natural_AVX2(){
    
    // Fisher Yates Natural AVX2
    uint16_t seed[SEED_BYTES / 2] = {0};
    perm_t p[N_PERMUTATIONS];

    printf("Test Fisher-Yates Natural AVX2 ...      ");
    for (int j = 0; j <N_ITERATIONS; ++j) {

        for (int i = 0; i < N_PERMUTATIONS; ++i) {
            seed[0] = i;
            perm_set_random_fisher_yates_natural_avx2(p[i], (uint8_t *) seed);
        }
        for (int i = 0; i < N_PERMUTATIONS; ++i) {
            if(verify_permutation(p[i])!= EXIT_SUCCESS){
                printf("FAILED\n");
                return EXIT_FAILURE;
            }
            if (test_non_triviality(p[i]) != EXIT_SUCCESS){
                printf("FAILED - Trivial permutation\n");
                return EXIT_FAILURE;
            }
        }
    }

    printf("PASSED\n");
    return EXIT_SUCCESS;
}


static int test_fisher_yates_sendrier_AVX2(){
    
    // Fisher-Yates Sendrier AVX2
    uint16_t seed[SEED_BYTES / 2] = {0};
    permAVX_t p[N_PERMUTATIONS];

    printf("Test Fisher-Yates Sendrier AVX2 ...     ");
    for (int j = 0; j < N_ITERATIONS; ++j) {

        for (int i = 0; i < N_PERMUTATIONS; ++i) {
            seed[0] = i;
            perm_set_random_sendrier_avx2(&p[i], (uint8_t *) seed);
        }
        for (int i = 0; i < N_PERMUTATIONS; ++i) {
            if(verify_permutation_avx2(&p[i])!= EXIT_SUCCESS){
                printf("FAILED\n");
                return EXIT_FAILURE;
            }
            if (test_non_triviality_avx2(p[i]) != EXIT_SUCCESS){
                printf("FAILED - Trivial permutation\n");
                return EXIT_FAILURE;
            }
        }
    }

    printf("PASSED\n");
    return EXIT_SUCCESS;
}


int main() {

    printf("Start test permutation sampling. Sample %d permutations of length %d, repeat %d times\n\n", N_PERMUTATIONS, PARAM_N, N_ITERATIONS);fflush(stdout);

    test_djbsort();
    test_djbsort_AVX2();
    test_fisher_yates_natural_FY();
    test_fisher_yates_natural_AVX2();
    test_fisher_yates_sendrier();
    test_fisher_yates_sendrier_AVX2();
    test_fisher_yates_non_ct();

    printf("\nDone\n");

    return 0;
}


