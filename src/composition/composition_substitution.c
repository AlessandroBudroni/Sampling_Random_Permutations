//
// Created by Alessandro Budroni on 08/07/2023.
//

#include "composition_substitution.h"
#include "../common.h"

void perm_compose_substitution_non_ct(perm_t out, const perm_t in1, const perm_t in2){

    for (int i = 0; i < PARAM_N; ++i) {
        out[i] = in1[in2[i]];
    }
}

void perm_compose_substitution(perm_t out, const perm_t in1, const perm_t in2){

    uint16_t mask, tmp1;
    for (int i = 0; i < PARAM_N; i++) {
        tmp1 = in1[i];
        for (int j = 0; j < PARAM_N; ++j) {  //buffer[i] = in1[in2[i]];
            mask = ISZERO(i - in2[j]) -1;
            out[j] = MASKAPPLY(mask, tmp1, out[j]);
        }
    }

}
