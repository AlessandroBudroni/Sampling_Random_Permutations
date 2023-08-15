add_library(test_interface INTERFACE)
target_include_directories(test_interface INTERFACE ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/external)
target_link_libraries(test_interface INTERFACE XKCP)
target_sources(test_interface PUBLIC
        test_utils.c
        benchmark_utils.c)


add_library(test_interface_avx2 INTERFACE)
target_include_directories(test_interface_avx2 INTERFACE ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/external)
target_link_libraries(test_interface_avx2 INTERFACE XKCP_AVX2)
target_compile_options(test_interface_avx2 INTERFACE -mavx2)
target_sources(test_interface_avx2 PUBLIC
        test_utils.c
        benchmark_utils.c)

set(SRC_VQSORT_32
        ${CMAKE_SOURCE_DIR}/src/sorting/vqsort_wrapper.c
        ${CMAKE_SOURCE_DIR}/src/permutation_sampling/sort_sample.c
        ${CMAKE_SOURCE_DIR}/src/inversion/sort_inversion.c
        ${CMAKE_SOURCE_DIR}/src/composition/sort_composition.c)

set(SRC_VQSORT_64
        ${CMAKE_SOURCE_DIR}/src/sorting/vqsort_wrapper.c
        ${CMAKE_SOURCE_DIR}/src/permutation_sampling/sort_sample_64.c
        ${CMAKE_SOURCE_DIR}/src/inversion/sort_inversion.c
        ${CMAKE_SOURCE_DIR}/src/composition/sort_composition.c)

set(SRC_DJBSORT_32
        ${CMAKE_SOURCE_DIR}/src/sorting/djbsort_wrapper.c
        ${CMAKE_SOURCE_DIR}/src/permutation_sampling/sort_sample.c
        ${CMAKE_SOURCE_DIR}/src/inversion/sort_inversion.c
        ${CMAKE_SOURCE_DIR}/src/composition/sort_composition.c)

set(SRC_DJBSORT_64
        ${CMAKE_SOURCE_DIR}/src/sorting/djbsort_wrapper.c
        ${CMAKE_SOURCE_DIR}/src/permutation_sampling/sort_sample_64.c
        ${CMAKE_SOURCE_DIR}/src/inversion/sort_inversion.c
        ${CMAKE_SOURCE_DIR}/src/composition/sort_composition.c)

set(SRC_FY
        ${CMAKE_SOURCE_DIR}/src/permutation_sampling/shuffle_sample.c
        ${CMAKE_SOURCE_DIR}/src/shuffling/fisher_yates_shuffle.c
        ${CMAKE_SOURCE_DIR}/src/uniform_sampling/buffered_lemire_uniform_index_sample.c
        ${CMAKE_SOURCE_DIR}/src/inversion/inversion_substitution.c
        ${CMAKE_SOURCE_DIR}/src/composition/standard_composition.c)

set(SRC_FYS_CT
        ${CMAKE_SOURCE_DIR}/src/permutation_sampling/shuffle_sample.c
        ${CMAKE_SOURCE_DIR}/src/shuffling/sendrier_shuffle.c
        ${CMAKE_SOURCE_DIR}/src/uniform_sampling/buffered_uniform_index_sample_ct.c
        ${CMAKE_SOURCE_DIR}/src/inversion/inversion_substitution_ct.c
        ${CMAKE_SOURCE_DIR}/src/composition/standard_composition_ct.c)

set(SRC_FYS_AVX2_CT
        ${CMAKE_SOURCE_DIR}/src/permutation_sampling/shuffle_sample.c
        ${CMAKE_SOURCE_DIR}/src/shuffling/sendrier_shuffle_avx2.c
        ${CMAKE_SOURCE_DIR}/src/uniform_sampling/buffered_uniform_index_sample_ct.c
        ${CMAKE_SOURCE_DIR}/src/inversion/inversion_substitution_ct.c
        ${CMAKE_SOURCE_DIR}/src/composition/standard_composition_ct.c)

