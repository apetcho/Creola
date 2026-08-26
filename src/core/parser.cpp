#include "creola/core/parser.hpp"

#include<cctype>
#include<string>
#include<stdexcept>

/*
// -*-
Expr Parser::parse(void){
    // parse a statement or expression
    //if(this->m_current.kind==TokenKind::END){ return ExprNode::make_none(); }
    if(this->m_current.kind==TokenKind::LET){ return this->parse_let(); }
    if(this->m_current.kind==TokenKind::FUN){ return this->parse_fun(); }
    if(this->m_current.kind==TokenKind::DIFF){ return this->parse_diff(); }
    if(this->m_current.kind==TokenKind::INTEGRATE){ return this->parse_integrate(); }

    auto expr = this->parse_expr();
    if(this->match(TokenKind::SEMI)){ return expr; } // parse optional semicolon
    return expr;
}
*/

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola::core                               -*-
// -*----------------------------------------------------------------*-
namespace creola::core{
// -

// -*-
Parser::Parser(const Str& src)
: m_lexer{src}
{
    this->m_current = this->m_lexer.next();
}

/*
// -*- Parser
class Parser{
public:

Expr Parser::parse(void){}

void Parser::consume(TokenKind kind){}
bool Parser::match(TokenKind kind){}
void Parser::expect(TokenKind kind, const Str& msg){}

Stmt Parser::parse_let(void){}
Stmt Parser::parse_fun(void){}

Expr Parser::parse_equation(void){}
Expr Parser::parse_system(void){}
Expr Parser::parse_simplify(void){}
Expr Parser::parse_diff(void){}
Expr Parser::parse_expand(void){}
Expr Parser::parse_factor(void){}
Expr Parser::parse_integral(void){}
Expr Parser::parse_integrate(void){}
Expr Parser::parse_taylor(void){}
Expr Parser::parse_limit(void){}
Expr Parser::parse_roots(void){}
Expr Parser::parse_solve(void){}
Expr Parser::parse_fibonacci(void){}
Expr Parser::parse_factorial(void){}
Expr Parser::parse_modulo(void){}
Expr Parser::parse_prime(void){}
Expr Parser::parse_help(void){}
Expr Parser::parse_show(void){}
Expr Parser::parse_plot(void){}
Expr Parser::parse_config(void){}

Expr Parser::parse_expr(void){}
Expr Parser::parse_add_expr(void){}
Expr Parser::parse_mul_expr(void){}
Expr Parser::parse_pow_expr(void){}
Expr Parser::parse_unary_expr(void){}
Expr Parser::parse_primary_expr(void){}

private:
    Lexer m_lexer;
    Token m_current;

};
*/

// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola::core                                 -*-
// -*----------------------------------------------------------------*-