//
// Created by perin on 8/2/23.
//

#ifndef RANDOM_PERMUTATION_SAMPLING_MACROS_H
#define RANDOM_PERMUTATION_SAMPLING_MACROS_H

#include <stdint.h>

// constant time "greater than"
#define GT16(b1,b2) ((uint16_t)((uint16_t)(b2) -(uint16_t)(b1)) >> 15 & (uint8_t)0x1)

// Return 1 if n = 0, 0 otherwise - for uint16_t
#define ISZERO(n) ((n == 0) ? 1 : 0) //(1 & ((n - 1) >> 16))

// Return 1 if n ≠ 0, 0 otherwise - for uint16_t
//#define ISNOTZERO(n) ((((n) | (~(n) + 1)) >> 15) & (uint8_t)0x1)

// Return either a or b according to the mask
#define MASKAPPLY(mask, a, b) ((a) ^ (mask & (a ^ b))) //(((mask) & (a)) | ((~(mask)) & (b)))

#endif //RANDOM_PERMUTATION_SAMPLING_MACROS_H
