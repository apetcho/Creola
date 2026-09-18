#include<gtest/gtest.h>
#include "core/parser.hpp"
#include "core/evaluator.hpp"
#include "core/env.hpp"

using namespace creola;

TEST(ParserTest, BasicArithmetic){
    Parser parser("let x = 1 + 2 * 3");
    auto stmt = parser.parse();
    EXPECT_NE(stmt, nullptr);
}


// TEST(EvaluatorTest, BasicArithmetic){}

// TEST(ParserTest, FunctionDefinition){}

// TEST(EvaluatorTest, FunctionCall){}


/*

*/