set(SRC_FYN_CT
        ${CMAKE_SOURCE_DIR}/src/permutation_sampling/shuffle_sample.c
        ${CMAKE_SOURCE_DIR}/src/shuffling/natural_shuffle.c
        ${CMAKE_SOURCE_DIR}/src/uniform_sampling/natural_buffered_uniform_index_sample_ct.c
        ${CMAKE_SOURCE_DIR}/src/inversion/inversion_substitution_ct.c
        ${CMAKE_SOURCE_DIR}/src/composition/standard_composition_ct.c)

set(SRC_FYN_AVX2_CT
        ${CMAKE_SOURCE_DIR}/src/permutation_sampling/shuffle_sample.c
        ${CMAKE_SOURCE_DIR}/src/shuffling/natural_shuffle_avx2.c
        ${CMAKE_SOURCE_DIR}/src/uniform_sampling/natural_buffered_uniform_index_sample_ct.c
        ${CMAKE_SOURCE_DIR}/src/inversion/inversion_substitution_ct.c
        ${CMAKE_SOURCE_DIR}/src/composition/standard_composition_ct.c)

set(DUDECT_TIMEOUT 5)

macro(register_test test_name test_sources test_libraries TIMEOUT_FAIL)
    message(">> register  ${test_name}")
    message(".. sources   ${${test_sources}}")
    message(".. libraries ${${test_libraries}}")
    message(".. Expect Fail ${TIMEOUT_FAIL}")
    add_executable(test_${test_name} test_api.c ${${test_sources}})
    target_link_libraries(test_${test_name} PRIVATE ${${test_libraries}})
    target_compile_definitions(test_${test_name} PRIVATE SCHEME_NAME="${test_name}")
    set_target_properties(test_${test_name} PROPERTIES LINKER_LANGUAGE C)
    add_test(test_${test_name} test_${test_name})
    set_tests_properties(test_${test_name} PROPERTIES LABEL test)

    add_executable(kat_${test_name} kat_api.c ${${test_sources}})
    target_link_libraries(kat_${test_name} PRIVATE ${${test_libraries}})
    target_compile_definitions(kat_${test_name} PRIVATE SCHEME_NAME="${test_name}")
    set_target_properties(kat_${test_name} PROPERTIES LINKER_LANGUAGE C)
    add_test(kat_${test_name} kat_${test_name})
    set_tests_properties(kat_${test_name} PROPERTIES LABEL test)

    add_executable(benchmark_${test_name} benchmark_api.c ${${test_sources}})
    target_link_libraries(benchmark_${test_name} PRIVATE ${${test_libraries}})
    target_compile_definitions(benchmark_${test_name} PRIVATE SCHEME_NAME="${test_name}")
    set_target_properties(benchmark_${test_name} PROPERTIES LINKER_LANGUAGE C)
    add_test(benchmark_${test_name} benchmark_${test_name})
    set_tests_properties(benchmark_${test_name} PROPERTIES LABEL benchmark)

    add_executable(dudect_${test_name} dudect_api.c ${${test_sources}})
    target_link_libraries(dudect_${test_name} PRIVATE ${${test_libraries}} m)
    target_compile_definitions(dudect_${test_name} PRIVATE SORT_SAMPLING_ABORT_DISABLED)
    set_target_properties(dudect_${test_name} PROPERTIES LINKER_LANGUAGE C)
    add_test(dudect_${test_name} dudect_${test_name})
    set_tests_properties(dudect_${test_name} PROPERTIES LABEL dudect WILL_FAIL ${TIMEOUT_FAIL})
endmacro()

#vqsort
set(hwy_libraries test_interface vqsort)

if (NOT PARAM_N EQUAL 4096)
    register_test(api_vqsort_32 SRC_VQSORT_32 hwy_libraries True)
