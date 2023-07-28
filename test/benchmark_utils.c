//
// Created by Alessandro Budroni on 07/09/2022.
//

#include "benchmark_utils.h"
#include "../src/definitions.h"
#include <time.h>

/* warm up cpu to avoid cpu throttling */
double warm_up(){

    double time_taken, start, end;

    start = (double) clock();
    volatile int k = 2, m = 3;
    for (int i = 0; i < 10000; ++i) {
        k *= k;
        k = k % 0X800;
        k = k+1;
        for (int j = 0; j < 10000; ++j) {
            m *= k;
            m = m % 0X400;
            m += 1;
        }
    }
    end = (double) clock();

    if(k == m && m == 0){
        return 0;
    }
    time_taken = (end - start) / ((double) CLOCKS_PER_SEC);
    return time_taken;
}

/* Access system counter for benchmarking */
int64_t cpucycles(void)
{
    unsigned int hi, lo;
    __asm__ volatile ("rdtsc\n\t" : "=a" (lo), "=d"(hi));
    return ((int64_t)lo) | (((int64_t)hi) << 32);
}
