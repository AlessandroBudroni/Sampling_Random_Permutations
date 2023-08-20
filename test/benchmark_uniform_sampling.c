/**
 * MIT License
 *
 * Copyright (c) 2023 [FULL NAME]
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "benchmark_utils.h"
#include "test_utils.h"

#include <xkcp/SimpleFIPS202.h>
#include <uniform_sampling/common_uniform_index_sample.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

void bench_sample_random_uniform() {

    unsigned long long cycles_tot, cycles1, cycles2;
    double start, end;
    uint8_t seed[SEED_BYTES + 1] = {0};

    printf("Start benchmark. Sample %d uniform indexes of length %d\n\n", N_ITERATIONS, PARAM_N);fflush(stdout);

    uniform_indexes_t indexes;
    cycles_tot = 0;
    double time_taken = 0;

    for (int i = 0; i < N_ITERATIONS; ++i) {
        SHAKE128(seed, sizeof(seed), (uint8_t *)&i, sizeof(i));

        start = (double) clock();
        cycles1 = cpucycles();

        common_sample_uniform_index_buffer(indexes, seed);

        cycles2 = cpucycles();
        end = (double)clock();
        time_taken += (end - start) / ((double) CLOCKS_PER_SEC);
        cycles_tot+= cycles2 - cycles1;
    }

    printf("Scheme %s time %10lld ", scheme_name(), cycles_tot);
    printf("cycles");
    printf("\n");
    printf("Time taken %lf\n\n", time_taken);
}

int main(){

//    printf("\nWarming up... ");fflush(stdout);
//    double time_taken = warm_up();
//    printf("Done\n");
//    printf("Time taken %lf\n\n", time_taken);

    bench_sample_random_uniform();

    return EXIT_SUCCESS;
}
