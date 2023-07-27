//
// Created by Alessandro Budroni on 08/07/2023.
//

#include "../api.h"

void perm_inverse(perm_t out, const perm_t in) {

    for (int i = 0; i < PARAM_N; ++i) {
        out[in[i]] = i;
    }
}

