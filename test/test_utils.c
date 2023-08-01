//
// Created by Alessandro Budroni on 06/07/2023.
//

#include "test_utils.h"
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>

int get_seed()
{
    // get seed for RNG
    int randomData = open("/dev/urandom", O_RDONLY);
    if(randomData  < 0){
        printf("Could not open /dev/urandom\n");
        return EXIT_FAILURE;
    }
    int seed, ret;
    ret = read(randomData, &seed, sizeof(int));
    if(ret  < 0){
        printf("Could not read from /dev/urandom\n");
        return EXIT_FAILURE;
    }
    close(randomData);
    return seed;
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
