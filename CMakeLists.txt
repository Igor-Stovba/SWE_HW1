cmake_minimum_required(VERSION 3.10)

project(Spath)

set(CMAKE_CXX_STANDARD 17)

# Fetch Catch2 instead of including it as a directory
include(FetchContent)
FetchContent_Declare(
	Catch2
   	GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.8.0 
)
FetchContent_MakeAvailable(Catch2)

# Fetch Google Benchmark of including is as a directory
include(FetchContent)
FetchContent_Declare(
	benchmark
	GIT_REPOSITORY https://github.com/google/benchmark.git
	GIT_TAG v1.9.1	
)
FetchContent_MakeAvailable(benchmark)

add_executable(Tests "${CMAKE_CURRENT_SOURCE_DIR}/tests/tests.cpp")
target_link_libraries(Tests PRIVATE Catch2::Catch2WithMain)

add_custom_target(Install 
        COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/include/spath.hpp"   
        "/usr/local/include/"
)

target_include_directories(Tests 
        PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

