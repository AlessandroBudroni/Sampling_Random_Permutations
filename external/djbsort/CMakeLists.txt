
add_library(DJBSORT STATIC)
target_sources(DJBSORT PRIVATE djbsort_i32.c djbsort_i64.c djbsort_u32.c djbsort_u64.c)
target_compile_options(DJBSORT PRIVATE -fwrapv)

add_library(DJBSORT_MCELIECE STATIC)
target_sources(DJBSORT_MCELIECE PRIVATE djbsort_i32.c djbsort_i64.c djbsort_u32_mceliece.c djbsort_u64_mceliece.c)
target_compile_options(DJBSORT_MCELIECE PRIVATE -fwrapv)
target_compile_definitions(DJBSORT_MCELIECE PUBLIC DJBSORT_IS_MCELIECE)

add_library(DJBSORT_AVX2 STATIC)
target_sources(DJBSORT_AVX2 PRIVATE djbsort_i32_avx2.c djbsort_i64.c djbsort_u32.c djbsort_u64.c)
target_compile_options(DJBSORT_AVX2 PRIVATE -fwrapv -mavx2)
