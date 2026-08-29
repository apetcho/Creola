#include "creola/core/parser.hpp"
#include "creola/core/ast.hpp"
#include "creola/core/evaluator.hpp"

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
Ast Parser::parse(void){
    if(this->m_current.kind==TokenKind::Let){
        return this->parse_let();
    }
    if(this->m_current.kind==TokenKind::Fun){
        return this->parse_fun();
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

void Parser::expect(TokenKind kind, const Str& msg){
    if(this->m_current.kind!=kind){
        throw CreolaError(msg);
    }
}

// -*-
Expr Parser::parse_expr(void){
    return this->parse_add_expr();
}

// -*-
Expr Parser::parse_add_expr(void){
    auto lhs = this->parse_mul_expr();
    while(this->m_current.kind==TokenKind::Plus || this->m_current.kind==TokenKind::Minus){
        if(this->m_current.kind==TokenKind::Plus){
            this->consume(TokenKind::Plus);
            auto rhs = this->parse_mul_expr();
            lhs = make_add_expr(lhs, rhs);
        }else{
            this->consume(TokenKind::Minus);
            auto rhs = this->parse_mul_expr();
            rhs = make_neg_expr(rhs);
            lhs = make_add_expr(lhs, rhs);
        }
    }

    return lhs;
}

// -*-
Expr Parser::parse_mul_expr(void){
    auto lhs = this->parse_pow_expr();
    while(this->m_current.kind==TokenKind::Star || this->m_current.kind==TokenKind::Slash){
        if(this->m_current.kind==TokenKind::Star){
            this->consume(TokenKind::Star);
            auto rhs = this->parse_pow_expr();
            lhs = make_mul_expr(lhs, rhs);
        }else{
            this->consume(TokenKind::Slash);
            auto rhs = this->parse_pow_expr();
            if(is_zero(rhs)){
                throw CreolaError("division by zero");
            }
            rhs = make_pow_expr(rhs, make_number_expr(-1.0));
            lhs = make_mul_expr(lhs, rhs);
        }
    }

    return lhs;
}

// -*-
Expr Parser::parse_pow_expr(void){
    auto expr = this->parse_unary_expr();
    if(this->m_current.kind==TokenKind::Caret){
        this->consume(TokenKind::Caret);
        auto rhs = this->parse_unary_expr();
        expr = make_pow_expr(expr, rhs);
    }

    return expr;
}

// -*-
Expr Parser::parse_unary_expr(void){
    if(this->m_current.kind==TokenKind::Minus){
        this->consume(TokenKind::Minus);
        return make_neg_expr(this->parse_unary_expr());
    }

    return this->parse_primary_expr();
}

// -*-
Expr Parser::parse_primary_expr(void){
    if(this->m_current.kind==TokenKind::Integer || this->m_current.kind==TokenKind::Float){
        if(this->m_current.kind==TokenKind::Integer){
            auto num = std::stoll(this->m_current.text);
            return make_number_expr(num);
        }
        auto num = std::stod(this->m_current.text);
        return make_number_expr(num);
    }

    if(this->m_current.kind==TokenKind::Ident){
        auto name = this->m_current.text;
        this->consume(TokenKind::Ident);
        if(this->m_current.kind==TokenKind::LParen){
            this->consume(TokenKind::LParen);
            Vec<Expr> args{};
            if(this->m_current.kind != TokenKind::RParen){
                args.push_back(this->parse_expr());
                while(this->match(TokenKind::Comma)){
                    if(this->m_current.kind == TokenKind::RParen){ break; }
                    args.push_back(this->parse_expr());
                }
            }
            this->consume(TokenKind::RParen);
            return make_call_expr(name, args);
        }
        return make_symbol_expr(name);
    }

    if(this->m_current.kind==TokenKind::LParen){        // (expr)
        this->consume(TokenKind::LParen);
        auto expr = this->parse_expr();
        this->consume(TokenKind::RParen);
        return expr;
    }

    throw CreolaError("unexpected token in primary-expression: " + this->m_current.text);
}

// -*-
Stmt Parser::parse_let(void){
    // let x = 3.14
    this->consume(TokenKind::Let);
    this->expect(TokenKind::Ident, "expected identifier after `let`");
    auto name = this->m_current.text;
    this->consume(TokenKind::Ident);
    this->consume(TokenKind::Equal);
    auto expr = this->parse_expr();
    if(this->match(TokenKind::Semi)){}

    return make_let_stmt(name, expr);
}

// -*-
Stmt Parser::parse_fun(void){
    // fun f(x) = x + 1
    this->consume(TokenKind::Fun);
    this->expect(TokenKind::Ident, "Expected function name");
    auto fname = this->m_current.text;
    this->consume(TokenKind::Ident);
    this->consume(TokenKind::LParen);
    Vec<Str> params{};
    if(this->m_current.kind==TokenKind::Ident){
        params.push_back(this->m_current.text);
        this->consume(TokenKind::Ident);
        while(this->match(TokenKind::Comma)){
            this->expect(TokenKind::Ident, "Expected parameter name");
            params.push_back(this->m_current.text);
            this->consume(TokenKind::Ident);
        }
    }
    this->consume(TokenKind::RParen);
    this->consume(TokenKind::Equal);
    auto body = this->parse_expr();
    auto lambda = make_lambda_expr(params, body);
    return make_fun_stmt(fname, as_lambda_expr(lambda));
}

// -*-
Expr Parser::parse_equation(void){
    //! @todo
    throw CreolaError("`equation()`: not implemented yet.");
}

// -*-
Expr Parser::parse_system(void){
    //! @todo
    throw CreolaError("`system()`: not implemented yet.");
}

// -*-
Expr Parser::parse_simplify(void){
    //! @todo
    throw CreolaError("`simplify()`: not implemented yet.");
}

// -*-
Expr Parser::parse_diff(void){
    // diff(expr, var);
    this->consume(TokenKind::Diff);
    this->consume(TokenKind::LParen);
    auto expr = this->parse_expr();
    this->consume(TokenKind::Comma);
    this->expect(TokenKind::Ident, "Expected variable in derivative");
    auto var = this->m_current.text;
    this->consume(TokenKind::Ident);
    this->consume(TokenKind::RParen);

    return Evaluator::diff(expr, var);
}

// -*-
Expr Parser::parse_expand(void){
    // expand(expr)
    this->consume(TokenKind::Expand);
    this->consume(TokenKind::LParen);
    auto expr = this->parse_expr();
    this->consume(TokenKind::RParen);

    return Evaluator::expand(expr);
}

// -*-
Expr Parser::parse_factor(void){
    // factor(expr)
    this->consume(TokenKind::Factor);
    this->consume(TokenKind::LParen);
    auto expr = this->parse_expr();
    this->consume(TokenKind::RParen);

    return Evaluator::factor(expr);
}

// -*-
Expr Parser::parse_integral(void){
    // integral(expr, var)
    this->consume(TokenKind::Integral);
    this->consume(TokenKind::LParen);
    auto expr = this->parse_expr();
    this->consume(TokenKind::Comma);
    this->expect(TokenKind::Ident, "Expected variable in integral");
    auto var = this->m_current.text;
    this->consume(TokenKind::Ident);
    this->consume(TokenKind::RParen);
    
    return Evaluator::integral(expr, var);
}

// -*-
Expr Parser::parse_integrate(void){
    // integrate(expr, var, vmin, vmax)
    this->consume(TokenKind::Integral);
    this->consume(TokenKind::LParen);
    auto expr = this->parse_expr();
    this->consume(TokenKind::Comma);
    this->expect(TokenKind::Ident, "Expected variable in integral");
    auto var = this->m_current.text;
    this->consume(TokenKind::Ident);
    this->consume(TokenKind::Comma);
    f64 vmin{};
    if(this->m_current.kind==TokenKind::Integer){
        vmin = static_cast<f64>(std::stoll(this->m_current.text));
        this->consume(TokenKind::Integer);
    }else if(this->m_current.kind==TokenKind::Float){
        vmin = std::stod(this->m_current.text);
        this->consume(TokenKind::Float);
    }else{
        throw CreolaError("`integrate()`: expected a number.");
    }
    this->consume(TokenKind::Comma);
    f64 vmax{};
    if(this->m_current.kind==TokenKind::Integer){
        vmax = static_cast<f64>(std::stoll(this->m_current.text));
        this->consume(TokenKind::Integer);
    }else if(this->m_current.kind==TokenKind::Float){
        vmax = std::stod(this->m_current.text);
        this->consume(TokenKind::Float);
    }else{
        throw CreolaError("`integrate()`: expected a number.");
    }
    this->consume(TokenKind::RParen);

    return Evaluator::integrate(expr, var, vmin, vmax);
}

// -*-
Expr Parser::parse_taylor(void){
    // taylor(expr, var, center, order)
    this->consume(TokenKind::Taylor);
    this->consume(TokenKind::LParen);
    auto expr = this->parse_expr();
    this->consume(TokenKind::Comma);
    this->expect(TokenKind::Ident, "Expected variable in integral");
    auto var = this->m_current.text;
    this->consume(TokenKind::Ident);
    this->consume(TokenKind::Comma);
    f64 center{};
    if(this->m_current.kind==TokenKind::Integer){
        center = static_cast<f64>(std::stoll(this->m_current.text));
        this->consume(TokenKind::Integer);
    }else if(this->m_current.kind==TokenKind::Float){
        center = std::stod(this->m_current.text);
        this->consume(TokenKind::Float);
    }else{
        throw CreolaError("`taylor()`: expected a number");
    }
    this->expect(TokenKind::Integer, "`taylor()`: expected an integer");
    auto order = static_cast<u32>(std::stoul(this->m_current.text));
    this->consume(TokenKind::Integer);
    this->consume(TokenKind::RParen);

    return Evaluator::taylor(expr, var, center, order);
}

// -*-
Expr Parser::parse_limit(void){
    //! @todo
    throw CreolaError("`limit()`: not implemented yet.");
}

// -*-
Expr Parser::parse_roots(void){
    //! @todo
    throw CreolaError("`roots()`: not implemented yet.");
}

// -*-
Expr Parser::parse_solve(void){
    //! @todo
    throw CreolaError("`solve()`: not implemented yet.");
}

// -*-
Expr Parser::parse_fibonacci(void){
    //! @todo
    throw CreolaError("`fibonacci()`: not implemented yet.");
}

// -*-
Expr Parser::parse_factorial(void){
    //! @todo
    throw CreolaError("`factorial()`: not implemented yet.");
}

// -*-
Expr Parser::parse_modulo(void){
    //! @todo
    throw CreolaError("`modulo()`: not implemented yet.");
}

// -*-
Expr Parser::parse_prime(void){
    //! @todo
    throw CreolaError("`prime()`: not implemented yet.");
}

// -*-
Expr Parser::parse_gcd(void){
    //! @todo
    throw CreolaError("`gcd()`: not implemented yet.");
}

// -*-
Expr Parser::parse_lcm(void){
    //! @todo
    throw CreolaError("`lcm()`: not implemented yet.");
}

// -*-
Expr Parser::parse_help(void){
    //! @todo
    throw CreolaError("`help()`: not implemented yet.");
}

// -*-
Expr Parser::parse_show(void){
    //! @todo
    throw CreolaError("`show()`: not implemented yet.");
}

// -*-
Expr Parser::parse_plot(void){
    //! @todo
    throw CreolaError("`plot()`: not implemented yet.");
}

// -*-
Expr Parser::parse_config(void){
    //! @todo
    throw CreolaError("`config()`: not implemented yet.");
}


/*
// -*-
Expr Parser::parse_integrate(void){
    // integrate(expr, var)
    bool evaled{false};
    Expr x1{}, x2{}, result{};
    this->consume(TokenKind::INTEGRATE);
    this->consume(TokenKind::LPAREN);
    auto expr = this->parse_expr();
    this->consume(TokenKind::COMMA);
    this->expect(TokenKind::IDENT, "Expected variable in integral");
    auto var = this->m_current.text;
    this->consume(TokenKind::IDENT);
    if(this->m_current.kind == TokenKind::COMMA){
        x1 = this->parse_expr();
        this->expect(TokenKind::COMMA, "Expected comma in intregal evaluation");
        x2 = this->parse_expr();
    }
    this->consume(TokenKind::RPAREN);

    if(evaled){
        result = CasEngine::integrate(expr, var, x1, x2);
    }else{
        result = CasEngine::integrate(expr, var);
    }
    return CasEngine::simplify(result);
}
*/

/*
// -*- Parser
class Parser{
public:




private:
    Lexer m_lexer;
    Token m_current;

};
*/

// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola::core                                 -*-
// -*----------------------------------------------------------------*-