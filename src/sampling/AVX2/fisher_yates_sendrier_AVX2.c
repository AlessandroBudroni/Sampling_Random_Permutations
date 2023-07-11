//
// Created by Alessandro Budroni on 07/09/2022.
//

#include "../../../fips202/fips202.h"
#include "fisher_yates_sendrier_AVX2.h"
#include "../../common.h"

#include <immintrin.h>
#include <string.h>
#include <stdio.h>


// or horizontally 16 int16_t packed in __m256i
static int16_t hor_256_16_avx2(__m256i v) {

    __m128i *pblck128 = (__m128i*)&v;
    pblck128[0] = _mm_or_si128(pblck128[0], pblck128[1]);

    int64_t *pblck64 = (int64_t*)pblck128;
    pblck64[0] |= pblck64[1];

    int32_t *pblck32 = (int32_t*)pblck64;
    pblck32[0] |= pblck32[1];

    int16_t *pblck16 = (int16_t*)pblck32;
    pblck16[0] |= (int16_t)pblck16[1];

    return pblck16[0] & (int16_t)0x1;
}

static void fisher_yates_shuffle_sendrier_avx2(permAVX_t *p) {

    for (int i = PARAM_N - 1; i >= 0; --i) {

        // non AVX2 part
        uint16_t mask = 0;
        uint16_t pi = (uint16_t)p->i[i];
        for (int j = i + 1; j < precomp_split[i]; ++j) {
            mask |= ISZERO((uint16_t)(p->i[j] - pi));
        }

        // AVX2 part
        __m256i avxmask = _mm256_setzero_si256();
        __m256i avxpi = _mm256_set1_epi16((int16_t)pi);
        for (int j = precomp_div[i]; j < AVX256_X_BLOCK; ++j) {
            avxmask = _mm256_or_si256(_mm256_cmpeq_epi16(p->avx[j], avxpi), avxmask);
        }
        mask |= hor_256_16_avx2(avxmask);
        p->i[i] = MASKAPPLY(mask-1, i, pi);
    }
}

static int set_random_with_bound_for_permutation_avx2(permAVX_t *p, const uint16_t rnd_buff[CHUNK_RND_U16_LENGTH]) {

    int size = PARAM_N;
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

static void sample_random_chunk_avx2( uint8_t rnd_buff[CHUNK_RND_BYTES_LENGTH], uint8_t expanded_seed[SEED_BYTES + 2]){
    shake128((uint8_t *)rnd_buff, CHUNK_RND_BYTES_LENGTH, expanded_seed, SEED_BYTES + 2);
}

void perm_set_random_sendrier_avx2(permAVX_t *p_out, uint8_t seed[SEED_BYTES]) {
    uint16_t rnd_buff[CHUNK_RND_U16_LENGTH];
    uint8_t expanded_seed[SEED_BYTES + 2];

    memcpy(expanded_seed, seed, SEED_BYTES);
    expanded_seed[SEED_BYTES] = DOMAIN_SEPARATOR_PERM;
    expanded_seed[SEED_BYTES + 1] = 0;

    sample_random_chunk((uint8_t *)rnd_buff, expanded_seed);

    while (set_random_with_bound_for_permutation_avx2(p_out, rnd_buff) != EXIT_SUCCESS) {
        expanded_seed[SEED_BYTES + 1] += 1;
        sample_random_chunk_avx2((uint8_t *)rnd_buff, expanded_seed);
    }
    fisher_yates_shuffle_sendrier_avx2(p_out);

    memset(rnd_buff, 0, sizeof(rnd_buff));
}

