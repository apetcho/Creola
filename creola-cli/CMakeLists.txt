#[[==
if(NOT CREOLA_READLINE_INCLUDE_DIR)
    find_path(
        CREOLA_READLINE_INCLUDE_DIR
        NAMES readline.h
        PATHS 
            /opt/homebrew/include
            /opt/homebrew/Cellar/gmp/6.3.0/include
    )
endif()

if(CREOLA_READLINE_INCLUDE_DIR)
    message("CREOLA_READLINE_INCLUDE_DIR: ${CREOLA_READLINE_INCLUDE_DIR}")
endif()

if(NOT CREOLA_READLINE_LIB)
    find_library(CREOLA_READLINE_LIB
        NAMES readline libreadline
        PATHS
            /opt/homebrew/Cellar/readline/8.3.3/lib/
    )
endif()


if(CREOLA_READLINE_LIB)
    message("CREOLA_READLINE_LIB: ${CREOLA_READLINE_LIB}")
endif(CREOLA_READLINE_LIB)

include_directories(${CREOLA_READLINE_INCLUDE_DIR})

add_executable(creola-cli
    creola-cli.cpp creola-cli.hpp
)

target_link_libraries(creola-cli creola readline)
==]]