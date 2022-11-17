//
// Created by Alessandro Budroni on 07/09/2022.
//

#include "../include/sendrier_AVX2.h"
#include "../fips202/fips202.h"

#include <immintrin.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define ISNOTZERO(n) ((((n) | (~(n) + 1)) >> 31) & 0x1)

#define MASKAPPLY(mask, i, pj) (((mask) & (i)) | ((~(mask)) & (pj)))


static inline __m256i mask_avx2(const __m256i pi, const __m256i pj){ // unsigned is safer for bit operations

    __m256i mask;
    mask = _mm256_sub_epi16 (pi, pj);

    // ((mask | (~mask + 1))
    mask = _mm256_or_si256(mask, _mm256_add_epi16( _mm256_xor_si256(mask, _mm256_set1_epi16(-1LL)), _mm256_set1_epi16(1) ));
    // mask >> 15
    mask = _mm256_srai_epi16 (mask, 15);
    // mask & 1
    mask = _mm256_and_si256(mask, _mm256_set1_epi16(1));
    // mask -1
    mask = _mm256_sub_epi16 (mask, _mm256_set1_epi16(1));

    return mask;
}

static void fisher_yates_shuffle_avx_1(permAVX_t *p) {
    uint16_t mask;
    uint16_t pi, *pj;

    __m256i avxmask;
    __m256i avxpi, imask;

    for (int16_t i = PARAM_N1 - 1; i >= 0; --i) {

        // non AVX part
        pi = p->i[i];
        for (int16_t j = i + 1; j < precomp_split[i]; ++j) {
            pj = &p->i[j];
            mask = ISNOTZERO(*pj - pi) - 1;
            *pj = MASKAPPLY(mask, i, *pj); //(mask & i) | (~mask & *pj);
        }

        // AVX part
        imask = _mm256_set1_epi16((int16_t)i);
        avxpi = _mm256_set1_epi16((int16_t)pi);
        for (int16_t j = precomp_div[i]; j < AVX256_X_BLOCK; ++j) {
//            avxmask = mask_avx2(p->avx[j], avxpi);
            // alternative to the above
            avxmask = _mm256_cmpeq_epi16(p->avx[j], avxpi);
            p->avx[j] = _mm256_or_si256(_mm256_and_si256(avxmask, imask),  _mm256_and_si256(~avxmask, p->avx[j]));
        }
    }
}

static int16_t hxor_256_16_avx(__m256i v) {

    __m128i *pblck128 = (__m128i*)&v;
    pblck128[0] = _mm_xor_si128(pblck128[0], pblck128[1]);

    int64_t *pblck64 = (int64_t*)pblck128;
    pblck64[0] ^= pblck64[1];

    int32_t *pblck32 = (int32_t*)pblck64;
    pblck32[0] ^= pblck32[1];

    int16_t *pblck16 = (int16_t*)pblck32;
    pblck16[0] ^= (int16_t)pblck16[1];

    return pblck16[0];
}

static void fisher_yates_shuffle_avx_2(permAVX_t *p) {
    uint16_t mask;
    uint16_t *pi;

    __m256i avxmask;
    __m256i avxpi;

    for (int16_t i = PARAM_N1 - 1; i >= 0; --i) {

        // non AVX part
        pi = &p->i[i];
        mask = 0;
        for (int16_t j = i + 1; j < PARAM_N1; ++j) {  //p[j] = p[j] == p[i] ? i : p[j];
            mask ^= 1-ISNOTZERO(p->i[j] - *pi);
        }

        // AVX part
        avxpi = _mm256_set1_epi16((int16_t)*pi);
        for (int16_t j = precomp_div[i]; j < AVX256_X_BLOCK; ++j) {
            avxmask = _mm256_and_si256(avxmask,_mm256_cmpeq_epi16(p->avx[j], avxpi));
        }
        mask ^= hxor_256_16_avx(avxmask);
        *pi = MASKAPPLY(-mask, i, *pi);
    }
}


static int set_random_with_bound_for_permutation_avx(permAVX_t *p, const uint16_t rnd_buff[CHUNK_RND_U16_LENGTH]) {

    int size = PARAM_N1;
    uint16_t rnd;
    int32_t index = 0;
    uint32_t max;
    int bound;

    for (int i = 0; i < size; i++) {
        bound = size - i;
        max = ((0x10000 / bound ) * bound);// (2^16 / bound) * bound
        do {
            if (index >= CHUNK_RND_U16_LENGTH -1){
                return EXIT_FAILURE;
            }
            rnd = rnd_buff[index++];
        } while (rnd >= max);
        p->i[i] = i + (rnd % bound);
    }
    return EXIT_SUCCESS;
}

static void sample_random_chunk_avx( uint8_t rnd_buff[CHUNK_RND_BYTES_LENGTH], uint8_t expanded_seed[SEED_BYTES + 2]){
    shake128((uint8_t *)rnd_buff, CHUNK_RND_BYTES_LENGTH, expanded_seed, SEED_BYTES + 2);
}

void perm_set_random_sendrier_avx(permAVX_t *p, uint8_t seed[SEED_BYTES]) {
    uint16_t rnd_buff[CHUNK_RND_U16_LENGTH];
    uint8_t expanded_seed[SEED_BYTES + 2];

    memcpy(expanded_seed, seed, SEED_BYTES);
    expanded_seed[SEED_BYTES] = DOMAIN_SEPARATOR_PERM;
    expanded_seed[SEED_BYTES + 1] = 0;

    sample_random_chunk((uint8_t *)rnd_buff, expanded_seed);

    while (set_random_with_bound_for_permutation_avx(p, rnd_buff) != EXIT_SUCCESS) {
        expanded_seed[SEED_BYTES + 1] += 1;
        sample_random_chunk_avx((uint8_t *)rnd_buff, expanded_seed);
    }
    fisher_yates_shuffle_avx_2(p);

    memset(rnd_buff, 0, sizeof(rnd_buff));
}

int verify_permutation_avx(permAVX_t *p) {

    uint16_t verification[PARAM_N1] = {0};

    for (int i = 0; i < PARAM_N1; ++i) {
        if (p->i[i] > PARAM_N1){
            printf("Verification failure with index %d and value %d\n", i, p->i[i]);
            return EXIT_FAILURE;
        }
        verification[p->i[i]]++;
    }

    for (int i = 0; i < PARAM_N1; ++i) {
        if (verification[i] != 1) {
            printf("Verification failure at %d rep %d\n", i, p->i[i]);
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}
