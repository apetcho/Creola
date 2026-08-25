add_library(lexer
    ${CREOLA_SOURCE_DIR}/core/lexer.cpp
    ${CREOLA_INCLUDE_DIR}/creola/core/lexer.hpp
)

add_library(ast
    ${CREOLA_SOURCE_DIR}/core/ast.cpp
    ${CREOLA_INCLUDE_DIR}/creola/core/ast.hpp
)

# -*-------------------*-
# -*- CREOLA_ADD_TEST -*-
# -*-------------------*-
function(creola_add_test tsuffix tlibs)
    set(MY_TARGET test_${tsuffix})
    add_executable(${MY_TARGET} ${MY_TARGET}.cpp)
    target_link_libraries(${MY_TARGET} GTest::gtest_main ${tlibs})
    gtest_discover_tests(${MY_TARGET})
endfunction(creola_add_test target)


# -*- Lexer
creola_add_test(lexer lexer)
creola_add_test(ast ast)