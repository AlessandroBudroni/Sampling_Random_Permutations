
#include "common_shuffle.h"
#include "../macros.h"

void common_shuffle(perm_t output, uniform_indexes_t input) {

    for (int i = 0; i < PARAM_N; i++) {
        uint16_t pi = input[i];
        int tmp = i;
        for (int j = 0; j < i; j++) {
            uint16_t mask = GT16(input[j], pi);
            tmp -= mask;
            input[j] -= mask;
        }
        output[i] = pi + tmp;
    }
}
