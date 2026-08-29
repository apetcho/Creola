#pragma once

#include<string>
#include "creola/core/lexer.hpp"

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola::core                               -*-
// -*----------------------------------------------------------------*-
namespace creola::core{
//

// -*- Parser
class Parser{
public:
    explicit Parser(const Str& src);
    ~Parser() = default;

    Ast parse(Evaluator& evaluator);

private:
    Lexer m_lexer;
    Token m_current;

    void consume(TokenKind kind);
    bool match(TokenKind kind);
    void expect(TokenKind kind, const Str& msg);

    Stmt parse_let(void);
    Stmt parse_fun(void);

    Expr parse_equation(Evaluator& evaluator);
    Expr parse_system(Evaluator& evaluator);
    Expr parse_simplify(Evaluator& evaluator);
    Expr parse_diff(Evaluator& evaluator);
    Expr parse_expand(Evaluator& evaluator);
    Expr parse_factor(Evaluator& evaluator);
    Expr parse_integral(Evaluator& evaluator);
    Expr parse_integrate(Evaluator& evaluator);
    Expr parse_taylor(Evaluator& evaluator);
    Expr parse_limit(Evaluator& evaluator);
    Expr parse_roots(Evaluator& evaluator);
    Expr parse_solve(Evaluator& evaluator);
    Expr parse_fibonacci(Evaluator& evaluator);
    Expr parse_factorial(Evaluator& evaluator);
    Expr parse_modulo(Evaluator& evaluator);
    Expr parse_prime(Evaluator& evaluator);
    Expr parse_gcd(Evaluator& evaluator);
    Expr parse_lcm(Evaluator& evaluator);
    Expr parse_help(Evaluator& evaluator);
    Expr parse_show(Evaluator& evaluator);
    Expr parse_plot(Evaluator& evaluator);
    Expr parse_config(Evaluator& evaluator);

    Expr parse_expr(void);
    Expr parse_add_expr(void);
    Expr parse_mul_expr(void);
    Expr parse_pow_expr(void);
    Expr parse_unary_expr(void);
    Expr parse_primary_expr(void);
};



// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola::core                                 -*-
// -*----------------------------------------------------------------*-