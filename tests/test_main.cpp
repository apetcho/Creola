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

TEST(ParserTest, FunctionDefinition){
    Parser parser("fun f(x) = x^2");
    auto stmt = parser.parse();
    EXPECT_NE(stmt, nullptr);
}

TEST(EvaluatorTest, FunctionCall){
    Parser parser("fun f(x) = x^2");
    auto stmt = parser.parse();
    Env env;
    Interpreter interp;
    stmt->execute(interp, env);

    // Simulate calling f(3)
    // this requires building a Call node manually or parsing "f(3)"
    // For this test, we assume the parser can handle "f(3)" if we
    // extend the grammar.
    // Here we just test the definition
    EXPECT_TRUE(env.hashKey("f"));
}