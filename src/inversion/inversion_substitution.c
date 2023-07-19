//
// Created by Alessandro Budroni on 08/07/2023.
//

#include "inversion_substitution.h"
#include "../common.h"

void perm_invert_substitution_non_ct(perm_t out, const perm_t in) {

    for (int i = 0; i < PARAM_N; ++i) {
        out[in[i]] = i;
    }
}

void perm_invert_substitution(perm_t out, const perm_t in) {

    uint16_t mask, tmp;
    for (int i = 0; i < PARAM_N; ++i) {
        tmp = in[i];
        for (int j = 0; j < PARAM_N; ++j) {
            mask = ISZERO(j - tmp)-1;
            out[j] = MASKAPPLY(mask, i, out[j]);
        }
    }
}
