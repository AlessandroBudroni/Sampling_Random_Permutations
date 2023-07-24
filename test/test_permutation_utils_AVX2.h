//
// Created by Alessandro Budroni on 08/07/2023.
//

#ifndef RANDOM_PERMUTATION_SAMPLING_TEST_PERMUTATION_UTILS_AVX2_H
#define RANDOM_PERMUTATION_SAMPLING_TEST_PERMUTATION_UTILS_AVX2_H

int validate_permutation_avx2(permAVX_t *p);
int test_non_triviality_avx2(permAVX_t p);
int test_equality_avx2(const perm_t p1, permAVX_t p2);

#endif //RANDOM_PERMUTATION_SAMPLING_TEST_PERMUTATION_UTILS_AVX2_H
