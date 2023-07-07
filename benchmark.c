
#include <stdio.h>

#include "src/utils.h"
#include "src/fisher_yates.h"
#include "src/fisher_yates_sendrier.h"
#include "src/djbsort_sample.h"
#include "src/verification.h"
#include <time.h>

static void bench_fisher_yates_non_ct(int n_permutations){

    unsigned long long cycles_tot, cycles1, cycles2;
    double start, end;
    uint16_t seed[SEED_BYTES / 2] = {0};
    double time_taken;

    // Fisher Yates non-constant time

    perm_t p[n_permutations];

    cycles_tot = 0;
    time_taken = 0;

    for (int j = 0; j <N_ITERATIONS; ++j) {
        start = (double) clock();
        cycles1 = cpucycles();

        for (int i = 0; i < n_permutations; ++i) {
            seed[0] = i;
            perm_set_random_fisher_yates_non_ct(p[i], (uint8_t *) seed);
        }

        cycles2 = cpucycles();
        end = (double)clock();
        time_taken += (end - start) / ((double) CLOCKS_PER_SEC);
        cycles_tot+= cycles2 - cycles1;
        for (int i = 0; i < n_permutations; ++i) {
            verify_permutation(p[i]);
        }
    }

    printf("Fisher Yates non-constant time ..............................%10lld ", cycles_tot);
    printf("cycles");
    printf("\n");
    printf("Time taken %lf\n\n", time_taken);
}

static void bench_djbsort(int n_permutations){

    unsigned long long cycles_tot, cycles1, cycles2;
    double start, end;
    uint16_t seed[SEED_BYTES / 2] = {0};
    double time_taken;

    // Djbsort

    perm_t p[n_permutations];

    cycles_tot = 0;
    time_taken = 0;

    for (int j = 0; j <N_ITERATIONS; ++j) {
        start = (double) clock();
        cycles1 = cpucycles();
        for (int i = 0; i < n_permutations; ++i) {
            seed[0] = i;
            perm_set_random_djbsort(p[i], (uint8_t *) seed);
        }

        cycles2 = cpucycles();
        end = (double)clock();
        time_taken += (end - start) / ((double) CLOCKS_PER_SEC);
        cycles_tot+= cycles2 - cycles1;
        for (int i = 0; i < n_permutations; ++i) {
            verify_permutation(p[i]);
        }
    }

    printf("Djbsort .................................................... %10lld ", cycles_tot);
    printf("cycles");
    printf("\n");
    printf("Time taken %lf\n\n", time_taken);
}

static void bench_fisher_yates_sendrier(int n_permutations){

    unsigned long long cycles_tot, cycles1, cycles2;
    double start, end;
    uint16_t seed[SEED_BYTES / 2] = {0};
    double time_taken;

    // Sendrier Fisher Yates

    perm_t p[n_permutations];

    cycles_tot = 0;
    time_taken = 0;

    for (int j = 0; j <N_ITERATIONS; ++j) {
        start = (double) clock();
        cycles1 = cpucycles();

        for (int i = 0; i < n_permutations; ++i) {
            seed[0] = i;
            perm_set_random_sendrier(p[i], (uint8_t *) seed);
        }

        cycles2 = cpucycles();
        end = (double)clock();
        time_taken += (end - start) / ((double) CLOCKS_PER_SEC);
        cycles_tot+= cycles2 - cycles1;
        for (int i = 0; i < n_permutations; ++i) {
            verify_permutation(p[i]);
        }
    }

    printf("Sendrier Fisher Yates ...................................... %10lld ", cycles_tot);
    printf("cycles");
    printf("\n");
    printf("Time taken %lf\n\n", time_taken);
}

