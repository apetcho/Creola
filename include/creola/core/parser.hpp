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

    Ast parse(void);

private:
    Lexer m_lexer;
    Token m_current;

    void consume(TokenKind kind);
    bool match(TokenKind kind);
    void expect(TokenKind kind, const Str& msg);

    Stmt parse_let(void);
    Stmt parse_fun(void);

    Expr parse_equation(void);
    Expr parse_system(void);
    Expr parse_simplify(void);
    Expr parse_diff(void);
    Expr parse_expand(void);
    Expr parse_factor(void);
    Expr parse_integral(void);
    Expr parse_integrate(void);
    Expr parse_taylor(void);
    Expr parse_limit(void);
    Expr parse_roots(void);
    Expr parse_solve(void);
    Expr parse_fibonacci(void);
    Expr parse_factorial(void);
    Expr parse_modulo(void);
    Expr parse_prime(void);
    Expr parse_gcd(void);
    Expr parse_lcm(void);
    Expr parse_help(void);
    Expr parse_show(void);
    Expr parse_plot(void);
    Expr parse_config(void);

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