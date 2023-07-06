//
// Created by Alessandro Budroni on 06/07/2023.
//

#include "verification.h"
#include <stdio.h>

int verify_permutation(const perm_t p) {

    uint16_t verification[PARAM_N1] = {0};

    for (int i = 0; i < PARAM_N1; ++i) {
        if (p[i] > PARAM_N1){
            printf("Verification failure with index %d and value %d\n", i, p[i]);
            return EXIT_FAILURE;
        }
        verification[p[i]]++;
    }

    for (int i = 0; i < PARAM_N1; ++i) {
        if (verification[i] != 1) {
            printf("Verification failure at %d rep %d\n", i, p[i]);
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}