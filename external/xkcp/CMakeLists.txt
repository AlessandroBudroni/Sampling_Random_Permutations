
set(XKCP_COMMON
        KeccakHash.c
        KeccakSponge.c
        SimpleFIPS202.c
        )

# XKCP opt64
add_library(XKCP STATIC)
target_sources(XKCP PRIVATE
        ${XKCP_COMMON}
        opt64/KeccakP-1600-opt64.c
        )
target_include_directories(XKCP PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/opt64)


# XKCP AVX2
add_library(XKCP_AVX2 STATIC)
target_sources(XKCP_AVX2 PRIVATE
        ${XKCP_COMMON}
        avx2/KeccakP-1600-AVX2.s
        )
target_include_directories(XKCP_AVX2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/avx2)
target_link_options(XKCP_AVX2 PUBLIC -z noexecstack)
target_compile_options(XKCP_AVX2 PRIVATE -mavx2)

if (APPLE)
    set_source_files_properties(avx2/KeccakP-1600-AVX2.S PROPERTIES COMPILE_FLAGS
            "-x assembler-with-cpp -Wa,-defsym,old_gas_syntax=1 -Wa,-defsym,no_plt=1")
endif ()



