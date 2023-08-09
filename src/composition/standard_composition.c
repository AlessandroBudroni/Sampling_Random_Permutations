//
// Created by Alessandro Budroni on 08/07/2023.
//

#include "../definitions.h"

#include <string.h>

void perm_compose(perm_t out, const perm_t in1, const perm_t in2){

    perm_t tmp;
    memcpy(tmp, in1, sizeof(uint16_t)*PARAM_N);
    for (int i = 0; i < PARAM_N; ++i) {
        out[i] = tmp[in2[i]];
    }
}

