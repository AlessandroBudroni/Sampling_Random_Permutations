
#include "common_shuffle.h"
#include "../macros.h"

void common_shuffle(perm_t output, uniform_indexes_t input) {
    for (int i = PARAM_N - 1; i >= 0; --i) {
        uint16_t pi = input[i];
        output[i] = input[i];
        for (int j = i + 1; j < PARAM_N; ++j) { // p[j] = (p[j] == p[i]) ? i : p[j];
            uint16_t mask = ISZERO(output[j] - pi);
            output[j] = MASKAPPLY(mask-1, i, output[j]);
        }
    }
}
