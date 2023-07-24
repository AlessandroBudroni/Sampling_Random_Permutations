//
// Created by Alessandro Budroni on 08/07/2023.
//

#include "../src/sampling/AVX2/definitions_AVX2.h"
#include "test_permutation_utils_AVX2.h"

#include <stdio.h>

// return EXIT_SUCCESS if the permutation is valid, EXIT_FAILURE otherwise
int validate_permutation_avx2(permAVX_t *p) {

    uint16_t verification[PARAM_N] = {0};

    // verify that the values are all smaller than PARAM_N
    for (int i = 0; i < PARAM_N; ++i) {
        if (p->i[i] > PARAM_N){
            printf("Verification failure with index %d and value %d\n", i, p->i[i]);
            return EXIT_FAILURE;
        }
        verification[p->i[i]]++;
    }

    // verify that there are no repetitions
    for (int i = 0; i < PARAM_N; ++i) {
        if (verification[i] != 1) {
            printf("Verification failure at %d rep %d\n", i, p->i[i]);
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

// return EXIT_SUCCESS if the permutation is not trivial, EXIT_FAILURE otherwise
int test_non_triviality_avx2(const permAVX_t p){

    int sum = 1;
    for (int i = 0; i < PARAM_N; ++i) {
        sum &= (p.i[i] == i);
    }
    return (sum == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

// return EXIT_SUCCESS if the permutations are equal, EXIT_FAILURE otherwise
int test_equality_avx2(const perm_t p1, const permAVX_t p2) {

    for (int i = 0; i < PARAM_N; ++i) {
        if (p1[i] != p2.i[i]){
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}