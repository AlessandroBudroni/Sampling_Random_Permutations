//
// Created by Alessandro Budroni on 07/09/2022.
//

#include "../../../fips202/fips202.h"
#include "fisher_yates_sendrier_AVX2.h"
#include "../../common.h"

#include <immintrin.h>
#include <string.h>

static void fisher_yates_shuffle_sendrier_avx2(permAVX_t *p) {

    for (int i = PARAM_N - 1; i >= 0; --i) {

        // non AVX2 part
        uint16_t pi = p->i[i];
        for (int j = i + 1; j < precomp_split[i]; ++j) {
            uint16_t mask = ISZERO(p->i[j] - pi);
            p->i[j] = MASKAPPLY(mask-1, i, p->i[j]);
        }

        // AVX2 part
        __m256i avxpi = _mm256_set1_epi16((int16_t)pi);
        __m256i avxi = _mm256_set1_epi16((int16_t)i);
        for (int j = precomp_div[i]; j < AVX256_X_BLOCK; ++j) {
            __m256i avxmask = _mm256_cmpeq_epi16(p->avx[j], avxpi);
            p->avx[j] = _mm256_xor_si256(p->avx[j], _mm256_and_si256(avxmask, _mm256_xor_si256(p->avx[j], avxi)));
        }
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

