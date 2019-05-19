# CMake entry point
cmake_minimum_required (VERSION 2.6)
include(CheckIncludeFile)
project (V5CrossCompile)

# shared library
add_library(testLib SHARED V5SharedLibrary.h V5SharedLibrary.cpp lib.h lib.cpp)
target_compile_definitions(testLib PUBLIC DYNAMIC_LIB)

# dynamic loading
add_executable(testEXE V5SharedLibrary.h V5SharedLibrary.cpp mainTest.cpp)

# static loading
add_executable(testEXE2 V5SharedLibrary.h V5SharedLibrary.cpp mainTest.cpp lib.h)
target_compile_definitions(testEXE2 PUBLIC STATIC_LOAD)
target_link_libraries(testEXE2 testLib)

