set(CMAKE_CXX_EXTENSIONS OFF)

set(HWY_SOURCES
        hwy/base.h
        hwy/cache_control.h
        hwy/detect_compiler_arch.h
        hwy/detect_targets.h
        hwy/foreach_target.h
        hwy/highway.h
        hwy/highway_export.h
        hwy/ops/arm_neon-inl.h
        hwy/ops/arm_sve-inl.h
        hwy/ops/emu128-inl.h
        hwy/ops/generic_ops-inl.h
        hwy/ops/ppc_vsx-inl.h
        hwy/ops/rvv-inl.h
        hwy/ops/scalar-inl.h
        hwy/ops/set_macros-inl.h
        hwy/ops/shared-inl.h
        hwy/ops/tuple-inl.h
        hwy/ops/wasm_128-inl.h
        hwy/ops/wasm_256-inl.h
        hwy/ops/x86_128-inl.h
        hwy/ops/x86_256-inl.h
        hwy/ops/x86_512-inl.h
        hwy/per_target.cc
        hwy/per_target.h
        hwy/targets.cc
        hwy/targets.h
        )
file(GLOB HWY_VQSORT_SOURCES "hwy/contrib/sort/vqsort_*.cc")
list(APPEND HWY_VQSORT_SOURCES
        hwy/contrib/algo/copy-inl.h
        hwy/contrib/sort/order.h
        hwy/contrib/sort/shared-inl.h
        hwy/contrib/sort/sorting_networks-inl.h
        hwy/contrib/sort/traits-inl.h
        hwy/contrib/sort/traits128-inl.h
        hwy/contrib/sort/vqsort-inl.h
        hwy/contrib/sort/vqsort.cc
        hwy/contrib/sort/vqsort.h
        vqsort_cwrapper.cpp
        )

if (MSVC)
    set(HWY_FLAGS
            # fix build error C1128 in blockwise*_test & arithmetic_test
            /bigobj
            )
else()
    set(HWY_FLAGS
            # Avoid changing binaries based on the current time and date.
            -Wno-builtin-macro-redefined
            -D__DATE__="redacted"
            -D__TIMESTAMP__="redacted"
            -D__TIME__="redacted"
            # Optimizations
            -fmerge-all-constants
            # Warnings
            -Wall
            -Wextra
            # These are not included in Wall nor Wextra:
            -Wconversion
            -Wsign-conversion
            -Wvla
            -Wnon-virtual-dtor
            )

    if(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
        list(APPEND HWY_FLAGS
                -Wfloat-overflow-conversion
                -Wfloat-zero-conversion
                -Wfor-loop-analysis
                -Wgnu-redeclared-enum
                -Winfinite-recursion
                -Wself-assign
                -Wstring-conversion
                -Wtautological-overlap-compare
                -Wthread-safety-analysis
                -Wundefined-func-template
                -fno-cxx-exceptions
                -fno-slp-vectorize
                -fno-vectorize
                # Use color in messages
                -fdiagnostics-show-option -fcolor-diagnostics
                )
        if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 6.0)
            list(APPEND HWY_FLAGS -Wc++2a-extensions)
        endif()
    endif()

    if (WIN32)
        if(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
            list(APPEND HWY_FLAGS
                    -Wno-global-constructors
                    -Wno-language-extension-token
                    -Wno-used-but-marked-unused
                    -Wno-shadow-field-in-constructor
                    -Wno-unused-member-function
                    -Wno-unused-template
                    -Wno-c++98-compat-pedantic
                    -Wno-used-but-marked-unused
                    -Wno-zero-as-null-pointer-constant
                    )
        endif()

        list(APPEND HWY_FLAGS
                -Wno-cast-align
                -Wno-double-promotion
                -Wno-float-equal
                -Wno-format-nonliteral
                -Wno-shadow
                -Wno-sign-conversion
                )
    else()
        list(APPEND HWY_FLAGS
                -fmath-errno
                -fno-exceptions
                )
    endif()  # WIN32

    # Workaround for excess precision, see #1488.
    if (HWY_CMAKE_SSE2)
        list(APPEND HWY_FLAGS -msse2 -mfpmath=sse)
    endif()

    if (HWY_CMAKE_ARM7)
        list(APPEND HWY_FLAGS
                -march=armv7-a
                -mfpu=neon-vfpv4
                -mfloat-abi=hard  # must match the toolchain specified as CXX=
                -mfp16-format=ieee  # required for vcvt_f32_f16
                )
    endif()  # HWY_CMAKE_ARM7

    if(HWY_RISCV)
        # gcc(13) and recent clang both support V, but not yet runtime dispatch, so
        # we add the gcv compiler flag, which then requires the CPU (now when using
        # either compiler) to support V.
        list(APPEND HWY_FLAGS -march=rv64gcv1p0)
        if(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
            list(APPEND HWY_FLAGS -menable-experimental-extensions)
        endif()
    endif()

    if (HWY_WARNINGS_ARE_ERRORS)
        list(APPEND HWY_FLAGS -Werror)
    endif()

    # Prevent "wasm-ld: error: --shared-memory is disallowed by targets.cc.o
    # because it was not compiled with 'atomics' or 'bulk-memory' features."
    if (HWY_EMSCRIPTEN)
        list(APPEND HWY_FLAGS -matomics)
    endif()

endif()  # !MSVC

add_library(vqsort STATIC ${HWY_SOURCES} ${HWY_VQSORT_SOURCES})
target_include_directories(vqsort PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(vqsort PRIVATE ${HWY_FLAGS})
