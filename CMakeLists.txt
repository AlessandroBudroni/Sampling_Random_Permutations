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
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -funroll-loops")
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -funroll-loops -fsanitize=address -m64 -Wformat=0 -Werror -Wno-unused-function -Wno-unused-result -Wno-strict-prototypes -Wunused-value -Wunused-variable -Wundef")#-Wcast-align
else()
    message(FATAL_ERROR "CMAKE_BUILD_TYPE not valid")
endif()

log(CMAKE_C_COMPILER)
log(CMAKE_C_FLAGS)

set(CMAKE_C_STANDARD 11)

add_executable(Benchmark benchmark.c)
target_sources(Benchmark PRIVATE fips202/fips202.c)
target_sources(Benchmark PRIVATE src/fisher_yates.c)
target_sources(Benchmark PRIVATE src/fisher_yates_bike.c)
target_sources(Benchmark PRIVATE src/fisher_yates_sendrier.c)
target_sources(Benchmark PRIVATE src/fisher_yates_natural.c)
target_sources(Benchmark PRIVATE djbsort/djbsort.c)
target_sources(Benchmark PRIVATE src/djbsort_sample.c)
target_sources(Benchmark PRIVATE src/utils.c)

add_executable(Benchmark_AVX benchmark_avx.c)
target_sources(Benchmark_AVX PRIVATE fips202/fips202.c)
target_sources(Benchmark_AVX PRIVATE src/fisher_yates.c)
target_sources(Benchmark_AVX PRIVATE src/sendrier_AVX2.c)
target_sources(Benchmark_AVX PRIVATE src/fisher_yates_natural_AVX2.c)
target_sources(Benchmark_AVX PRIVATE djbsort/djbsort.c)
target_sources(Benchmark_AVX PRIVATE djbsort/djbsort_avx.c)
target_sources(Benchmark_AVX PRIVATE src/djbsort_sample_avx.c)
target_sources(Benchmark_AVX PRIVATE src/utils.c)
set_target_properties(Benchmark_AVX PROPERTIES COMPILE_FLAGS "-mavx2" )
