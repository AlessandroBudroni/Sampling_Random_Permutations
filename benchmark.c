
#include <stdio.h>

#include "src/fisher_yates.h"
#include "src/sendrier_AVX2.h"
#include "src/fisher_yates_natural.h"
#include "src/fisher_yates_natural_AVX2.h"
#include "src/djbsort_sample.h"
#include <time.h>

/* Access system counter for benchmarking */
int64_t cpucycles(void)
{
    unsigned int hi, lo;
    __asm__ volatile ("rdtsc\n\t" : "=a" (lo), "=d"(hi));
    return ((int64_t)lo) | (((int64_t)hi) << 32);
}

#define N_PERMUTATIONS 1000

void bench_fisher_yates_sendrier_FY(int n_permutations){

    unsigned long long cycles_tot, cycles1, cycles2;
    double start, end;
    uint16_t seed[SEED_BYTES / 2] = {0};
    double time_taken;

    /* NORMAL Fisher Yates */

    perm_t p[n_permutations];

    start = (double)clock();
    cycles1 = cpucycles();

    for (int i = 0; i < n_permutations; ++i) {
        seed[0] = i;
        perm_set_random(p[i], (uint8_t *) seed);
    }

    cycles2 = cpucycles();
    end = (double)clock();
    cycles_tot = cycles2 - cycles1;
    time_taken = (end - start) / ((double) CLOCKS_PER_SEC);

    printf("\nVerifying permutations... ");
    for (int i = 0; i < n_permutations; ++i) {
        verify_permutation(p[i]);
    }
    printf("Done\n");
    printf("Original Fisher Yates ...................................... %10lld ", cycles_tot);
    printf("cycles");
    printf("\n");
    printf("Time taken %lf\n", time_taken);
}

void bench_fisher_yates_non_ct(int n_permutations){

    unsigned long long cycles_tot, cycles1, cycles2;
    double start, end;
    uint16_t seed[SEED_BYTES / 2] = {0};
    double time_taken;

    /* Djbsort */

    perm_t p[n_permutations];

    start = (double)clock();
    cycles1 = cpucycles();

    for (int i = 0; i < n_permutations; ++i) {
        seed[0] = i;
        perm_set_random_non_ct(p[i], (uint8_t *) seed);
    }

    cycles2 = cpucycles();
    end = (double)clock();
    cycles_tot = cycles2 - cycles1;
    time_taken = (end - start) / ((double) CLOCKS_PER_SEC);

    printf("\nVerifying permutations... ");
    for (int i = 0; i < n_permutations; ++i) {
        verify_permutation(p[i]);
    }
    printf("Done\n");
    printf("Fisher Yates non-constant time ..............................%10lld ", cycles_tot);
    printf("cycles");
    printf("\n");
    printf("Time taken %lf\n", time_taken);
}

void bench_djbsort(int n_permutations){

    unsigned long long cycles_tot, cycles1, cycles2;
    double start, end;
    uint16_t seed[SEED_BYTES / 2] = {0};
    double time_taken;

    /* Djbsort */

    perm_t p[n_permutations];

    start = (double)clock();
    cycles1 = cpucycles();

    for (int i = 0; i < n_permutations; ++i) {
        seed[0] = i;
        perm_set_random_djbsort(p[i], (uint8_t *) seed);
    }

    cycles2 = cpucycles();
    end = (double)clock();
    cycles_tot = cycles2 - cycles1;
    time_taken = (end - start) / ((double) CLOCKS_PER_SEC);

    printf("\nVerifying permutations... ");
    for (int i = 0; i < n_permutations; ++i) {
        verify_permutation(p[i]);
    }
    printf("Done\n");
    printf("Djbsort .................................................... %10lld ", cycles_tot);
    printf("cycles");
    printf("\n");
    printf("Time taken %lf\n", time_taken);
}

void bench_djbsort_AVX(int n_permutations){

    unsigned long long cycles_tot, cycles1, cycles2;
    double start, end;
    uint16_t seed[SEED_BYTES / 2] = {0};
    double time_taken;

    /* Djbsort */

    perm_t p[n_permutations];

    start = (double)clock();
    cycles1 = cpucycles();

    for (int i = 0; i < n_permutations; ++i) {
        seed[0] = i;
        perm_set_random_djbsort_avx(p[i], (uint8_t *) seed);
    }

    cycles2 = cpucycles();
    end = (double)clock();
    cycles_tot = cycles2 - cycles1;
    time_taken = (end - start) / ((double) CLOCKS_PER_SEC);

    printf("\nVerifying permutations... ");
    for (int i = 0; i < n_permutations; ++i) {
        verify_permutation(p[i]);
    }
    printf("Done\n");
    printf("Djbsort AVX ................................................ %10lld ", cycles_tot);
    printf("cycles");
    printf("\n");
    printf("Time taken %lf\n", time_taken);
}

