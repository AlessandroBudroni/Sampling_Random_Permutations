# vqsort
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

file(GLOB HWY_VQSORT_SOURCES "hwy/contrib/sort/vqsort_*.cc")
list(APPEND HWY_VQSORT_SOURCES
        hwy/contrib/sort/order.h
        hwy/contrib/sort/shared-inl.h
        hwy/contrib/sort/sorting_networks-inl.h
        hwy/contrib/sort/traits-inl.h
        hwy/contrib/sort/traits128-inl.h
        hwy/contrib/sort/vqsort-inl.h
        hwy/contrib/sort/vqsort.cc
        hwy/contrib/sort/vqsort.h
        hwy/per_target.cc
        hwy/targets.cc
        vqsort_cwrapper.cpp
        )

add_library(hwy_vqsort STATIC ${HWY_VQSORT_SOURCES})
target_include_directories(hwy_vqsort PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(hwy_vqsort PRIVATE ${HWY_FLAGS})
# end vqsort
