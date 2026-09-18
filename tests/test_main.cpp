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


TEST(EvaluatorTest, BasicArithmetic){
    Parser parser("let x = 2 + 3");
    auto stmt = parser.parse();
    Env env;
    Interpreter interp;
    stmt->execute(interp, env);

    Expr expr = env.get("x");
    auto val = expr->eval(interp, env);
    auto ans = std::dynamic_pointer_cast<Number>(val);
    EXPECT_NE(ans, nullptr);
    Value num = ans->value;
    EXPECT_DOUBLE_EQ(std::get<double>(num), 5.0);
}


// TEST(ParserTest, FunctionDefinition){}

// TEST(EvaluatorTest, FunctionCall){}


/*

*/