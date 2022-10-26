cmake_minimum_required(VERSION 3.21)
project(secure_fisher_yates C)

# help macro
macro(log var)
    message(STATUS "${var}: ${${var}}")
endmacro()

# Other not used flags: -fsanitize=address -Wformat -Wformat-security -Wall -Wextra

execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion
        OUTPUT_VARIABLE GCC_VERSION)
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -mavx2")
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -mavx2 -fsanitize=address -m64 -Wformat=0 -Werror -Wno-unused-function -Wno-unused-result -Wno-strict-prototypes -Wcast-align -Wunused-value -Wunused-variable -Wundef")
else()
    message(FATAL_ERROR "CMAKE_BUILD_TYPE not valid")
endif()

log(CMAKE_C_COMPILER)
log(CMAKE_C_FLAGS)

set(CMAKE_C_STANDARD 11)

add_executable(Benchmark benchmark.c)
target_sources(Benchmark PRIVATE fips202/fips202.c)
target_sources(Benchmark PRIVATE src/fisher_yates.c)
target_sources(Benchmark PRIVATE src/fisher_yates_natural.c)
target_sources(Benchmark PRIVATE src/fisher_yates_AVX2.c)
target_sources(Benchmark PRIVATE src/fisher_yates_natural_AVX2.c)
