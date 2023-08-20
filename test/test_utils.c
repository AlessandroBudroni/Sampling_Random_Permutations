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

#include "test_utils.h"
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>

void get_seed(uint8_t seed[SEED_BYTES])
{
    // get seed for RNG
    int randomData = open("/dev/urandom", O_RDONLY);
    if(randomData  < 0){
        printf("Could not open /dev/urandom\n");
        exit(EXIT_FAILURE);
    }
    int ret;
    ret = read(randomData, seed, SEED_BYTES);
    if(ret  < 0){
        printf("Could not read from /dev/urandom\n");
        exit(EXIT_FAILURE);
    }
    close(randomData);
}


// return EXIT_SUCCESS if the permutation is valid, EXIT_FAILURE otherwise
int validate_permutation(const perm_t p) {

    uint16_t verification[PARAM_N] = {0};

    //  check that the entries are all below PARAM_N
    for (int i = 0; i < PARAM_N; ++i) {
        if (p[i] > PARAM_N){
            printf("Verification failure with index %d and value %d\n", i, p[i]);
            return EXIT_FAILURE;
        }
        verification[p[i]]++;
    }

    // check that there are no repetitions
    for (int i = 0; i < PARAM_N; ++i) {
        if (verification[i] != 1) {
            printf("Verification failure at %d rep %d\n", i, p[i]);
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

// return EXIT_SUCCESS if the permutation is not trivial, EXIT_FAILURE otherwise
int test_non_triviality(const perm_t p){

    int sum = 1;
    for (int i = 0; i < PARAM_N; ++i) {
        sum &= (p[i] == i);
    }
    return (sum == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

// return EXIT_SUCCESS if the permutations are equal, EXIT_FAILURE otherwise
int test_equality(const perm_t p1, const perm_t p2) {

    for (int i = 0; i < PARAM_N; ++i) {
        if (p1[i] != p2[i]){
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}
