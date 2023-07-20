//
// Created by Alessandro Budroni on 28/10/2022.
//

#ifndef SECURE_FISHER_YATES_DJBSORT_SAMPLE_H
#define SECURE_FISHER_YATES_DJBSORT_SAMPLE_H

#include "../../test/test_utils.h"
#include "fisher_yates.h"

void perm_set_random_djbsort(perm_t p, uint8_t seed[SEED_BYTES]);

#endif //SECURE_FISHER_YATES_DJBSORT_SAMPLE_H
