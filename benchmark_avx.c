
#include <stdio.h>

#include "include/utils.h"
#include "include/definitions.h"
#include "include/fisher_yates.h"
#include "include/sendrier_AVX2.h"
#include "include/fisher_yates_AVX2.h"
#include "include/djbsort_sample_avx.h"
#include <time.h>

void bench_djbsort_AVX(int n_permutations){

    unsigned long long cycles_tot, cycles1, cycles2;
    double start, end;
    uint16_t seed[SEED_BYTES / 2] = {0};
    double time_taken;

    /* Djbsort */

    perm_t p[n_permutations];
    cycles_tot = 0;
    time_taken = 0;

    for (int j = 0; j <N_ITERATIONS; ++j) {
        start = (double)clock();
        cycles1 = cpucycles();

        for (int i = 0; i < n_permutations; ++i) {
            seed[0] = i;
            perm_set_random_djbsort_avx(p[i], (uint8_t *) seed);
        }

        cycles2 = cpucycles();
        end = (double)clock();
        time_taken += (end - start) / ((double) CLOCKS_PER_SEC);
        cycles_tot+= cycles2 - cycles1;
        for (int i = 0; i < n_permutations; ++i) {
            verify_permutation(p[i]);
        }
    }

    printf("Djbsort AVX ................................................ %10lld ", cycles_tot);
    printf("cycles");
    printf("\n");
    printf("Time taken %lf\n\n", time_taken);
}

void bench_fisher_yates_sendrier_FY_AVX(int n_permutations){

    unsigned long long cycles_tot, cycles1, cycles2;
    double start, end;
    uint16_t seed[SEED_BYTES / 2] = {0};
    double time_taken;

    /* AVX Fisher Yates */

    permAVX_t pu[n_permutations];

    cycles_tot = 0;
    time_taken = 0;

    for (int j = 0; j <N_ITERATIONS; ++j) {
        start = (double) clock();
        cycles1 = cpucycles();

        for (int i = 0; i < n_permutations; ++i) {
            seed[0] = i;
            perm_set_random_sendrier_avx(&pu[i], (uint8_t *) seed);
        }
        cycles2 = cpucycles();
        end = (double)clock();
        time_taken += (end - start) / ((double) CLOCKS_PER_SEC);
        cycles_tot+= cycles2 - cycles1;
        for (int i = 0; i < n_permutations; ++i) {
            verify_permutation_avx(&pu[i]);
        }
    }

    printf("AVX Sendrier Fisher Yates .................................. %10lld ", cycles_tot);
    printf("cycles");
    printf("\n");
    printf("Time taken %lf\n\n", time_taken);

}

void bench_fisher_yates_natural_FY_AVX(int n_permutations){

    unsigned long long cycles_tot, cycles1, cycles2;
    double start, end;
    uint16_t seed[SEED_BYTES / 2] = {0};
    double time_taken;

    /* AVX Fisher Yates Natural*/

    perm_t pu[n_permutations];

    cycles_tot = 0;
    time_taken = 0;

    for (int j = 0; j <N_ITERATIONS; ++j) {
        start = (double) clock();
        cycles1 = cpucycles();

        for (int i = 0; i < n_permutations; ++i) {
            seed[0] = i;
            perm_set_random_natural_avx(pu[i], (uint8_t *) seed);
        }
        cycles2 = cpucycles();
        end = (double)clock();
        time_taken += (end - start) / ((double) CLOCKS_PER_SEC);
        cycles_tot+= cycles2 - cycles1;
        for (int i = 0; i < n_permutations; ++i) {
            verify_permutation(pu[i]);
        }
    }

    printf("AVX Natural Fisher Yates ................................... %10lld ", cycles_tot);
    printf("cycles");
    printf("\n");
    printf("Time taken %lf\n\n", time_taken);

}

int main() {

    printf("\nWarming up... ");fflush(stdout);
    double time_taken = warm_up();
    if (time_taken == 0){
        printf("Skipping warm up? TOO BAD\n");
    }
    printf("Done\n");
    printf("Time taken %lf\n\n", time_taken);

    printf("Start AVX benchmark. Sample %d permutations, repeat %d times\n\n", N_PERMUTATIONS, N_ITERATIONS);fflush(stdout);

    bench_fisher_yates_sendrier_FY_AVX(N_PERMUTATIONS);
    bench_fisher_yates_natural_FY_AVX(N_PERMUTATIONS);
    bench_djbsort_AVX(N_PERMUTATIONS);

    printf("\nDone\n");
    
    return 0;
}


