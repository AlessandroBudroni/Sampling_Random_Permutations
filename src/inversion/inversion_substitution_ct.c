//
// Created by perin on 7/27/23.
//

#include "../definitions.h"
#include "../macros.h"

void perm_inverse(perm_t out, const perm_t in) {

    uint16_t mask, tmp;
    for (int i = 0; i < PARAM_N; ++i) {
        tmp = in[i];
        for (int j = 0; j < PARAM_N; ++j) {
            mask = ISZERO(j - tmp)-1;
            out[j] = MASKAPPLY(mask, i, out[j]);
        }
    }
}
