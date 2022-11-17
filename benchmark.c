
#include <stdio.h>

#include "include/utils.h"
#include "include/fisher_yates.h"
#include "include/djbsort_sample.h"
#include <time.h>

void bench_fisher_yates_sendrier_FY(int n_permutations){

    unsigned long long cycles_tot, cycles1, cycles2;
    double start, end;
    uint16_t seed[SEED_BYTES / 2] = {0};
    double time_taken;

    /* NORMAL Fisher Yates */

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

void bench_fisher_yates_non_ct(int n_permutations){

    unsigned long long cycles_tot, cycles1, cycles2;
    double start, end;
    uint16_t seed[SEED_BYTES / 2] = {0};
    double time_taken;

    /* Djbsort */

    perm_t p[n_permutations];

    cycles_tot = 0;
    time_taken = 0;

    for (int j = 0; j <N_ITERATIONS; ++j) {
        start = (double) clock();
        cycles1 = cpucycles();

        for (int i = 0; i < n_permutations; ++i) {
            seed[0] = i;
            perm_set_random_non_ct(p[i], (uint8_t *) seed);
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

void bench_djbsort(int n_permutations){

    unsigned long long cycles_tot, cycles1, cycles2;
    double start, end;
    uint16_t seed[SEED_BYTES / 2] = {0};
    double time_taken;

    /* Djbsort */

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

void bench_fisher_yates_ternary_FY(int n_permutations){

    perm_t p[n_permutations];

    unsigned long long cycles_tot, cycles1, cycles2;
    double start, end;
    uint16_t seed[SEED_BYTES / 2] = {0};
    double time_taken;
    
    /* TERNARY Fisher Yates */

    cycles_tot = 0;
    time_taken = 0;

    for (int j = 0; j <N_ITERATIONS; ++j) {
        start = (double) clock();
        cycles1 = cpucycles();

        for (int i = 0; i < n_permutations; ++i) {
            seed[0] = i;
            perm_set_random_sendrier_ternary(p[i], (uint8_t *) seed);
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

    /* NATURAL Fisher Yates */

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

void bench_fisher_yates_bike(int n_permutations){

    perm_t p[n_permutations];

    unsigned long long cycles_tot, cycles1, cycles2;
    double start, end;
    uint16_t seed[SEED_BYTES / 2] = {0};
    double time_taken;

    /* NATURAL Fisher Yates */

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


int main() {

    printf("\nWarming up... ");fflush(stdout);
    double time_taken = warm_up();
    printf("Done\n");
    printf("Time taken %lf\n\n", time_taken);


    printf("Start benchmark. Sample %d permutations, repeat %d times\n\n", N_PERMUTATIONS, N_ITERATIONS);fflush(stdout);

    bench_fisher_yates_non_ct(N_PERMUTATIONS);
    bench_fisher_yates_ternary_FY(N_PERMUTATIONS);
    bench_fisher_yates_sendrier_FY(N_PERMUTATIONS);
    bench_fisher_yates_natural_FY(N_PERMUTATIONS);
    bench_fisher_yates_bike(N_PERMUTATIONS);
    bench_djbsort(N_PERMUTATIONS);

    printf("\nDone\n");
    
    return 0;
}


