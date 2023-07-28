//
// Created by perin on 7/27/23.
//

#include "../common_fisher_yates.h"
#include <string.h>

void perm_compose(perm_t out, const perm_t in1, const perm_t in2){

    perm_t tmp_perm;
    memcpy(tmp_perm, in1, sizeof(uint16_t)*PARAM_N);
    uint16_t mask, tmp_value;
    for (int i = 0; i < PARAM_N; i++) {
        tmp_value = tmp_perm[i];
        for (int j = 0; j < PARAM_N; ++j) {  //buffer[i] = in1[in2[i]];
            mask = ISZERO(i - in2[j]) -1;
            out[j] = MASKAPPLY(mask, tmp_value, out[j]);
        }
    }

}
