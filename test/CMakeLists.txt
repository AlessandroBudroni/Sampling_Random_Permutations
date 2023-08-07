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

set(SRC_VQSORT
        ${CMAKE_SOURCE_DIR}/src/sorting/vqsort_wrapper.c
        ${CMAKE_SOURCE_DIR}/src/sampling/sort_sample.c
        ${CMAKE_SOURCE_DIR}/src/inversion/sort_inversion.c
        ${CMAKE_SOURCE_DIR}/src/composition/sort_composition.c)

set(SRC_VQSORT_64
        ${CMAKE_SOURCE_DIR}/src/sorting/vqsort_wrapper.c
        ${CMAKE_SOURCE_DIR}/src/sampling/sort_sample_64.c
        ${CMAKE_SOURCE_DIR}/src/inversion/sort_inversion.c
        ${CMAKE_SOURCE_DIR}/src/composition/sort_composition.c)

set(SRC_DJBSORT
        ${CMAKE_SOURCE_DIR}/src/sorting/djbsort_wrapper.c
        ${CMAKE_SOURCE_DIR}/src/sampling/sort_sample.c
        ${CMAKE_SOURCE_DIR}/src/inversion/sort_inversion.c
        ${CMAKE_SOURCE_DIR}/src/composition/sort_composition.c)

set(SRC_DJBSORT_64
        ${CMAKE_SOURCE_DIR}/src/sorting/djbsort_wrapper.c
        ${CMAKE_SOURCE_DIR}/src/sampling/sort_sample_64.c
        ${CMAKE_SOURCE_DIR}/src/inversion/sort_inversion.c
        ${CMAKE_SOURCE_DIR}/src/composition/sort_composition.c)

set(SRC_FY
        ${CMAKE_SOURCE_DIR}/src/sampling/shuffle_sample.c
        ${CMAKE_SOURCE_DIR}/src/shuffling/fisher_yates_shuffle.c
        ${CMAKE_SOURCE_DIR}/src/shuffling/uniform_index_sample.c
        ${CMAKE_SOURCE_DIR}/src/inversion/inversion_substitution.c
        ${CMAKE_SOURCE_DIR}/src/composition/standard_composition.c)

set(SRC_FYS
        ${CMAKE_SOURCE_DIR}/src/sampling/shuffle_sample.c
        ${CMAKE_SOURCE_DIR}/src/shuffling/sendrier_shuffle.c
        ${CMAKE_SOURCE_DIR}/src/shuffling/uniform_index_sample.c
        ${CMAKE_SOURCE_DIR}/src/inversion/inversion_substitution.c
        ${CMAKE_SOURCE_DIR}/src/composition/standard_composition.c)

set(SRC_FYS_AVX2
        ${CMAKE_SOURCE_DIR}/src/sampling/shuffle_sample.c
        ${CMAKE_SOURCE_DIR}/src/shuffling/sendrier_shuffle_avx2.c
        ${CMAKE_SOURCE_DIR}/src/shuffling/uniform_index_sample.c
        ${CMAKE_SOURCE_DIR}/src/inversion/inversion_substitution.c
        ${CMAKE_SOURCE_DIR}/src/composition/standard_composition.c)

set(SRC_FYN
        ${CMAKE_SOURCE_DIR}/src/sampling/shuffle_sample.c
        ${CMAKE_SOURCE_DIR}/src/shuffling/natural_shuffle.c
        ${CMAKE_SOURCE_DIR}/src/shuffling/natural_uniform_index_sample.c
        ${CMAKE_SOURCE_DIR}/src/inversion/inversion_substitution.c
        ${CMAKE_SOURCE_DIR}/src/composition/standard_composition.c)

set(SRC_FYN_AVX2
        ${CMAKE_SOURCE_DIR}/src/sampling/shuffle_sample.c
        ${CMAKE_SOURCE_DIR}/src/shuffling/natural_shuffle_avx2.c
        ${CMAKE_SOURCE_DIR}/src/shuffling/natural_uniform_index_sample.c
        ${CMAKE_SOURCE_DIR}/src/inversion/inversion_substitution.c
        ${CMAKE_SOURCE_DIR}/src/composition/standard_composition.c)


set(DUDECT_TIMEOUT 5)

macro(register_test test_name test_sources test_libraries TIMEOUT_FAIL)
    message(">> register  ${test_name}")
    message(".. sources   ${${test_sources}}")
    message(".. libraries ${${test_libraries}}")
    message(".. Expect Fail ${TIMEOUT_FAIL}")
    add_executable(test_${test_name} test_api.c ${${test_sources}})
    target_link_libraries(test_${test_name} PRIVATE  ${${test_libraries}})
    target_compile_definitions(test_${test_name} PRIVATE SCHEME_NAME="${test_name}")
    set_target_properties(test_${test_name} PROPERTIES LINKER_LANGUAGE C)
    add_test(test_${test_name} test_${test_name})
    set_tests_properties(test_${test_name} PROPERTIES LABEL test)

    add_executable(benchmark_${test_name} benchmark_api.c ${${test_sources}})
    target_link_libraries(benchmark_${test_name} PRIVATE  ${${test_libraries}})
    target_compile_definitions(benchmark_${test_name} PRIVATE SCHEME_NAME="${test_name}")
    set_target_properties(benchmark_${test_name} PROPERTIES LINKER_LANGUAGE C)
    add_test(benchmark_${test_name} benchmark_${test_name})
    set_tests_properties(benchmark_${test_name} PROPERTIES LABEL benchmark)

#    add_executable(dudect_${test_name} dudect_api.c ${${test_sources}})
#    target_link_libraries(dudect_${test_name} PRIVATE  ${${test_libraries}} m)
#    target_compile_definitions(dudect_${test_name} PRIVATE SORT_SAMPLING_ABORT_ENABLED=0)
#    set_target_properties(dudect_${test_name} PROPERTIES LINKER_LANGUAGE C)
#    add_test(dudect_${test_name} dudect_${test_name})
#    set_tests_properties(dudect_${test_name} PROPERTIES LABEL dudect WILL_FAIL ${TIMEOUT_FAIL})
endmacro()

#vqsort
set(hwy_libraries test_interface_avx2 vqsort)
register_test(api_vqsort_32_avx2 SRC_VQSORT hwy_libraries True)
register_test(api_vqsort_64_avx2 SRC_VQSORT_64 hwy_libraries True)

#djbsort
set(djb_libraries test_interface DJBSORT)
register_test(api_djbsort_32 SRC_DJBSORT djb_libraries False)
register_test(api_djbsort_64 SRC_DJBSORT_64 djb_libraries False)

set(djb_libraries_avx2 test_interface_avx2 DJBSORT_AVX2)
register_test(api_djbsort_32_avx2 SRC_DJBSORT djb_libraries_avx2 False)
register_test(api_djbsort_64_avx2 SRC_DJBSORT_64 djb_libraries_avx2 False)

#fisher-yates
set(fy_libraries test_interface)
register_test(api_fisher_yates SRC_FY fy_libraries True)
register_test(api_fisher_yates_sendrier SRC_FYS fy_libraries False)
register_test(api_fisher_yates_natural SRC_FYN fy_libraries False)
#
set(fy_libraries_avx2 test_interface_avx2)
register_test(api_fisher_yates_avx2 SRC_FY fy_libraries_avx2 True)
register_test(api_fisher_yates_sendrier_avx2 SRC_FYS_AVX2 fy_libraries_avx2 False)
register_test(api_fisher_yates_natural_avx2 SRC_FYN_AVX2 fy_libraries_avx2 False)

