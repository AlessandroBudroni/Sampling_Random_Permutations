//
// Created by Alessandro Budroni on 20/07/2023.
//

#include "../api.h"
#include "../sorting/generic_sort.h"

void perm_inverse(perm_t o, const perm_t p) {
    uint32_t i;
    uint32_t buffer[PARAM_N];

    // set buffer as p[i] || i, for i=0...PARAM_N1
    for (i = 0; i < PARAM_N; i++) {
        buffer[i] = p[i];
        buffer[i] <<= 16;
        buffer[i] |= i;
    }

    // sort
    common_sort_uint32(buffer, PARAM_N);

    // extract permutation from buffer
    for (i = 0; i < PARAM_N; i++) {
        o[i] = (uint16_t)(buffer[i]);
    }

}

