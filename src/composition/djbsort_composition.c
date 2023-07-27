//
// Created by Alessandro Budroni on 08/07/2023.
//

#include "../api.h"
#include <djbsort/djbsort.h>

void perm_compose(perm_t out, const perm_t in1, const perm_t in2) {
    uint32_t i;
    uint32_t buffer[PARAM_N];
    perm_t tmp;

    perm_inverse(tmp, in2);
    // set buffer as tmp[i] || i, for i=0...PARAM_N1
    for (i = 0; i < PARAM_N; i++) {
        buffer[i] = tmp[i];
        buffer[i] <<= 16;
        buffer[i] |= in1[i];
    }

    // sort
    uint32_sort(buffer, PARAM_N);

    // extract permutation from buffer
    for (i = 0; i < PARAM_N; i++) {
        out[i] = (uint16_t)(buffer[i]);
    }
}

// compose in1 with inv(in2)
void perm_compose_with_inverse_djbsort(perm_t out, const perm_t in1, const perm_t in2) {
    uint32_t i;
    uint32_t buffer[PARAM_N];

    // set buffer as tmp[i] || i, for i=0...PARAM_N1
    for (i = 0; i < PARAM_N; i++) {
        buffer[i] = in2[i];
        buffer[i] <<= 16;
        buffer[i] |= in1[i];
    }

    // sort
    uint32_sort(buffer, PARAM_N);

    // extract permutation from buffer
    for (i = 0; i < PARAM_N; i++) {
        out[i] = (uint16_t)(buffer[i]);
    }
}
