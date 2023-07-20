//
// Created by Alessandro Budroni on 08/07/2023.
//

#ifndef RANDOM_PERMUTATION_SAMPLING_COMPOSITION_SORTING_H
#define RANDOM_PERMUTATION_SAMPLING_COMPOSITION_SORTING_H

#include "../definitions.h"

void perm_compose_djbsort(perm_t out, const perm_t in1, const perm_t in2);
void perm_compose_with_inverse_djbsort(perm_t out, const perm_t in1, const perm_t in2);


#endif //RANDOM_PERMUTATION_SAMPLING_COMPOSITION_SORTING_H
