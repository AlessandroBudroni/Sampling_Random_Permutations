
add_library(DJBSORT STATIC)
target_sources(DJBSORT PRIVATE djbsort.c)
target_compile_options(DJBSORT PRIVATE -fwrapv)

add_library(DJBSORT_AVX2 STATIC)
target_sources(DJBSORT_AVX2 PRIVATE djbsort_avx.c)
target_compile_options(DJBSORT_AVX2 PRIVATE -fwrapv -mavx2)
