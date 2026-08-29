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
Ast Parser::parse(Evaluator& evaluator){
    if(this->m_current.kind==TokenKind::Let){
        return this->parse_let();
    }
    if(this->m_current.kind==TokenKind::Fun){
        return this->parse_fun();
    }

    // equation
    if(this->m_current.kind==TokenKind::Equation){
        return this->parse_equation(evaluator);
    }
    // system
    if(this->m_current.kind==TokenKind::System){
        return this->parse_system(evaluator);
    }
    // simplify
    if(this->m_current.kind==TokenKind::Simplify){
        return this->parse_simplify(evaluator);
    }
    // diff
    if(this->m_current.kind==TokenKind::Diff){
        return this->parse_diff(evaluator);
    }
    // expand
    if(this->m_current.kind==TokenKind::Expand){
        return this->parse_expand(evaluator);
    }
    // fatcor
    if(this->m_current.kind==TokenKind::Factor){
        return this->parse_factor(evaluator);
    }
    // integral
    if(this->m_current.kind==TokenKind::Integral){
        return this->parse_integral(evaluator);
    }
    // integrate
    if(this->m_current.kind==TokenKind::Integrate){
        this->parse_integrate(evaluator);
    }
    // taylor
    if(this->m_current.kind==TokenKind::Taylor){
        return this->parse_taylor(evaluator);
    }
    // limit
    if(this->m_current.kind==TokenKind::Limit){
        return this->parse_limit(evaluator);
    }
    // roots
    if(this->m_current.kind==TokenKind::Roots){
        return this->parse_roots(evaluator);
    }
    // solve
    if(this->m_current.kind==TokenKind::Solve){
        return this->parse_solve(evaluator);
    }
    // fibonacci
    if(this->m_current.kind==TokenKind::Fibonacci){
        return this->parse_fibonacci(evaluator);
    }
    // factorial
    if(this->m_current.kind==TokenKind::Factorial){
        return this->parse_factorial(evaluator);
    }
    // modulo
    if(this->m_current.kind==TokenKind::Modulo){
        return this->parse_modulo(evaluator);
    }
    // prime
    if(this->m_current.kind==TokenKind::Prime){
        return this->parse_prime(evaluator);
    }
    // gcd
    if(this->m_current.kind==TokenKind::Gcd){
        return this->parse_gcd(evaluator);
    }
    // lcm
    if(this->m_current.kind==TokenKind::Lcm){
        return this->parse_lcm(evaluator);
    }

    // help
    if(this->m_current.kind==TokenKind::Help){
        return this->parse_help(evaluator);
    }
    // show
    if(this->m_current.kind==TokenKind::Show){
        return this->parse_show(evaluator);
    }
    // plot
    if(this->m_current.kind==TokenKind::Plot){
        return this->parse_plot(evaluator);
    }
    // config
    if(this->m_current.kind==TokenKind::Config){
        return this->parse_config(evaluator);
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
    if(this->match(TokenKind::Semi)){}
    auto lambda = make_lambda_expr(params, body);
    return make_fun_stmt(fname, as_lambda_expr(lambda));
}

// -*-
Expr Parser::parse_equation(Evaluator& evaluator){
    // equation(lhs=rhs, var)
    this->consume(TokenKind::Equation);
    this->consume(TokenKind::LParen);
    auto lhs = this->parse_expr();
    this->consume(TokenKind::Equal);
    auto rhs = this->parse_expr();
    this->expect(TokenKind::Ident, "`equation()`: expected a variable");
    auto var = this->m_current.text;
    this->consume(TokenKind::Ident);
    this->consume(TokenKind::RParen);

    return Evaluator::equation(evaluator, lhs, rhs, var);
}

// -*-
Expr Parser::parse_system(Evaluator& evaluator){
    // system({lhs1=rhs1,lhs2=rhs2, ...}, {...})
    Vec<Expr> lhs{}, rhs{};
    Vec<Str> vars{};
    this->consume(TokenKind::System);
    this->consume(TokenKind::LParen);
    this->expect(TokenKind::LBrace, "`system()`: expected '{'.");
    this->consume(TokenKind::LBrace);
    while(this->m_current.kind!=TokenKind::RBrace){
        auto _lhs = this->parse_expr();
        lhs.push_back(std::move(_lhs));
        this->consume(TokenKind::Equal);
        auto _rhs = this->parse_expr();
        rhs.push_back(std::move(_rhs));
        if(this->m_current.kind==TokenKind::Comma){
            this->consume(TokenKind::Comma);
            continue;
        }
    }
    this->expect(TokenKind::RBrace, "`system()`: expected '}'.");
    this->consume(TokenKind::RBrace);
    this->consume(TokenKind::Comma);
    this->expect(TokenKind::LBrace, "`system()`: expected '{'.");
    this->consume(TokenKind::LBrace);
    while(this->m_current.kind!=TokenKind::RBrace){
        this->expect(TokenKind::Ident, "`system()`: expected a variable.");
        vars.push_back(this->m_current.text);
        this->consume(TokenKind::Ident);
        if(this->m_current.kind==TokenKind::Comma){
            this->consume(TokenKind::Comma);
            continue;
        }
    }
    this->expect(TokenKind::RBrace, "`system()`: expected '}'.");
    this->consume(TokenKind::RBrace);

    return Evaluator::system(evaluator, lhs, rhs, vars);
}

// -*-
Expr Parser::parse_simplify(Evaluator& evaluator){
    // simplify(expr)
    this->consume(TokenKind::Simplify);
    this->consume(TokenKind::LParen);
    auto expr = this->parse_expr();
    this->consume(TokenKind::RParen);

    return Evaluator::simplify(evaluator, expr);
}

// -*-
Expr Parser::parse_diff(Evaluator& evaluator){
    // diff(expr, var);
    this->consume(TokenKind::Diff);
    this->consume(TokenKind::LParen);
    auto expr = this->parse_expr();
    this->consume(TokenKind::Comma);
    this->expect(TokenKind::Ident, "Expected variable in derivative");
    auto var = this->m_current.text;
    this->consume(TokenKind::Ident);
    this->consume(TokenKind::RParen);

    return Evaluator::diff(evaluator, expr, var);
}

// -*-
Expr Parser::parse_expand(Evaluator& evaluator){
    // expand(expr)
    this->consume(TokenKind::Expand);
    this->consume(TokenKind::LParen);
    auto expr = this->parse_expr();
    this->consume(TokenKind::RParen);

    return Evaluator::expand(evaluator, expr);
}

// -*-
Expr Parser::parse_factor(Evaluator& evaluator){
    // factor(expr)
    this->consume(TokenKind::Factor);
    this->consume(TokenKind::LParen);
    auto expr = this->parse_expr();
    this->consume(TokenKind::RParen);

    return Evaluator::factor(evaluator, expr);
}

// -*-
Expr Parser::parse_integral(Evaluator& evaluator){
    // integral(expr, var)
    this->consume(TokenKind::Integral);
    this->consume(TokenKind::LParen);
    auto expr = this->parse_expr();
    this->consume(TokenKind::Comma);
    this->expect(TokenKind::Ident, "Expected variable in integral");
    auto var = this->m_current.text;
    this->consume(TokenKind::Ident);
    this->consume(TokenKind::RParen);
    
    return Evaluator::integral(evaluator, expr, var);
}

// -*-
Expr Parser::parse_integrate(Evaluator& evaluator){
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

    return Evaluator::integrate(evaluator, expr, var, vmin, vmax);
}

// -*-
Expr Parser::parse_taylor(Evaluator& evaluator){
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

    return Evaluator::taylor(evaluator, expr, var, center, order);
}

// -*-
Expr Parser::parse_limit(Evaluator& evaluator){
    //! @todo
    // limit(expr, center)
    // limit(expr, center, :left)
    // limit(expr, center, :right)
    throw CreolaError("`limit()`: not implemented yet.");
}

// -*-
Expr Parser::parse_roots(Evaluator& evaluator){
    //! @todo
    // roots(expr, var)
    // roots(expr, var, tolerance)
    throw CreolaError("`roots()`: not implemented yet.");
}

// -*-
Expr Parser::parse_solve(Evaluator& evaluator){
    //! @todo
    // solve(equation)
    // solve(equation, tolerance)
    // solve(system)
    // solve(system, tolerance)
    throw CreolaError("`solve()`: not implemented yet.");
}

// -*-
Expr Parser::parse_fibonacci(Evaluator& evaluator){
    //! @todo
    // fibonacci(n)
    throw CreolaError("`fibonacci()`: not implemented yet.");
}

// -*-
Expr Parser::parse_factorial(Evaluator& evaluator){
    //! @todo
    // factorial(n)
    throw CreolaError("`factorial()`: not implemented yet.");
}

// -*-
Expr Parser::parse_modulo(Evaluator& evaluator){
    //! @todo
    // module(x, y)
    throw CreolaError("`modulo()`: not implemented yet.");
}

// -*-
Expr Parser::parse_prime(Evaluator& evaluator){
    //! @todo
    // prime(n)
    throw CreolaError("`prime()`: not implemented yet.");
}

// -*-
Expr Parser::parse_gcd(Evaluator& evaluator){
    //! @todo
    // gcd(x, y)
    throw CreolaError("`gcd()`: not implemented yet.");
}

// -*-
Expr Parser::parse_lcm(Evaluator& evaluator){
    //! @todo
    // lcm(x, y)
    throw CreolaError("`lcm()`: not implemented yet.");
}

// -*-
Expr Parser::parse_help(Evaluator& evaluator){
    //! @todo
    // help(ident)
    throw CreolaError("`help()`: not implemented yet.");
}

// -*-
Expr Parser::parse_show(Evaluator& evaluator){
    //! @todo
    /*
        show("variables")
        show("functions")
        show("env")
        show("builtin.constants")
        show("builtin.functions")
    */
    throw CreolaError("`show()`: not implemented yet.");
}

// -*-
Expr Parser::parse_plot(Evaluator& evaluator){
    //! @todo
    /*
        config("plot.xmin", -1)
        config("plot.xmax", 1.0)
        config("plot.ymin", -1)
        config("plot.ymax", 1.0)
        config("plot.color", ...)
        plot(expr)
        plot(expr, ...)
    */
    throw CreolaError("`plot()`: not implemented yet.");
}

// -*-
Expr Parser::parse_config(Evaluator& evaluator){
    //! @todo
    /*
        - format.long
        - tolerance
        - ps1
        - ps2
        - diff.[...]
        - roots.[...]
        - solve.[...]
        - limit.[...]
        - integrate.[...] 
        - plot.xmin
        - plot.xmax
        - plot.ymin
        - plot.ymax
        - plot.linewidth
        - plot.linecolor
        - plot.font.size
        - plot.font.family
        - plot.font.weight
        - plot.font.bold
        - plot.xlabel.[color, font, ]
        - plot.ylabel.[color, font, ]
        - plot.title.[color, font, ]
        - plot.gird.[minor, major]
        - plot.legend.[boxon, location]
        -
        config("plot.xmin", -1)
    */
    throw CreolaError("`config()`: not implemented yet.");
}

// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola::core                                 -*-
// -*----------------------------------------------------------------*-