
#include <stdio.h>

#include "benchmark_utils.h"
#include "test_utils.h"
#include <api.h>
#include <time.h>

int main() {

    unsigned long long cycles_tot, cycles1, cycles2;
    double start, end;
    uint16_t seed[SEED_BYTES / 2] = {0};


    printf("\nWarming up... ");fflush(stdout);
    double time_taken = warm_up();
    printf("Done\n");
    printf("Time taken %lf\n\n", time_taken);


    printf("Start benchmark. Sample %d permutations of length %d, repeat %d times\n\n", N_PERMUTATIONS, PARAM_N, N_ITERATIONS);fflush(stdout);


    // Fisher Yates non-constant time

    perm_t p[N_PERMUTATIONS];

    cycles_tot = 0;
    time_taken = 0;

    for (int j = 0; j <N_ITERATIONS; ++j) {
        start = (double) clock();
        cycles1 = cpucycles();

        for (int i = 0; i < N_PERMUTATIONS; ++i) {
            seed[0] = i;
            perm_set_random(p[i], (uint8_t *) seed);
        }

        cycles2 = cpucycles();
        end = (double)clock();
        time_taken += (end - start) / ((double) CLOCKS_PER_SEC);
        cycles_tot+= cycles2 - cycles1;
        for (int i = 0; i < N_PERMUTATIONS; ++i) {
            validate_permutation(p[i]);
        }
    }

    printf("<SCHEME NAME> time ..............................%10lld ", cycles_tot);
    printf("cycles");
    printf("\n");
    printf("Time taken %lf\n\n", time_taken);
}


