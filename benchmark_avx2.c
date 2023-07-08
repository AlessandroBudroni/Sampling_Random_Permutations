
#include <stdio.h>

#include "src/utils.h"
#include "src/definitions.h"
#include "src/fisher_yates.h"
#include "src/AVX2/djbsort_sample_AVX2.h"
#include "src/AVX2/fisher_yates_natural_AVX2.h"
#include "src/verification.h"
#include <time.h>

static void bench_fisher_yates_non_ct(){

    unsigned long long cycles_tot, cycles1, cycles2;
    double start, end;
    uint16_t seed[SEED_BYTES / 2] = {0};
    double time_taken;

    // Fisher Yates non-constant time

    perm_t p[N_PERMUTATIONS];

    cycles_tot = 0;
    time_taken = 0;

    for (int j = 0; j <N_ITERATIONS; ++j) {
        start = (double) clock();
        cycles1 = cpucycles();

        for (int i = 0; i < N_PERMUTATIONS; ++i) {
            seed[0] = i;
            perm_set_random_fisher_yates_non_ct(p[i], (uint8_t *) seed);
        }

        cycles2 = cpucycles();
        end = (double)clock();
        time_taken += (end - start) / ((double) CLOCKS_PER_SEC);
        cycles_tot+= cycles2 - cycles1;
        for (int i = 0; i < N_PERMUTATIONS; ++i) {
            verify_permutation(p[i]);
        }
    }

    printf("Fisher Yates non-constant time ..............................%10lld ", cycles_tot);
    printf("cycles");
    printf("\n");
    printf("Time taken %lf\n\n", time_taken);
}


static void bench_djbsort_AVX2(){

    unsigned long long cycles_tot, cycles1, cycles2;
    double start, end;
    uint16_t seed[SEED_BYTES / 2] = {0};
    double time_taken;

    /* Djbsort */

    perm_t p[N_PERMUTATIONS];
    cycles_tot = 0;
    time_taken = 0;

    for (int j = 0; j <N_ITERATIONS; ++j) {
        start = (double)clock();
        cycles1 = cpucycles();

        for (int i = 0; i < N_PERMUTATIONS; ++i) {
            seed[0] = i;
            perm_set_random_djbsort_avx2(p[i], (uint8_t *) seed);
        }

        cycles2 = cpucycles();
        end = (double)clock();
        time_taken += (end - start) / ((double) CLOCKS_PER_SEC);
        cycles_tot+= cycles2 - cycles1;
        for (int i = 0; i < N_PERMUTATIONS; ++i) {
            verify_permutation(p[i]);
        }
    }

    printf("Djbsort AVX ................................................ %10lld ", cycles_tot);
    printf("cycles");
    printf("\n");
    printf("Time taken %lf\n\n", time_taken);
}

void bench_fisher_yates_natural_AVX2(){

    unsigned long long cycles_tot, cycles1, cycles2;
    double start, end;
    uint16_t seed[SEED_BYTES / 2] = {0};
    double time_taken;

    // AVX Fisher Yates Natural

    perm_t pu[N_PERMUTATIONS];

    cycles_tot = 0;
    time_taken = 0;

    for (int j = 0; j <N_ITERATIONS; ++j) {
        start = (double) clock();
        cycles1 = cpucycles();

        for (int i = 0; i < N_PERMUTATIONS; ++i) {
            seed[0] = i;
            perm_set_random_fisher_yates_natural_avx2(pu[i], (uint8_t *) seed);
        }
        cycles2 = cpucycles();
        end = (double)clock();
        time_taken += (end - start) / ((double) CLOCKS_PER_SEC);
        cycles_tot+= cycles2 - cycles1;
        for (int i = 0; i < N_PERMUTATIONS; ++i) {
            verify_permutation(pu[i]);
        }
    }

    printf("AVX Natural Fisher Yates ................................... %10lld ", cycles_tot);
    printf("cycles");
    printf("\n");
    printf("Time taken %lf\n\n", time_taken);

}

/*
void bench_fisher_yates_sendrier_FY_AVX(int n_permutations){

    unsigned long long cycles_tot, cycles1, cycles2;
    double start, end;
    uint16_t seed[SEED_BYTES / 2] = {0};
    double time_taken;

    // AVX Fisher Yates

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

*/


int main() {

    printf("\nWarming up... ");fflush(stdout);
    double time_taken = warm_up();
    if (time_taken == 0){
        printf("Skipping warm up? TOO BAD\n");
    }
    printf("Done\n");
    printf("Time taken %lf\n\n", time_taken);

    printf("Start AVX benchmark. Sample %d permutations, repeat %d times\n\n", N_PERMUTATIONS, N_ITERATIONS);fflush(stdout);

    bench_fisher_yates_non_ct();
    bench_fisher_yates_natural_AVX2();
    bench_djbsort_AVX2();

    printf("\nDone\n");
    
    return 0;
}


