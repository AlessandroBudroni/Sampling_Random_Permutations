//
// Created by Alessandro Budroni on 06/07/2023.
//

#include "verification.h"
#include <stdio.h>

int verify_permutation(const perm_t p) {

    uint16_t verification[PARAM_N] = {0};

    //  check that the entries are all below PARAM_N
    for (int i = 0; i < PARAM_N; ++i) {
        if (p[i] > PARAM_N){
            printf("Verification failure with index %d and value %d\n", i, p[i]);
            return EXIT_FAILURE;
        }
        verification[p[i]]++;
    }

    // check that there are no repetitions
    for (int i = 0; i < PARAM_N; ++i) {
        if (verification[i] != 1) {
            printf("Verification failure at %d rep %d\n", i, p[i]);
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}