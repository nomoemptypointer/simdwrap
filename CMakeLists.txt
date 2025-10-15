cmake_minimum_required(VERSION 3.15)
project(simdwrap C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_library(simdwrap STATIC
        src/simdwrap.c
        src/simdwrap_scalar.c
)

# Include directories for this library
target_include_directories(simdwrap
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Compiler-specific optimization flags
if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(simdwrap PRIVATE -march=native -O3)
elseif (MSVC)
    target_compile_options(simdwrap PRIVATE /O2)
endif()
