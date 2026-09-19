
set(CMAKE_BUILD_TYPE "Debug")
FetchContent_Declare(
    replxx
    GIT_REPOSITORY "https://github.com/apetcho/replxx.git"
    GIT_SHALLOW     TRUE
)
FetchContent_MakeAvailable(replxx)

if(NOT TARGET replxx::replxx)
    add_library(replxx::replxx ALIAS replxx)
endif()

set(CREOLA_LIB_HEADERS
    ./ast.hpp
    ./env.hpp
    ./tokenizer.hpp
    ./parser.hpp
    ./evaluator.hpp
)

set(CREOLA_LIB_SOURCES
    core/ast.cpp
    core/env.cpp
    core/tokenizer.cpp
    core/parser.cpp
    core/evaluator.cpp
)

set(CREAOLA_CLI_HEADERS core/repl.hpp)
set(CREAOLA_CLI_SOURCES ./cli_main.cpp core/repl.cpp)

add_library(creola_core ${CREOLA_LIB_SOURCES} ${CREOLA_LIB_HEADERS})

add_executable(creola_cli ${CREAOLA_CLI_SOURCES} ${CREAOLA_CLI_HEADERS})
target_link_libraries(creola_cli PRIVATE creola_core replxx::replxx)