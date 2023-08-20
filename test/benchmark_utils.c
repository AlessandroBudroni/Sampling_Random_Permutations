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
