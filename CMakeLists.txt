cmake_minimum_required(VERSION 3.21)
project(random_permutation_sampling C)

# help macro
macro(log var)
    message(STATUS "${var}: ${${var}}")
endmacro()

# Other not used flags: -fsanitize=address -Wformat -Wformat-security -Wall -Wextra

execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion
        OUTPUT_VARIABLE GCC_VERSION)
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -funroll-loops")
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -funroll-loops -fsanitize=address -m64 -Wformat=0 -Werror -Wno-unused-function -Wno-unused-result -Wno-strict-prototypes -Wunused-value -Wunused-variable -Wundef")#-Wcast-align
else()
    message(FATAL_ERROR "CMAKE_BUILD_TYPE not valid")
endif()

log(CMAKE_C_COMPILER)
log(CMAKE_C_FLAGS)

set(CMAKE_C_STANDARD 11)

# Benchmark for non-AVX2 permutation sampling
add_executable(benchmark_sampling test/benchmark_sampling.c)
target_sources(benchmark_sampling PRIVATE fips202/fips202.c djbsort/djbsort.c src/sampling/fisher_yates.c src/sampling/fisher_yates_sendrier.c src/sampling/fisher_yates_natural.c src/common.c test/test_permutation_utils.c src/sampling/djbsort_sample.c src/sampling/djbsort_sample.c test/test_utils.c)

# Benchmark for AVX2 permutation sampling
add_executable(benchmark_sampling_AVX test/benchmark_sampling_avx2.c)
target_sources(benchmark_sampling_AVX PRIVATE fips202/fips202.c djbsort/djbsort.c djbsort/djbsort_avx.c test/test_utils.c src/common.c test/test_permutation_utils.c test/test_permutation_utils_AVX2.c src/sampling/fisher_yates.c src/sampling/AVX2/fisher_yates_sendrier_AVX2.c src/sampling/AVX2/fisher_yates_natural_AVX2.c src/sampling/AVX2/djbsort_sample_AVX2.c)
set_target_properties(benchmark_sampling_AVX PROPERTIES COMPILE_FLAGS "-mavx2" )

# Test sampling
add_executable(test_sampling test/test_sampling.c)
target_sources(test_sampling PRIVATE fips202/fips202.c djbsort/djbsort.c djbsort/djbsort_avx.c src/sampling/fisher_yates.c src/sampling/fisher_yates_sendrier.c src/sampling/fisher_yates_natural.c src/common.c test/test_permutation_utils.c src/sampling/djbsort_sample.c src/sampling/djbsort_sample.c test/test_utils.c src/sampling/AVX2/fisher_yates_sendrier_AVX2.c src/sampling/AVX2/fisher_yates_natural_AVX2.c src/sampling/AVX2/djbsort_sample_AVX2.c test/test_permutation_utils_AVX2.c)
set_target_properties(test_sampling PROPERTIES COMPILE_FLAGS "-mavx2" )


# Test inversion and composition
add_executable(test_invcomp test/test_inversion_composition.c)
target_sources(test_invcomp PRIVATE fips202/fips202.c djbsort/djbsort.c src/sampling/fisher_yates.c src/sampling/fisher_yates_sendrier.c src/sampling/fisher_yates_natural.c src/common.c test/test_permutation_utils.c src/sampling/djbsort_sample.c test/test_utils.c  src/composition/standard_composition.c src/inversion/inversion_substitution.c src/composition/djbsort_composition.c src/inversion/djbsort_inversion.c)

# Test compatibility
add_executable(test_compatibility test/test_compatibility.c)
target_sources(test_compatibility PRIVATE fips202/fips202.c djbsort/djbsort.c src/sampling/fisher_yates.c src/sampling/fisher_yates_sendrier.c src/sampling/fisher_yates_natural.c src/common.c test/test_permutation_utils.c src/sampling/djbsort_sample.c test/test_utils.c  src/composition/standard_composition.c src/inversion/inversion_substitution.c src/composition/djbsort_composition.c src/inversion/djbsort_inversion.c)
