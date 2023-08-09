#include "test_utils.h"

#include <xkcp/SimpleFIPS202.h>
#include <djbsort/djbsort.h>
#include <stdlib.h>
#include <stdio.h>

extern void vqsort_avx2_u32_asc(uint32_t* data, size_t len);
extern void vqsort_avx2_u64_asc(uint64_t* data, size_t len);

void test_uint32_sort() {
    uint32_t data_djb[PARAM_N] = {0};
    uint32_t data_vqs[PARAM_N] = {0};
    uint8_t seed[SEED_BYTES] = {0};

    SHAKE128((uint8_t *)data_djb, sizeof(data_djb), seed, sizeof(seed));
    memcpy(data_vqs, data_djb, sizeof(data_djb));

    uint32_sort(data_djb, PARAM_N);
    vqsort_avx2_u32_asc(data_vqs, PARAM_N);

    uint32_t previous_djb = 0;
    uint32_t previous_vqs = 0;
    for (int i = 0; i < PARAM_N; i++) {
        // Test djbsort ordering
        if (data_djb[i] < previous_djb ) {
            printf("djbsort32 no good..\n");
            exit(EXIT_FAILURE);
        }
        previous_djb = data_djb[i];

        // Test vqsort ordering
        if (data_vqs[i] < previous_vqs ) {
            printf("vqsort32 no good..\n");
            exit(EXIT_FAILURE);
        }
        previous_vqs = data_vqs[i];

        // Test compatibility
        if (data_djb[i] != data_vqs[i]) {
            printf("sorting 32 not compatible!\n");
            exit(EXIT_FAILURE);
        }
    }
    printf("sort 32 ok!\n");
}


void test_uint64_sort() {
    uint64_t data_djb[PARAM_N] = {0};
    uint64_t data_vqs[PARAM_N] = {0};
    uint8_t seed[SEED_BYTES] = {0};

    SHAKE128((uint8_t *)data_djb, sizeof(data_djb), seed, sizeof(seed));
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
}

int main() {
    test_uint32_sort();
    test_uint64_sort();
}