//
// Created by Alessandro Budroni on 08/07/2023.
//

#include "composition_substitution.h"

void perm_compose_substitution_non_ct(perm_t out, const perm_t in1, const perm_t in2){

    for (int i = 0; i < PARAM_N; ++i) {
        out[i] = in1[in2[i]];
    }
}

void perm_compose_substitution_ct(perm_t out, const perm_t in1, const perm_t in2){

    for (int i = 0; i < PARAM_N; ++i) {
        for (int j = 0; j < PARAM_N; ++j) {
            out[i] =
        }
        out[i] = in1[in2[i]];
    }
}