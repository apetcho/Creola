cmake_minimum_required(VERSION 3.20)
project(Creola LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CREOLA_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(CREOLA_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(CREOLA_LIBRARY_DIR ${CMAKE_SOURCE_DIR}/lib)
set(CREOLA_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)

message("CREOLA_INCLUDE_DIR: ${CREOLA_INCLUDE_DIR}")
message("CREOLA_SOURCE_DIR: ${CREOLA_SOURCE_DIR}")
message("CREOLA_LIBRARY_DIR: ${CREOLA_LIBRARY_DIR}")
message("CREOLA_SOURCE_DIR: ${CREOLA_BINARY_DIR}")

include_directories(${CREOLA_INCLUDE_DIR})

option(CREOLA_BUILD_TEST "Build creola tests to tests its APIs." ON)

# add_library(creola
#     ${CREOLA_SOURCE_DIR}/core/parser.cpp
#     ${CREOLA_SOURCE_DIR}/core/expr.cpp
#     ${CREOLA_SOURCE_DIR}/core/engine.cpp
# )

#set(MY_HOME "${ENV[HOME]}")
message("HOME: ${MY_HOME}")
#set(CREOLA_BUILD_TEST ON)
message("CREOLA_BUILD_TEST: ${CREOLA_BUILD_TEST}")

# -------------
# -*- TESTS -*-
# -------------
if(CREOLA_BUILD_TEST)
    message("Testing Creola APIs ...")
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.0
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()
    include(GoogleTest)

    add_subdirectory(tests)
endif()



# add_library(Creola::Creola INTERFACE IMPORTED)
# set_target_properties(
#     Creola::Creola
#     PROPERTIES
#     IMPORTED_LIBNAME "creola"
#     INTERFACE_INCLUDE_DIRECTORIES "${CREOLA_INCLUDE_DIR}"
# )

#add_subdirectory(src/cli)
#add_subdirectory(src/core)

#[[

# -*- creola-cli -*-

.. code-block:: cmake

    find_package(Armadillo REQUIRED)

    if(Armadillo_FOUND AND NOT TARGET Armadillo::Armadillo)
        add_library(Armadillo::Armadillo INTERFACE IMPORTED)
        set_target_properties(
            Armadillo::Armadillo
            PROPERTIES
            INTERFACE_LINK_LIBRARIES "${ARMADILLO_LIBRARIES}"
            INTERFACE_INCLUDE_DIRECTORIES "${ARMADILLO_INCLUDE_DIRS}"
        )
    endif()

    add_executable(foo foo.cc)
    target_link_libraries(foo PRIVATE Armadillo::Armadillo)

-----------

    # Find backward-compatibility interface for Pspell.
    find_path(
        ASPELL_PSPELL_INCLUDE_DIR
        NAMES pspell.h
        PATH_SUFFIXES pspell
        DOC "Directory containing <pspell.h> BC interface header"
    )
    mark_as_advanced(ASPELL_PSPELL_INCLUDE_DIR)

    find_library(
        ASPELL_LIBRARY
        NAMES aspell aspell-15 libaspell-15 libaspell
        DOC "The path to the Aspell library."
    )
    mark_as_advanced(ASPELL_LIBRARY)



#]]