void bench_fisher_yates_ternary(int n_permutations){

    perm_t p[n_permutations];

    unsigned long long cycles_tot, cycles1, cycles2;
    double start, end;
    uint16_t seed[SEED_BYTES / 2] = {0};
    double time_taken;

    // TERNARY Fisher Yates

    cycles_tot = 0;
    time_taken = 0;

    for (int j = 0; j <N_ITERATIONS; ++j) {
        start = (double) clock();
        cycles1 = cpucycles();

        for (int i = 0; i < n_permutations; ++i) {
            seed[0] = i;
            perm_set_random_sendrier_ternary_operator(p[i], (uint8_t *) seed);
        }
        cycles2 = cpucycles();
        end = (double)clock();
        time_taken += (end - start) / ((double) CLOCKS_PER_SEC);
        cycles_tot+= cycles2 - cycles1;
        for (int i = 0; i < n_permutations; ++i) {
            verify_permutation(p[i]);
        }
    }

    printf("Ternary Fisher Yates ....................................... %10lld ", cycles_tot);
    printf("cycles");
    printf("\n");
    printf("Time taken %lf\n\n", time_taken);
}

void bench_fisher_yates_natural_FY(int n_permutations){

    perm_t p[n_permutations];

    unsigned long long cycles_tot, cycles1, cycles2;
    double start, end;
    uint16_t seed[SEED_BYTES / 2] = {0};
    double time_taken;

    // NATURAL Fisher Yates

    cycles_tot = 0;
    time_taken = 0;

    for (int j = 0; j <N_ITERATIONS; ++j) {
        start = (double) clock();
        cycles1 = cpucycles();

        for (int i = 0; i < n_permutations; ++i) {
            seed[0] = i;
            perm_set_random_natural(p[i], (uint8_t *) seed);
        }
        cycles2 = cpucycles();
        end = (double)clock();
        time_taken += (end - start) / ((double) CLOCKS_PER_SEC);
        cycles_tot+= cycles2 - cycles1;
        for (int i = 0; i < n_permutations; ++i) {
            verify_permutation(p[i]);
        }
    }

    printf("Natural Fisher Yates ....................................... %10lld ", cycles_tot);
    printf("cycles");
    printf("\n");
    printf("Time taken %lf\n\n", time_taken);
}

/*
void bench_fisher_yates_bike(int n_permutations){

    perm_t p[n_permutations];

    unsigned long long cycles_tot, cycles1, cycles2;
    double start, end;
    uint16_t seed[SEED_BYTES / 2] = {0};
    double time_taken;

    // BIKE Fisher Yates

    cycles_tot = 0;
    time_taken = 0;

    for (int j = 0; j <N_ITERATIONS; ++j) {
        start = (double) clock();
        cycles1 = cpucycles();

        for (int i = 0; i < n_permutations; ++i) {
            seed[0] = i;
            perm_set_random_bike(p[i], (uint8_t *) seed);
        }
        cycles2 = cpucycles();
        end = (double)clock();
        time_taken += (end - start) / ((double) CLOCKS_PER_SEC);
        cycles_tot+= cycles2 - cycles1;
        for (int i = 0; i < n_permutations; ++i) {
            verify_permutation(p[i]);
        }
    }

    printf("Sendrier Fisher BIKE ........................................ %10lld ", cycles_tot);
    printf("cycles");
    printf("\n");
    printf("Time taken %lf\n\n", time_taken);
}

*/

int main() {

    printf("\nWarming up... ");fflush(stdout);
    double time_taken = warm_up();
    printf("Done\n");
    printf("Time taken %lf\n\n", time_taken);


    printf("Start benchmark. Sample %d permutations of length %d, repeat %d times\n\n", N_PERMUTATIONS, PARAM_N1, N_ITERATIONS);fflush(stdout);

    bench_djbsort(N_PERMUTATIONS);
    bench_fisher_yates_ternary(N_PERMUTATIONS);
    bench_fisher_yates_natural_FY(N_PERMUTATIONS);
    bench_fisher_yates_sendrier(N_PERMUTATIONS);
    bench_fisher_yates_non_ct(N_PERMUTATIONS);
//    bench_fisher_yates_bike(N_PERMUTATIONS);


    printf("\nDone\n");
    
    return 0;
}


