//
// Created by Alessandro Budroni on 07/09/2022.
//

#ifndef PLAYAVX2_FISHER_YATES_H
#define PLAYAVX2_FISHER_YATES_H

#include <stdint.h>
#include "../../test/test_utils.h"
#include "../definitions.h"
#include "../common.h"

void perm_set_random_natural(perm_t p_out, uint8_t seed[SEED_BYTES]);
void perm_set_random_fisher_yates_non_ct(perm_t p, uint8_t seed[SEED_BYTES]);
void perm_set_random_bike(perm_t p, uint8_t seed[SEED_BYTES]);

#endif //PLAYAVX2_FISHER_YATES_H
