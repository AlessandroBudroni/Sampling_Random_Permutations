#include "test_utils.h"

#include <xkcp/SimpleFIPS202.h>
#include <djbsort/djbsort.h>
#include <stdlib.h>
#include <stdio.h>

extern void vqsort_avx2_u32_asc(uint32_t* data, size_t len);
extern void vqsort_avx2_u64_asc(uint64_t* data, size_t len);

int main() {
    printf("Start sorting test with %s\n", scheme_name());
    uint64_t data_djb[PARAM_N] = {0};
    uint64_t data_vqs[PARAM_N] = {0};
    uint8_t seed[SEED_BYTES] = {0};

    SHAKE128((uint8_t *)data_djb, sizeof(data_djb), seed, sizeof(seed));
#ifdef DJBSORT_IS_MCELIECE
    printf("Using Mceliece Djbsort, sorting up to 63 bits\n");
    for (int i =0; i < PARAM_N; ++i) {
        data_djb[i] &= 0x7FFFFFFFFFFFFFFF;
    }
#endif
    memcpy(data_vqs, data_djb, sizeof(data_djb));

    uint64_sort(data_djb, PARAM_N);
    vqsort_avx2_u64_asc(data_vqs, PARAM_N);

    uint64_t previous_djb = 0;
    uint64_t previous_vqs = 0;

    for (int i = 0; i < PARAM_N; i++) {
        // Test djbsort ordering
        if (data_djb[i] < previous_djb ) {
            printf("djbsort64 no good..\n");
            exit(EXIT_FAILURE);
        }
        previous_djb = data_djb[i];
        // Test vqsort ordering
        if (data_vqs[i] < previous_vqs ) {
            printf("vqsort64 no good..\n");
            exit(EXIT_FAILURE);
        }
        previous_vqs = data_vqs[i];
        // Test compatibility
        if (data_djb[i] != data_vqs[i]) {
            printf("sorting 64 not compatible!\n");
            exit(EXIT_FAILURE);
        }
    }
    printf("sort 64 ok!\n");
    return EXIT_SUCCESS;
}