void bench_fisher_yates_ternary_FY(int n_permutations){

    perm_t p[n_permutations];

    unsigned long long cycles_tot, cycles1, cycles2;
    double start, end;
    uint16_t seed[SEED_BYTES / 2] = {0};
    double time_taken;
    
    /* TERNARY Fisher Yates */

    start = (double)clock();
    cycles1 = cpucycles();

    for (int i = 0; i < n_permutations; ++i) {
        seed[0] = i;
        perm_set_random_sendrier(p[i], (uint8_t *) seed);
    }

    cycles2 = cpucycles();
    end = (double)clock();
    cycles_tot = cycles2 - cycles1;
    time_taken = (end - start) / ((double) CLOCKS_PER_SEC);

    printf("\nVerifying permutations... ");
    for (int i = 0; i < n_permutations; ++i) {
        verify_permutation(p[i]);
    }
    printf("Done\n");
    printf("Ternary Fisher Yates ....................................... %10lld ", cycles_tot);
    printf("cycles");
    printf("\n");
    printf("Time taken %lf\n", time_taken);
}

void bench_fisher_yates_natural_FY(int n_permutations){

    perm_t p[n_permutations];

    unsigned long long cycles_tot, cycles1, cycles2;
    double start, end;
    uint16_t seed[SEED_BYTES / 2] = {0};
    double time_taken;

    /* NATURAL Fisher Yates */

    start = (double)clock();
    cycles1 = cpucycles();

    for (int i = 0; i < n_permutations; ++i) {
        seed[0] = i;
        perm_set_random_natural(p[i], (uint8_t *) seed);
    }

    cycles2 = cpucycles();
    end = (double)clock();
    cycles_tot = cycles2 - cycles1;
    time_taken = (end - start) / ((double) CLOCKS_PER_SEC);

    printf("\nVerifying permutations... ");
    for (int i = 0; i < n_permutations; ++i) {
        verify_permutation(p[i]);
    }
    printf("Done\n");
    printf("Natural Fisher Yates ....................................... %10lld ", cycles_tot);
    printf("cycles");
    printf("\n");
    printf("Time taken %lf\n", time_taken);
}

void bench_fisher_yates_sendrier_FY_AVX(int n_permutations){
    
    unsigned long long cycles_tot, cycles1, cycles2;
    double start, end;
    uint16_t seed[SEED_BYTES / 2] = {0};
    double time_taken;

    /* AVX Fisher Yates */

    permAVX_t pu[n_permutations];

    start = (double)clock();
    cycles1 = cpucycles();

    for (int i = 0; i < n_permutations; ++i) {
        seed[0] = i;
        perm_set_random_avx(&pu[i], (uint8_t *) seed);
    }

    cycles2 = cpucycles();
    end = (double)clock();
    cycles_tot = cycles2 - cycles1;
    time_taken = (end - start) / ((double) CLOCKS_PER_SEC);

    printf("\nVerifying permutations... ");
    for (int i = 0; i < n_permutations; ++i) {
        verify_permutation_avx(&pu[i]);
    }
    printf("Done\n");
    printf("AVX Sendrier Fisher Yates .................................. %10lld ", cycles_tot);
    printf("cycles");
    printf("\n");
    printf("Time taken %lf\n", time_taken);

}

void bench_fisher_yates_natural_FY_AVX(int n_permutations){

    unsigned long long cycles_tot, cycles1, cycles2;
    double start, end;
    uint16_t seed[SEED_BYTES / 2] = {0};
    double time_taken;

    /* AVX Fisher Yates Natural*/

    permAVX_t pu[n_permutations];

    start = (double)clock();
    cycles1 = cpucycles();

    for (int i = 0; i < n_permutations; ++i) {
        seed[0] = i;
        perm_set_random_natural_avx(&pu[i], (uint8_t *) seed);
    }

    cycles2 = cpucycles();
    end = (double)clock();
    cycles_tot = cycles2 - cycles1;
    time_taken = (end - start) / ((double) CLOCKS_PER_SEC);

    printf("\nVerifying permutations... ");
    for (int i = 0; i < n_permutations; ++i) {
        verify_permutation_avx(&pu[i]);
    }
    printf("Done\n");
    printf("AVX Natural Fisher Yates ................................... %10lld ", cycles_tot);
    printf("cycles");
    printf("\n");
    printf("Time taken %lf\n", time_taken);

}

int main() {

    printf("Start\n");

    bench_fisher_yates_sendrier_FY(N_PERMUTATIONS);
    bench_fisher_yates_non_ct(N_PERMUTATIONS);
    bench_fisher_yates_ternary_FY(N_PERMUTATIONS);
    bench_fisher_yates_natural_FY(N_PERMUTATIONS);
    bench_fisher_yates_sendrier_FY_AVX(N_PERMUTATIONS);
    bench_fisher_yates_natural_FY_AVX(N_PERMUTATIONS);
    bench_djbsort(N_PERMUTATIONS);
    bench_djbsort_AVX(N_PERMUTATIONS);

    printf("\nDone\n");
    
    return 0;
}


