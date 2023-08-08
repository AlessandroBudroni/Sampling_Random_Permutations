
#include <api.h>
#include "test_utils.h"

#include <xkcp/SimpleFIPS202.h>
#include <stdlib.h>
#include <stdio.h>

void print_buffer(char * msg, uint8_t kat[SEED_BYTES]) {
    printf("%s ", msg);
    for (int i = 0; i < SEED_BYTES; i++) {
        printf("%02X", kat[i]);
    }
    printf("\n");
}

int main() {

    printf("Simple KAT test for scheme %s\n", scheme_name());
    fflush(stdout);

    // Fisher Yates non-constant time
    uint8_t seed[SEED_BYTES] = {0};
    uint8_t kat[SEED_BYTES] = {0};
    perm_t p;

    // sample permutation
    SHAKE128(seed, sizeof(seed), seed, sizeof(seed));

    perm_set_random(p, seed);
    if (validate_permutation(p) != EXIT_SUCCESS) {
        printf("FAILED - Invalid permutation\n");
        return EXIT_FAILURE;
    }
    SHAKE128(kat, sizeof(kat), (uint8_t *)p, sizeof(p));
    print_buffer("kat: ", kat);

    printf("PASSED\n");
    return EXIT_SUCCESS;
}


