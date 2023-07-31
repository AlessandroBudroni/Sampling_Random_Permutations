
#include "benchmark_utils.h"
#include "test_utils.h"
#include <api.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

void bench_sample_api() {

    unsigned long long cycles_tot, cycles1, cycles2;
    double start, end;
    uint8_t seed[SEED_BYTES] = {0};
    uint64_t *b_seed = (uint64_t*)seed;

    printf("Start benchmark. Sample %d permutations of length %d\n\n", N_PERMUTATIONS, PARAM_N);fflush(stdout);

    perm_t p;
    cycles_tot = 0;
    double time_taken = 0;

    for (int j = 0; j < N_ITERATIONS; ++j) {
        *b_seed = j;
        start = (double) clock();
        cycles1 = cpucycles();

        perm_set_random(p, seed);

        cycles2 = cpucycles();
        end = (double)clock();
        time_taken += (end - start) / ((double) CLOCKS_PER_SEC);
        cycles_tot+= cycles2 - cycles1;
        if(validate_permutation(p) != EXIT_SUCCESS){
            printf("ERROR: ending test\n");
            exit(0);
        }
    }

    printf("<SCHEME NAME> time ..............................%10lld ", cycles_tot);
    printf("cycles");
    printf("\n");
    printf("Time taken %lf\n\n", time_taken);
}

void bench_compose_api() {

    unsigned long long cycles_tot, cycles1, cycles2;
    double start, end;
    uint8_t seed[SEED_BYTES] = {0};
    uint64_t *b_seed = (uint64_t*)seed;

    printf("Start benchmark. Compose %d permutations of length %d\n\n", N_PERMUTATIONS, PARAM_N);fflush(stdout);

    perm_t p, prand;

    cycles_tot = 0;
    double time_taken = 0;

    *b_seed = -1;
    perm_set_random(p, seed);

    for (int j = 0; j < N_ITERATIONS; ++j) {

        *b_seed = j;
        perm_set_random(prand, seed);

        start = (double) clock();
        cycles1 = cpucycles();

        perm_compose(p, p, prand);

        cycles2 = cpucycles();
        end = (double)clock();


        time_taken += (end - start) / ((double) CLOCKS_PER_SEC);
        cycles_tot+= cycles2 - cycles1;
        if(validate_permutation(p) != EXIT_SUCCESS){
            printf("ERROR: ending test\n");
            exit(0);
        }
    }

    printf("<SCHEME NAME> time ..............................%10lld ", cycles_tot);
    printf("cycles");
    printf("\n");
    printf("Time taken %lf\n\n", time_taken);
}

void bench_inverse_api() {

    unsigned long long cycles_tot, cycles1, cycles2;
    double start, end;
    uint8_t seed[SEED_BYTES] = {0};
    uint64_t *b_seed = (uint64_t*)seed;

    printf("Start benchmark. Invert %d permutations of length %d\n\n", N_PERMUTATIONS, PARAM_N);fflush(stdout);

    perm_t p, pinv, id;

    cycles_tot = 0;
    double time_taken = 0;

    for (int j = 0; j < N_ITERATIONS; ++j) {

        *b_seed = j;
        perm_set_random(p, seed);

        start = (double) clock();
        cycles1 = cpucycles();

        perm_inverse(pinv, p);

        cycles2 = cpucycles();
        end = (double)clock();

        time_taken += (end - start) / ((double) CLOCKS_PER_SEC);
        cycles_tot+= cycles2 - cycles1;
        perm_compose(id, p, pinv);
        if(test_non_triviality(id) == EXIT_SUCCESS || validate_permutation(pinv)){
            printf("\nERROR: ending test\n");
            exit(0);
        }
    }

    printf("<SCHEME NAME> time ..............................%10lld ", cycles_tot);
    printf("cycles");
    printf("\n");
    printf("Time taken %lf\n\n", time_taken);
}


int main(){

    printf("\nWarming up... ");fflush(stdout);
    double time_taken = warm_up();
    printf("Done\n");
    printf("Time taken %lf\n\n", time_taken);

    bench_sample_api();
    bench_compose_api();
    bench_inverse_api();

    return EXIT_SUCCESS;
}