//
// Created by Alessandro Budroni on 07/09/2022.
//

#ifndef PLAYAVX2_UTILS_H
#define PLAYAVX2_UTILS_H

#include <stdint.h>

int64_t cpucycles(void);
double warm_up();

// Return 1 if the arguments are equal to each other. Return 0 otherwise.
static inline uint32_t secure_cmp32(const uint32_t v1, const uint32_t v2)
{
#if defined(__aarch64__)
uint32_t res;
  __asm__ __volatile__("cmp  %w[V1], %w[V2]; \n "
                       "cset %w[RES], EQ; \n"
                       : [RES] "=r"(res)
                       : [V1] "r"(v1), [V2] "r"(v2)
                       : "cc" /*The condition code flag*/);
  return res;
#elif defined(__x86_64__) || defined(__i386__)
uint32_t res;
__asm__ __volatile__("xor  %%edx, %%edx; \n"
                     "cmp  %1, %2; \n "
                     "sete %%dl; \n"
                     "mov %%edx, %0; \n"
: "=r"(res)
: "r"(v1), "r"(v2)
: "rdx");
return res;
#else
// Insecure comparison: The main purpose of secure_cmp32 is to avoid
      // branches to prevent potential side channel leaks. To do that,
      // we normally leverage some special CPU instructions such as "sete"
      // (for __x86_64__) and "cset" (for __aarch64__). When dealing with general
      // CPU architectures, the interpretation of the line below is left for the
      // compiler. It could lead to an "insecure" branch. This case needs to be
      // checked individually on such platforms
      // (e.g., by checking the compiler-generated assembly).
      return (v1 == v2 ? 1 : 0);
#endif
}

#endif //PLAYAVX2_UTILS_H
