#include "creola/core/parser.hpp"

#include<cctype>
#include<string>
#include<stdexcept>


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

// -*-
Option<Expr> Parser::parse(void){
    if(this->m_current.kind==TokenKind::Let){
        this->parse_let();
        return std::nullopt;
    }
    if(this->m_current.kind==TokenKind::Fun){
        this->parse_fun();
        return std::nullopt;
    }

    // equation
    if(this->m_current.kind==TokenKind::Equation){
        return this->parse_equation();
    }
    // system
    if(this->m_current.kind==TokenKind::System){
        return this->parse_system();
    }
    // simplify
    if(this->m_current.kind==TokenKind::Simplify){
        return this->parse_simplify();
    }
    // diff
    if(this->m_current.kind==TokenKind::Diff){
        return this->parse_diff();
    }
    // expand
    if(this->m_current.kind==TokenKind::Expand){
        return this->parse_expand();
    }
    // fatcor
    if(this->m_current.kind==TokenKind::Factor){
        return this->parse_factor();
    }
    // integral
    if(this->m_current.kind==TokenKind::Integral){
        return this->parse_integral();
    }
    // integrate
    if(this->m_current.kind==TokenKind::Integrate){
        this->parse_integrate();
    }
    // taylor
    if(this->m_current.kind==TokenKind::Taylor){
        return this->parse_taylor();
    }
    // limit
    if(this->m_current.kind==TokenKind::Limit){
        return this->parse_limit();
    }
    // roots
    if(this->m_current.kind==TokenKind::Roots){
        return this->parse_roots();
    }
    // solve
    if(this->m_current.kind==TokenKind::Solve){
        return this->parse_solve();
    }
    // fibonacci
    if(this->m_current.kind==TokenKind::Fibonacci){
        return this->parse_fibonacci();
    }
    // factorial
    if(this->m_current.kind==TokenKind::Factorial){
        return this->parse_factorial();
    }
    // modulo
    if(this->m_current.kind==TokenKind::Modulo){
        return this->parse_modulo();
    }
    // prime
    if(this->m_current.kind==TokenKind::Prime){
        return this->parse_prime();
    }
    // gcd
    if(this->m_current.kind==TokenKind::Gcd){
        return this->parse_gcd();
    }
    // lcm
    if(this->m_current.kind==TokenKind::Lcm){
        return this->parse_lcm();
    }

    // help
    if(this->m_current.kind==TokenKind::Help){
        return this->parse_help();
    }
    // show
    if(this->m_current.kind==TokenKind::Show){
        return this->parse_show();
    }
    // plot
    if(this->m_current.kind==TokenKind::Plot){
        return this->parse_plot();
    }
    // config
    if(this->m_current.kind==TokenKind::Config){
        return this->parse_config();
    }

    auto expr = this->parse_expr();
    // consume optional semicolon
    if(this->match(TokenKind::Semi)){
        return expr;
    }
    return expr;
}

/*

// -*-
bool Parser::match(TokenKind kind){
    if(this->m_current.kind == kind){
        // this->m_current = this->peek();
        // this->advance();
        this->m_current = this->m_lexer.next();
        return true;
    }
    return false;
}

void Parser::expect(TokenKind kind, const Str& msg){
    if(this->m_current.kind!=kind){
        throw CasError(msg);
    }
}
*/

// -*-
void Parser::consume(TokenKind kind){
    if(this->m_current.kind != kind){
        throw CreolaError("Unexpected token: " + this->m_current.text);
    }
    this->m_current = this->m_lexer.next();
}

// -*-
bool Parser::match(TokenKind kind){
    if(this->m_current.kind == kind){
        this->m_current = this->m_lexer.next();
        return true;
    }
    return false;
}

void Parser::expect(TokenKind kind, const Str& msg){}



/*
// -*- Parser
class Parser{
public:


void Parser::parse_let(void){}
void Parser::parse_fun(void){}

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
Expr Parser::parse_gcd(void);
Expr Parser::parse_lcm(void);
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