endif ()
register_test(api_vqsort_64 SRC_VQSORT_64 hwy_libraries True)

set(hwy_libraries_avx2 test_interface_avx2 vqsort_avx2)
if (NOT PARAM_N EQUAL 4096)
    register_test(api_vqsort_32_avx2 SRC_VQSORT_32 hwy_libraries_avx2 True)
endif ()
register_test(api_vqsort_64_avx2 SRC_VQSORT_64 hwy_libraries_avx2 True)

#djbsort
set(djb_libraries test_interface DJBSORT)
if (NOT PARAM_N EQUAL 4096)
    register_test(api_djbsort_32 SRC_DJBSORT_32 djb_libraries False)
endif ()
register_test(api_djbsort_64 SRC_DJBSORT_64 djb_libraries False)
#
set(djb_libraries_avx2 test_interface_avx2 DJBSORT_AVX2)
if (NOT PARAM_N EQUAL 4096)
    register_test(api_djbsort_32_avx2 SRC_DJBSORT_32 djb_libraries_avx2 False)
endif ()
register_test(api_djbsort_64_avx2 SRC_DJBSORT_64 djb_libraries_avx2 False)
#
set(djb_mceliece_libraries test_interface DJBSORT_MCELIECE)
if (NOT PARAM_N EQUAL 4096)
    register_test(api_djbsort_32_mceliece SRC_DJBSORT_32 djb_mceliece_libraries False)
endif ()
register_test(api_djbsort_64_mceliece SRC_DJBSORT_64 djb_mceliece_libraries False)
#
set(djb_mceliece_libraries_avx2 test_interface_avx2 DJBSORT_MCELIECE)
if (NOT PARAM_N EQUAL 4096)
    register_test(api_djbsort_32_mceliece_avx2 SRC_DJBSORT_32 djb_mceliece_libraries_avx2 False)
endif ()
register_test(api_djbsort_64_mceliece_avx2 SRC_DJBSORT_64 djb_mceliece_libraries_avx2 False)

#fisher-yates
set(fy_libraries test_interface)
register_test(api_fisher_yates SRC_FY fy_libraries True)
register_test(api_fisher_yates_sendrier_ct SRC_FYS_CT fy_libraries False)
register_test(api_fisher_yates_natural_ct SRC_FYN_CT fy_libraries False)

#
set(fy_libraries_avx2 test_interface_avx2)
register_test(api_fisher_yates_avx2 SRC_FY fy_libraries_avx2 True)
register_test(api_fisher_yates_sendrier_avx2_ct SRC_FYS_AVX2_CT fy_libraries_avx2 False)
register_test(api_fisher_yates_natural_avx2_ct SRC_FYN_AVX2_CT fy_libraries_avx2 False)

# Test sorting

if (NOT PARAM_N EQUAL 4096)
    add_executable(test_sort_u32 test_sort_u32.c)
    target_link_libraries(test_sort_u32 PRIVATE test_interface_avx2 DJBSORT vqsort_avx2)
    target_compile_definitions(test_sort_u32 PRIVATE SCHEME_NAME="sorting_u32")
    set_target_properties(test_sort_u32 PROPERTIES LINKER_LANGUAGE C)
    add_test(test_sort_u32 test_sort_u32)
    set_tests_properties(test_sort_u32 PROPERTIES LABEL test)

    add_executable(test_sort_u32_mceliece test_sort_u32.c)
    target_link_libraries(test_sort_u32_mceliece PRIVATE test_interface_avx2 DJBSORT_MCELIECE vqsort_avx2)
    target_compile_definitions(test_sort_u32_mceliece PRIVATE SCHEME_NAME="sorting_u32_mceliece")
    set_target_properties(test_sort_u32_mceliece PROPERTIES LINKER_LANGUAGE C)
    add_test(test_sort_u32_mceliece test_sort_u32_mceliece)
    set_tests_properties(test_sort_u32_mceliece PROPERTIES LABEL test)
