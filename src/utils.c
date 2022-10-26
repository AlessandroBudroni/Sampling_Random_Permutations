//
// Created by Alessandro Budroni on 07/09/2022.
//

#include <immintrin.h>
#include <stdio.h>
#include "utils.h"

int test_addition_modulo(){

    __m128i q =  _mm_set_epi16(30, 30, 30, 30, 30, 30, 30, 30);
    __m128i qm1 =  _mm_set_epi16(29, 29, 29, 29, 29, 29, 29, 29);
    __m128i a = _mm_set_epi16(10, 27,  8, 9, 26,  3, 15,  28);
    __m128i b = _mm_set_epi16(20, 22, 18, 9, 10, 13,  5, 14);
    __m128i dst = _mm_add_epi16(a, b);
    __m128i mask = _mm_cmpgt_epi16(dst, qm1);
    __m128i qmask = _mm_and_si128(q, mask);

    dst = _mm_sub_epi16(dst, qmask);

    /* Display the elements of the result vector */
    uint16_t *res = (uint16_t*)&dst;
    printf("%d %d %d %d %d %d %d %d\n",
           res[7], res[6], res[5], res[4], res[3], res[2], res[1], res[0]);

    return 0;
}
