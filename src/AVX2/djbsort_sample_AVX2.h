//
// Created by Alessandro Budroni on 28/10/2022.
//

#ifndef SECURE_FISHER_YATES_DJBSORT_SAMPLE_H
#define SECURE_FISHER_YATES_DJBSORT_SAMPLE_H

#include "../utils.h"
#include "../fisher_yates.h"

void perm_set_random_djbsort_avx2(perm_t p, uint8_t seed[16]);

#endif //SECURE_FISHER_YATES_DJBSORT_SAMPLE_H
