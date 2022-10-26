//
// Created by Alessandro Budroni on 10/10/2022.
//

#include "fisher_yates.h"
#include "fisher_yates_natural_AVX2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/**
 * Sample random data for Fisher Yates permutation
 * @param rsource prng struct
 * @param p permutation array
 * @param size size of permutation array.
 * @return EXIT_FAILURE in case of rejection (sampled data not sufficient), EXIT_SUCCESS in case of success
 */
int set_random_with_bound_for_permutation_natural_avx(permAVX_t p, const uint16_t rnd_buff[CHUNK_RND_U16_LENGTH]) {

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
        p.i[i] = (rnd % bound);
    }
    return EXIT_SUCCESS;
}

#define GTEQ16(b1,b2) ((uint16_t)((uint16_t)(b2) -(uint16_t)(b1) -(uint16_t)1) >> 15 & (uint8_t)0x1)

// sum horizontally 16 int16_t packed in __m256i
inline int16_t hsum_256_16_avx(__m256i v) {

    __m128i *pblck128 = (__m128i*)&v;
    pblck128[0] = _mm_add_epi16(pblck128[0], pblck128[1]);

    int64_t *pblck64 = (int64_t*)pblck128;
    pblck64[0] += pblck64[1];

    int32_t *pblck32 = (int32_t*)pblck64;
    pblck32[0] += pblck32[1];

    int16_t *pblck16 = (int16_t*)pblck32;
    pblck16[0] += (int16_t)pblck16[1];

    return pblck16[0];
}

static inline __m256i mask_avx2_natural(const __m256i pi, const __m256i pj, const __m256i avxone){ // unsigned is safer for bit operations
    __m256i mask;
    // mask = pj - pi - 1 (the -1 is done before the loop)
    mask = _mm256_sub_epi16(pj, pi);
    // mask >> 15
    mask = _mm256_srai_epi16(mask, 15);
    // mask & 1
    mask = _mm256_and_si256(mask, avxone);

    return mask;
}

inline void fisher_yates_shuffle_natural_avx(permAVX_t *p_out, permAVX_t p_rand) {

    uint16_t div, rem_en, rem_st, j;
    uint16_t *pi, tmp;
    uint8_t mask;
    __m256i avxpi, avxmask, avxone, avxsum;
    avxone = _mm256_set1_epi16(1);

    for (uint16_t i = 0; i < PARAM_N1; i++) {
        pi = &p_out->i[i];
        *pi = p_rand.i[i];

        div = div_nat[i];
        rem_st = rem_str[i];
        rem_en = rem_end[i];

        tmp = 0;
        // pi+1
        avxpi = _mm256_add_epi16(_mm256_set1_epi16((int16_t)*pi), avxone);
        avxsum = _mm256_setzero_si256();
        for (j = 0; j < div; j++) {
            avxmask = _mm256_and_si256(avxone,_mm256_cmpgt_epi16(avxpi, p_rand.avx[j]));
            // alternatives to the above
            // avxmask =_mm256_and_si256(_mm256_srai_epi16(_mm256_sub_epi16(p_rand.avx[j], avxpi), 15), avxone);
            // avxmask = mask_avx2_natural(avxpi, p_rand.avx[j], avxone);
            avxsum = _mm256_add_epi16(avxsum, avxmask);
            p_rand.avx[j] = _mm256_sub_epi16(p_rand.avx[j], _mm256_xor_si256(avxmask, avxone));
        }
        tmp += hsum_256_16_avx(avxsum);

        for (j = rem_st; j < rem_en; j++) {
            mask = GTEQ16(*pi, p_rand.i[j]);
            tmp += mask;
            p_rand.i[j] -= mask ^ (uint8_t)0x1;
        }
        *pi += tmp;
    }
}

/**
 * Apply fisher yates to sample permutation. This function should be called after
 * set_random_with_bound_for_permutation()
 * @param p permutation with random data already filled in
 * @param size of permutation
 * @return EXIT_SUCCESS for success, EXIT_FAILURE for failure in sampling
 */
void perm_set_random_natural_avx(permAVX_t *p_out, uint8_t seed[SEED_BYTES]) {
    uint16_t rnd_buff[CHUNK_RND_U16_LENGTH];
    uint8_t expanded_seed[SEED_BYTES + 2];
    permAVX_t p_rand = {0};
    memcpy(expanded_seed, seed, SEED_BYTES);
    expanded_seed[SEED_BYTES] = DOMAIN_SEPARATOR_PERM;
    expanded_seed[SEED_BYTES + 1] = 0;
    sample_random_chunk((uint8_t *)rnd_buff, expanded_seed);

    while (set_random_with_bound_for_permutation_natural_avx(p_rand, rnd_buff) != EXIT_SUCCESS) {
        expanded_seed[SEED_BYTES + 1] += 1;
        sample_random_chunk((uint8_t *)rnd_buff, expanded_seed);
    }
    fisher_yates_shuffle_natural_avx(p_out, p_rand);
}
