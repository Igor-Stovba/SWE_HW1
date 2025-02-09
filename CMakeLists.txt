cmake_minimum_required(VERSION 3.10)
project(Spath)

set(CMAKE_CXX_STANDARD 17)

add_library(Spath STATIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/converter.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/spath.cpp"
)

target_include_directories(Spath PUBLIC include)

install(TARGETS Spath
    ARCHIVE DESTINATION /usr/local/lib
)

install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/spath.hpp" 
              "${CMAKE_CURRENT_SOURCE_DIR}/include/edge.hpp"
              "${CMAKE_CURRENT_SOURCE_DIR}/include/converter.hpp"
    DESTINATION /usr/local/include
)


include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.8.0
)
FetchContent_MakeAvailable(Catch2)

add_executable(Tests "${CMAKE_CURRENT_SOURCE_DIR}/src/tests.cpp")
target_link_libraries(Tests PUBLIC Spath)
target_link_libraries(Tests PRIVATE Catch2::Catch2WithMain)

