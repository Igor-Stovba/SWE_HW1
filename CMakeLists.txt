cmake_minimum_required(VERSION 3.10)

project(Spath)

set(CMAKE_CXX_STANDARD 17)

option(MAKE_INSTALL "Define for installation" OFF)
option(MAKE_TESTS "Define for building tests" OFF)



add_library(Spath STATIC "${CMAKE_CURRENT_SOURCE_DIR}/src/converter.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/spath.cpp")

target_include_directories(Spath PUBLIC include)


if (MAKE_TESTS)
        # Fetch Catch2 instead of including it as a directory
        include(FetchContent)
        FetchContent_Declare(
                Catch2
                GIT_REPOSITORY https://github.com/catchorg/Catch2.git
                GIT_TAG v3.8.0 
        )
        FetchContent_MakeAvailable(Catch2)

        add_executable(Tests "${CMAKE_CURRENT_SOURCE_DIR}/src/tests.cpp")
        target_link_libraries(Tests PRIVATE Catch2::Catch2WithMain)

        target_include_directories(Tests 
                PRIVATE
                "${CMAKE_CURRENT_SOURCE_DIR}/include"
        )

        target_link_libraries(Tests PUBLIC Spath)
endif()

if (MAKE_INSTALL)
        install(TARGETS Spath DESTINATION /usr/local/lib)
        install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION /usr/local/include/spath FILES_MATCHING PATTERN "*.hpp" )
endif()


