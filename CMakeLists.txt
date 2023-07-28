cmake_minimum_required(VERSION 3.21)
project(random_permutation_sampling C CXX ASM)

enable_testing()

# help macro
macro(log var)
    message(STATUS "${var}: ${${var}}")
endmacro()

set(CC clang)
set(CXX clang++)

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

add_subdirectory(external/djbsort)
add_subdirectory(external/vqsort)
add_subdirectory(external/xkcp)

add_library(test_interface INTERFACE)
target_include_directories(test_interface INTERFACE src external)
target_link_libraries(test_interface INTERFACE XKCP)
target_sources(test_interface PUBLIC
        test/test_utils.c
        test/benchmark_utils.c)


add_library(test_interface_avx2 INTERFACE)
target_include_directories(test_interface_avx2 INTERFACE src external)
target_link_libraries(test_interface_avx2 INTERFACE XKCP_AVX2)
target_compile_options(test_interface_avx2 INTERFACE -mavx2)
target_sources(test_interface_avx2 PUBLIC
        test/test_utils.c
        test/benchmark_utils.c)

set(SRC_VQSORT
        src/sorting/vqsort_wrapper.c
        src/sampling/sort_sample.c
        src/inversion/sort_inversion.c
        src/composition/sort_composition.c)

set(SRC_DJBSORT
        src/sorting/djbsort_wrapper.c
        src/sampling/sort_sample.c
        src/inversion/sort_inversion.c
        src/composition/sort_composition.c)

set(SRC_FY
        src/common_fisher_yates.c
        src/sampling/fisher_yates.c
        src/inversion/inversion_substitution.c
        src/composition/standard_composition.c)

set(SRC_FYS
        src/common_fisher_yates.c
        src/sampling/fisher_yates_sendrier.c
        src/inversion/inversion_substitution_ct.c
        src/composition/standard_composition_ct.c)

set(SRC_FYS_AVX2
        src/common_fisher_yates.c
        src/sampling/AVX2/fisher_yates_sendrier_AVX2.c
        src/inversion/inversion_substitution_ct.c
        src/composition/standard_composition_ct.c)

set(SRC_FYN
        src/common_fisher_yates.c
        src/sampling/fisher_yates_natural.c
        src/inversion/inversion_substitution_ct.c
        src/composition/standard_composition_ct.c)

set(SRC_FYN_AVX2
        src/common_fisher_yates.c
        src/sampling/AVX2/fisher_yates_natural_AVX2.c
        src/inversion/inversion_substitution_ct.c
        src/composition/standard_composition_ct.c)

#vqsort
add_executable(test_api_vqsort test/test_api.c ${SRC_VQSORT})
target_link_libraries(test_api_vqsort PRIVATE test_interface_avx2 hwy_vqsort)
add_test(test_api_vqsort test_api_vqsort)
set_tests_properties(test_api_vqsort PROPERTIES LABEL test)
set_target_properties(test_api_vqsort PROPERTIES LINKER_LANGUAGE C)

add_executable(benchmark_api_vqsort test/benchmark_api.c ${SRC_VQSORT})
target_link_libraries(benchmark_api_vqsort PRIVATE test_interface_avx2 hwy_vqsort)
add_test(benchmark_api_vqsort benchmark_api_vqsort)
set_tests_properties(benchmark_api_vqsort PROPERTIES LABEL benchmark)
set_target_properties(benchmark_api_vqsort PROPERTIES LINKER_LANGUAGE C)

#djbsort
add_executable(test_api_djbsort test/test_api.c ${SRC_DJBSORT})
target_link_libraries(test_api_djbsort PRIVATE test_interface DJBSORT)
add_test(test_api_djbsort test_api_djbsort)
set_tests_properties(test_api_djbsort PROPERTIES LABEL test)

add_executable(benchmark_api_djbsort test/benchmark_api.c ${SRC_DJBSORT})
target_link_libraries(benchmark_api_djbsort PRIVATE test_interface DJBSORT)
add_test(benchmark_api_djbsort benchmark_api_djbsort)
set_tests_properties(benchmark_api_djbsort PROPERTIES LABEL benchmark)

#djbsort-AVX2
add_executable(test_api_djbsort_avx2 test/test_api.c ${SRC_DJBSORT})
target_link_libraries(test_api_djbsort_avx2 PRIVATE test_interface_avx2 DJBSORT_AVX2)
add_test(test_api_djbsort_avx2 test_api_djbsort_avx2)
set_tests_properties(test_api_djbsort_avx2 PROPERTIES LABEL test)

add_executable(benchmark_api_djbsort_avx2 test/benchmark_api.c ${SRC_DJBSORT})
target_link_libraries(benchmark_api_djbsort_avx2 PRIVATE test_interface_avx2 DJBSORT_AVX2)
add_test(benchmark_api_djbsort_avx2 benchmark_api_djbsort_avx2)
set_tests_properties(benchmark_api_djbsort_avx2 PROPERTIES LABEL benchmark)

# Fisher-Yates
add_executable(test_api_fisher_yates test/test_api.c ${SRC_FY})
target_link_libraries(test_api_fisher_yates PRIVATE test_interface)
add_test(test_api_fisher_yates test_api_fisher_yates)
set_tests_properties(test_api_fisher_yates PROPERTIES LABEL test)

