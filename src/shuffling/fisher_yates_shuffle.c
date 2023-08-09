
#include "common_shuffle.h"


void common_shuffle(perm_t output, uniform_indexes_t input) {
    for (int i = PARAM_N - 1; i >= 0; --i){
        output[i] = output[input[i]];
        output[input[i]] = i;
    }
}
