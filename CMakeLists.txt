cmake_minimum_required(VERSION 3.10)

project(Spath)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(Catch2)
add_executable(Tests "${CMAKE_CURRENT_SOURCE_DIR}/tests/tests.cpp")
target_link_libraries(Tests PRIVATE Catch2::Catch2WithMain)

add_custom_target(Install 
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/include/spath.hpp"   
    "/usr/local/include/"
)

target_include_directories(Tests 
        PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include")