add_executable(benchmark_api_fisher_yates test/benchmark_api.c ${SRC_FY})
target_link_libraries(benchmark_api_fisher_yates PRIVATE test_interface)
add_test(benchmark_api_fisher_yates benchmark_api_fisher_yates)
set_tests_properties(benchmark_api_fisher_yates PROPERTIES LABEL benchmark)

# Fisher-Yates-AVX2
add_executable(test_api_fisher_yates_avx2 test/test_api.c ${SRC_FY})
target_link_libraries(test_api_fisher_yates_avx2 PRIVATE test_interface_avx2)
add_test(test_api_fisher_yates_avx2 test_api_fisher_yates_avx2)
set_tests_properties(test_api_fisher_yates_avx2 PROPERTIES LABEL test)

add_executable(benchmark_api_fisher_yates_avx2 test/benchmark_api.c ${SRC_FY})
target_link_libraries(benchmark_api_fisher_yates_avx2 PRIVATE test_interface_avx2)
add_test(benchmark_api_fisher_yates_avx2 benchmark_api_fisher_yates_avx2)
set_tests_properties(benchmark_api_fisher_yates_avx2 PROPERTIES LABEL benchmark)

# Fisher-Yates-Sandrier
add_executable(test_api_fisher_yates_sandrier test/test_api.c ${SRC_FYS})
target_link_libraries(test_api_fisher_yates_sandrier PRIVATE test_interface)
add_test(test_api_fisher_yates_sandrier test_api_fisher_yates_sandrier)
set_tests_properties(test_api_fisher_yates_sandrier PROPERTIES LABEL test)

add_executable(benchmark_api_fisher_yates_sandrier test/benchmark_api.c ${SRC_FYS})
target_link_libraries(benchmark_api_fisher_yates_sandrier PRIVATE test_interface)
add_test(benchmark_api_fisher_yates_sandrier benchmark_api_fisher_yates_sandrier)
set_tests_properties(benchmark_api_fisher_yates_sandrier PROPERTIES LABEL benchmark)

# Fisher-Yates-Sandrier-AVX2
add_executable(test_api_fisher_yates_sandrier_avx2 test/test_api.c ${SRC_FYS_AVX2})
target_link_libraries(test_api_fisher_yates_sandrier_avx2 PRIVATE test_interface_avx2)
add_test(test_api_fisher_yates_sandrier_avx2 test_api_fisher_yates_sandrier_avx2)
set_tests_properties(test_api_fisher_yates_sandrier_avx2 PROPERTIES LABEL test)

add_executable(benchmark_api_fisher_yates_sandrier_avx2 test/benchmark_api.c ${SRC_FYS_AVX2})
target_link_libraries(benchmark_api_fisher_yates_sandrier_avx2 PRIVATE test_interface_avx2)
add_test(benchmark_api_fisher_yates_sandrier_avx2 benchmark_api_fisher_yates_sandrier_avx2)
set_tests_properties(benchmark_api_fisher_yates_sandrier_avx2 PROPERTIES LABEL benchmark)

# Fisher-Yates-natural
add_executable(test_api_fisher_yates_natural test/test_api.c ${SRC_FYN})
target_link_libraries(test_api_fisher_yates_natural PRIVATE test_interface)
add_test(test_api_fisher_yates_natural test_api_fisher_yates_natural)
set_tests_properties(test_api_fisher_yates_natural PROPERTIES LABEL test)

add_executable(benchmark_api_fisher_yates_natural test/benchmark_api.c ${SRC_FYN})
target_link_libraries(benchmark_api_fisher_yates_natural PRIVATE test_interface)
add_test(benchmark_api_fisher_yates_natural benchmark_api_fisher_yates_natural)
set_tests_properties(benchmark_api_fisher_yates_natural PROPERTIES LABEL benchmark)

# Fisher-Yates-natural-AVX2
add_executable(test_api_fisher_yates_natural_avx2 test/test_api.c ${SRC_FYN_AVX2})
target_link_libraries(test_api_fisher_yates_natural_avx2 PRIVATE test_interface_avx2)
add_test(test_api_fisher_yates_natural_avx2 test_api_fisher_yates_natural_avx2)
set_tests_properties(test_api_fisher_yates_natural_avx2 PROPERTIES LABEL test)

add_executable(benchmark_api_fisher_yates_natural_avx2 test/benchmark_api.c ${SRC_FYN_AVX2})
target_link_libraries(benchmark_api_fisher_yates_natural_avx2 PRIVATE test_interface_avx2)
add_test(benchmark_api_fisher_yates_natural_avx2 benchmark_api_fisher_yates_natural_avx2)
set_tests_properties(benchmark_api_fisher_yates_natural_avx2 PROPERTIES LABEL benchmark)

# vqsort
add_executable(test_vqsort test/test_vqsort.c)
target_link_libraries(test_vqsort hwy_vqsort)
add_test(test_vqsort test_vqsort)
set_target_properties(test_vqsort PROPERTIES LINKER_LANGUAGE C)
