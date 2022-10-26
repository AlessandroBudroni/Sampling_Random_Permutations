
#include <stdio.h>

#include "src/fisher_yates.h"
#include "src/fisher_yates_AVX2.h"
#include "src/fisher_yates_natural.h"
#include "src/fisher_yates_natural_AVX2.h"
#include <time.h>

/* Access system counter for benchmarking */
int64_t cpucycles(void)
{
    unsigned int hi, lo;
    __asm__ volatile ("rdtsc\n\t" : "=a" (lo), "=d"(hi));
    return ((int64_t)lo) | (((int64_t)hi) << 32);
}

#define N_PERMUTATION 1000

int main() {

    printf("Start\n\n");

    unsigned long long cycles_tot, cycles1, cycles2;
    double start, end;
    uint16_t seed[SEED_BYTES / 2] = {0};
    double time_taken;

    /* NORMAL Fisher Yates */

    perm_t p[N_PERMUTATION];

    cycles1 = cpucycles();
    start = (double)clock();

    for (int i = 0; i < N_PERMUTATION; ++i) {
        perm_set_random(p[i], (uint8_t *) seed);
        seed[0] = i % 256;
    }

    end = (double)clock();
    cycles2 = cpucycles();
    cycles_tot = cycles2 - cycles1;
    time_taken = (end - start) / ((double) CLOCKS_PER_SEC);

    printf("Verifying permutations... ");
    for (int i = 0; i < N_PERMUTATION; ++i) {
        verify_permutation(p[i]);
    }
    printf("Done\n");
    printf("Original Fisher Yates ...................................... %10lld ", cycles_tot);
    printf("cycles");
    printf("\n");
    printf("Time taken %lf\n", time_taken);

    /* NATURAL Fisher Yates */

    cycles1 = cpucycles();
    start = (double)clock();

    for (int i = 0; i < N_PERMUTATION; ++i) {
        perm_set_random_natural(p[i], (uint8_t *) seed);
        seed[0] = i % 256;
    }

    end = (double)clock();
    cycles2 = cpucycles();
    cycles_tot = cycles2 - cycles1;
    time_taken = (end - start) / ((double) CLOCKS_PER_SEC);

    printf("Verifying permutations... ");
    for (int i = 0; i < N_PERMUTATION; ++i) {
        verify_permutation(p[i]);
    }
    printf("Done\n");
    printf("Natural Fisher Yates ....................................... %10lld ", cycles_tot);
    printf("cycles");
    printf("\n");
    printf("Time taken %lf\n", time_taken);

    /* AVX Fisher Yates */

    permAVX_t pu[N_PERMUTATION];

    cycles1 = cpucycles();
    start = (double)clock();

    for (int i = 0; i < N_PERMUTATION; ++i) {
        perm_set_random_avx(&pu[i], (uint8_t *) seed);
        seed[0] = i % 256;
    }

    end = (double)clock();
    cycles2 = cpucycles();
    cycles_tot = cycles2 - cycles1;
    time_taken = (end - start) / ((double) CLOCKS_PER_SEC);

    printf("Verifying permutations... ");
    for (int i = 0; i < N_PERMUTATION; ++i) {
        verify_permutation_avx(&pu[i]);
    }
    printf("Done\n");
    printf("AVX Original Fisher Yates .................................. %10lld ", cycles_tot);
    printf("cycles");
    printf("\n");
    printf("Time taken %lf\n", time_taken);

    /* AVX Fisher Yates Natural*/

    cycles1 = cpucycles();
    start = (double)clock();

    for (int i = 0; i < N_PERMUTATION; ++i) {
        perm_set_random_natural_avx(&pu[i], (uint8_t *) seed);
        seed[0] = i % 256;
    }

    end = (double)clock();
    cycles2 = cpucycles();
    cycles_tot = cycles2 - cycles1;
    time_taken = (end - start) / ((double) CLOCKS_PER_SEC);

    printf("Verifying permutations... ");
    for (int i = 0; i < N_PERMUTATION; ++i) {
        verify_permutation_avx(&pu[i]);
    }
    printf("Done\n");
    printf("AVX Natural Fisher Yates ................................... %10lld ", cycles_tot);
    printf("cycles");
    printf("\n");
    printf("Time taken %lf\n", time_taken);



    return 0;
}


