#include <gtest/gtest.h>

#include "creola/core/lexer.hpp"
#include "creola/core/ast.hpp"

using namespace creola::core;

class AstTest: public testing::Test{
protected:

    void SetUp(void) override{
        // --
    }

    static Expr getSymbolExpr(void){
        return make_symbol_expr("symbol");
    }

    static Expr getNumberExpr(bool fixed=true){
        if(fixed){ return make_number_expr(i64(1)); }
        return make_number_expr(3.14);
    }

    static Expr getNegExpr(){
        auto expr = make_number_expr();
        return make_neg_expr(expr);
    }
};

//
TEST_F(AstTest, IsSymbolExpr){
    auto expr = getSymbolExpr();
    EXPECT_TRUE(is_symbol_expr(expr));
}

TEST_F(AstTest, IsNumberExpr){
    auto expr = getNumberExpr();
    EXPECT_TRUE(is_number_expr(expr));
    EXPECT_TRUE(is_number_expr(getNumberExpr(false)));
}

TEST_F(AstTest, IsNegExpr){
    auto expr = getNegExpr();
    EXPECT_TRUE(is_neg_expr(expr));
}

TEST_F(AstTest, IsAddExpr){
    auto lhs = make_number_expr();
    auto rhs = make_number_expr();
    auto expr = make_add_expr(lhs, rhs);
    EXPECT_TRUE(is_add_expr(expr));
}

TEST_F(AstTest, IsMulExpr){
    auto lhs = make_number_expr(1.0);
    auto rhs = make_number_expr(2.0);
    auto expr = make_mul_expr(lhs, rhs);
    EXPECT_TRUE(is_mul_expr(expr));
}

TEST_F(AstTest, IsPowExpr){
    auto base = make_add_expr(make_symbol_expr("x"), make_number_expr());
    auto expo = make_number_expr(2ll);
    auto expr = make_pow_expr(base, expo);
    EXPECT_TRUE(is_pow_expr(expr));
}

TEST_F(AstTest, IsCallExpr){
    // x + 2.0
    auto expr = make_add_expr(make_symbol_expr("x"), make_number_expr(2.0));
    EXPECT_TRUE(is_call_expr(make_call_expr("f", Vec<Expr>{expr})));
}

TEST_F(AstTest, IsEquationExpr){
    auto lhs = make_add_expr(make_symbol_expr("x"), make_number_expr(2ll));
    auto rhs = make_number_expr(1ll);
    // x + 2 = 1
    auto equ = make_equation_expr(lhs, rhs);
    EXPECT_TRUE(is_equation_expr(equ));
}

// -
TEST_F(AstTest, IsSystemExpr){
    EquationExpr eq1(make_symbol_expr("x"), make_number_expr());
    EquationExpr eq2(make_symbol_expr("x"), make_symbol_expr("y"));
    Vec<EquationExpr> exprs{eq1, eq2};
    auto sys = make_system_expr(exprs);
    EXPECT_TRUE(is_system_expr(sys));
}