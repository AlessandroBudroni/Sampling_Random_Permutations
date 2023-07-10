//
// Created by Alessandro Budroni on 08/07/2023.
//

#include "inversion_substitution.h"


void perm_invert_substitution_non_ct(perm_t out, perm_t in) {

    for (int i = 0; i < PARAM_N; ++i) {
        in[out[i]] = i;
    }
}
