//
// Created by Alessandro Budroni on 06/07/2023.
//

#include <stdlib.h>
#include <stdio.h>

#define ISZERO(n) (1 & ((n - 1) >> 16))

int main(){

    uint16_t a = 0;

    for (int i = 1; i < 8192; ++i) {
        if (ISZERO(i)){
            printf("Fail %d\n", i);
            break;
        }
    }
    printf("Result %d\n", ISZERO(a));

    return 0;
}