endif ()

add_executable(test_sort_u64 test_sort_u64.c)
target_link_libraries(test_sort_u64 PRIVATE test_interface_avx2 DJBSORT vqsort_avx2)
target_compile_definitions(test_sort_u64 PRIVATE SCHEME_NAME="sorting_u64")
set_target_properties(test_sort_u64 PROPERTIES LINKER_LANGUAGE C)
add_test(test_sort_u64 test_sort_u64)
set_tests_properties(test_sort_u64 PROPERTIES LABEL test)

add_executable(test_sort_u64_mceliece test_sort_u64.c)
target_link_libraries(test_sort_u64_mceliece PRIVATE test_interface_avx2 DJBSORT_MCELIECE vqsort_avx2)
target_compile_definitions(test_sort_u64_mceliece PRIVATE SCHEME_NAME="sorting_u64_mceliece")
set_target_properties(test_sort_u64_mceliece PROPERTIES LINKER_LANGUAGE C)
add_test(test_sort_u64_mceliece test_sort_u64_mceliece)
set_tests_properties(test_sort_u64_mceliece PROPERTIES LABEL test)


# test uniform sampling

add_executable(benchmark_uniform_sampling_lemire benchmark_uniform_sampling.c
        ${CMAKE_SOURCE_DIR}/src/uniform_sampling/natural_lemire_uniform_index_sample.c)
target_link_libraries(benchmark_uniform_sampling_lemire PRIVATE test_interface_avx2)
target_compile_definitions(benchmark_uniform_sampling_lemire PRIVATE SCHEME_NAME="Lemire sampling")
add_test(benchmark_uniform_sampling_lemire benchmark_uniform_sampling_lemire)
set_tests_properties(benchmark_uniform_sampling_lemire PROPERTIES LABEL benchmark)

add_executable(benchmark_uniform_sampling_buffered_lemire benchmark_uniform_sampling.c
        ${CMAKE_SOURCE_DIR}/src/uniform_sampling/natural_buffered_lemire_uniform_index_sample.c)
target_link_libraries(benchmark_uniform_sampling_buffered_lemire PRIVATE test_interface_avx2)
target_compile_definitions(benchmark_uniform_sampling_buffered_lemire PRIVATE SCHEME_NAME="Buffered Lemire sampling")
add_test(benchmark_uniform_sampling_buffered_lemire benchmark_uniform_sampling_buffered_lemire)
set_tests_properties(benchmark_uniform_sampling_buffered_lemire PROPERTIES LABEL benchmark)

add_executable(benchmark_uniform_sampling_ct benchmark_uniform_sampling.c
        ${CMAKE_SOURCE_DIR}/src/uniform_sampling/natural_buffered_uniform_index_sample_ct.c)
target_link_libraries(benchmark_uniform_sampling_ct PRIVATE test_interface_avx2)
target_compile_definitions(benchmark_uniform_sampling_ct PRIVATE SCHEME_NAME="Constant time sampling")
add_test(benchmark_uniform_sampling_ct benchmark_uniform_sampling_ct)
set_tests_properties(benchmark_uniform_sampling_ct PROPERTIES LABEL benchmark)

add_executable(benchmark_uniform_sampling_buffered_ct benchmark_uniform_sampling.c
        ${CMAKE_SOURCE_DIR}/src/uniform_sampling/natural_uniform_index_sample_ct.c)
target_link_libraries(benchmark_uniform_sampling_buffered_ct PRIVATE test_interface_avx2)
target_compile_definitions(benchmark_uniform_sampling_buffered_ct PRIVATE SCHEME_NAME="Constant time sampling")
add_test(benchmark_uniform_sampling_buffered_ct benchmark_uniform_sampling_buffered_ct)
set_tests_properties(benchmark_uniform_sampling_buffered_ct PROPERTIES LABEL benchmark)
