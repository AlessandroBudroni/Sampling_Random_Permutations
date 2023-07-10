//
// Created by Alessandro Budroni on 08/07/2023.
//

#include "sampling/AVX2/definitions_AVX2.h"
#include "verification_AVX2.h"

#include <stdio.h>

int verify_permutation_avx2(permAVX_t *p) {

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
