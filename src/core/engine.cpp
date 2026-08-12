#include "creola/core/engine.hpp"
#include "creola/core/parser.hpp"
#include<stdexcept>

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola::core                               -*-
// -*----------------------------------------------------------------*-
namespace creola::core{
//
std::mutex Creola::app_mtx;
Shared<Creola> Creola::app = std::make_shared<Creola>();
// -
std::unordered_map<std::string, UnaryMathFun> Creola::UNARY_MATH_FUNCTIONS = {
    {"sin", Creola::sin},
    {"cos", Creola::cos},
    {"tan", Creola::tan},
    {"asin", Creola::asin},
    {"acos", Creola::acos},
    {"atan", Creola::atan},

    {"sinh", Creola::sinh},
    {"cosh", Creola::cosh},
    {"tanh", Creola::tanh},
    {"asinh", Creola::asinh},
    {"acosh", Creola::acosh},
    {"atanh", Creola::atanh},

    {"sqrt", Creola::sqrt},
    {"cbrt", Creola::cbrt},
    {"exp", Creola::exp},
    {"ln", Creola::ln},
};

std::unordered_map<std::string, Func> Creola::COMMON_DIFF_TABLE = {
    {"sin", Creola::diff_sin},
    {"cos", Creola::diff_cos},
    {"tan", Creola::diff_tan},
    {"asin", Creola::diff_asin},
    {"acos", Creola::diff_acos},
    {"atan", Creola::diff_atan},

    {"sinh", Creola::diff_sinh},
    {"cosh", Creola::diff_cosh},
    {"tanh", Creola::diff_tanh},
    {"asinh", Creola::diff_asinh},
    {"acosh", Creola::diff_acosh},
    {"atanh", Creola::diff_atanh},

    {"sqrt", Creola::diff_sqrt},
    {"cbrt", Creola::diff_cbrt},
    {"exp", Creola::diff_exp},
    {"ln", Creola::diff_ln},
};

std::unordered_map<std::string, Func> Creola::COMMON_INTEGRATION_TABLE = {
    {"sin", Creola::integrate_sin},
    {"cos", Creola::integrate_cos},
    {"tan", Creola::integrate_tan},
    {"asin", Creola::integrate_asin},
    {"acos", Creola::integrate_acos},
    {"atan", Creola::integrate_atan},

    {"sinh", Creola::integrate_sinh},
    {"cosh", Creola::integrate_cosh},
    {"tanh", Creola::integrate_tanh},
    {"asinh", Creola::integrate_asinh},
    {"acosh", Creola::integrate_acosh},
    {"atanh", Creola::integrate_atanh},

    {"sqrt", Creola::integrate_sqrt},
    {"cbrt", Creola::integrate_cbrt},
    {"exp", Creola::integrate_exp},
    {"ln", Creola::integrate_ln},
};

// -*-
Expr Creola::number(f64 val){
    return std::make_shared<Number>(val);
}

// -*-
Expr Creola::symbol(const std::string& var){
    return std::make_shared<Symbol>(var);
}

// -*-
bool Creola::is_zero(const Expr& expr){
    Number num{0.0};
    Creola::as(expr, num);
    return num.value()==0.0;

    // auto num = std::dynamic_pointer_cast<Number>(expr);
    // return (num && std::fabsl(num->value()==0.0L));
}

// -*-
bool Creola::is_one(const Expr& expr){
    Number num{0.0};
    Creola::as(expr, num);
    return num.value()==1.0;
}

// -*-
bool Creola::is_number_expr(const Expr& expr){
    return expr->kind()==ExprKind::NUM;
}

// -*-
bool Creola::is_symbol_expr(const Expr& expr){
    return expr->kind()==ExprKind::SYM;
}

// -*-
bool Creola::is_neg_expr(const Expr& expr){
    return expr->kind()==ExprKind::NEG;
}

// -*-
bool Creola::is_add_expr(const Expr& expr){
    return expr->kind()==ExprKind::ADD;
}

// -*-
bool Creola::is_mul_expr(const Expr& expr){
    return expr->kind()==ExprKind::MUL;
}

// -*-
bool Creola::is_pow_expr(const Expr& expr){
    return expr->kind()==ExprKind::POW;
}

// -*-
bool Creola::is_call_expr(const Expr& expr){
    return expr->kind()==ExprKind::CALL;
}

// - handle_line()
void Creola::run(const std::string& src){
    if(src.empty()){ return; }
    Parser parser(src);
    if(parser.match(TokenKind::KwLet)){
        // let name = expr
        parser.consume(TokenKind::KwLet);
        if(!parser.match(TokenKind::Ident)){
            throw std::runtime_error("Expected identifier after `let'");
        }
        auto name = parser.current().text;
        parser.consume(TokenKind::Ident);
        if(!parser.match(TokenKind::Equal)){
            throw std::runtime_error("Expected `=' in let statement");
        }
        parser.consume(TokenKind::Equal);
        auto expr = parser.parse()->simplify();
        this->m_vars[name] = expr;
        Creola::println(std::cout, "Defined '", name, "' = ", expr);
        return;
    }
    if(parser.match(TokenKind::KwFun)){
        // fun f(x) = expr
        parser.consume(TokenKind::KwFun);
        if(!parser.match(TokenKind::Ident)){
            throw std::runtime_error("Expected function name.");
        }
        auto fname = parser.current().text;
        parser.consume(TokenKind::Ident);
        if(!parser.match(TokenKind::LParen)){
            throw std::runtime_error("Expected '(' after function name in `fun` statement.");
        }
        parser.consume(TokenKind::LParen);
        if(!parser.match(TokenKind::Ident)){
            throw std::runtime_error("Expected parameter name");
        }
        auto param = parser.current().text;
        parser.consume(TokenKind::Ident);
        if(!parser.match(TokenKind::RParen)){
            throw std::runtime_error("Expected ')' to close function parameter list.");
        }
        parser.consume(TokenKind::RParen);
        if(!parser.match(TokenKind::Equal)){
            throw std::runtime_error("Expected `=` in function definition");
        }
        parser.consume(TokenKind::Equal);
        auto body = parser.parse()->simplify();
        this->m_funcs[fname] = {param, body};
        Creola::println(std::cout, "Defined fun: '", fname, "(", param, ") = ", body);
        return;
    }
    // command like `simplify(...)`, `diff(...)`, `taylor(...)`, ...
    if(parser.match(TokenKind::Ident)){
        // `cmd'(...)
        auto cmd = parser.current().text;
        parser.consume(TokenKind::Ident);
        if(parser.match(TokenKind::LParen)){
            parser.consume(TokenKind::LParen);
            if(cmd == "simplify"){
                auto expr = parser.parse();
                parser.expect(TokenKind::RParen, "simplify: missing ')'");
                expr = Creola::simplify(expr);
                Creola::println(std::cout, "===> ", expr);
                return;
            }
            if(cmd == "expand"){
                auto expr = Creola::expand(parser.parse());
                parser.expect(TokenKind::RParen, "expand: missing ')'");
                Creola::println(std::cout, "===> ", expr);
                return;
            }
            if(cmd == "diff"){
                // diff(expr, var)
                auto expr = parser.parse();
                parser.expect(TokenKind::Comma, "diff: missing ','");
                if(!parser.match(TokenKind::Ident)){
                    throw std::runtime_error("diff: expected variable");
                }
                auto var = parser.current().text;
                parser.consume(TokenKind::Ident);
                parser.expect(TokenKind::RParen, "diff: missing ')'");
                auto result = Creola::diff(expr, var);
                Creola::println(std::cout, "===> ", result);
                return;
            }
            if(cmd == "integrate"){
                // integrate(expr, var)
                auto expr = parser.parse();
                parser.expect(TokenKind::Comma, "integrate: missing ','");
                if(!parser.match(TokenKind::Ident)){
                    throw std::runtime_error("integrate: expected variable.");
                }
                auto var = parser.current().text;
                parser.consume(TokenKind::Ident);
                parser.expect(TokenKind::RParen, "integrate: missing ')'");
                auto result = Creola::integrate(expr, var);
                Creola::println(std::cout, "===> ", result);
                return;
            }
            if(cmd == "taylor"){
                // taylor(expr, var, center, order)
                auto expr = parser.parse();
                parser.expect(TokenKind::Comma, "taylor: missing ','");
                if(!parser.match(TokenKind::Ident)){
                    throw std::runtime_error("taylor: expeted a variable");
                }
                auto var = parser.current().text;
                parser.consume(TokenKind::Ident);

                if(!parser.match(TokenKind::Number)){
                    throw std::runtime_error("taylor: expected a number (i.e the center)");
                }
                auto center = parser.current().num;
                parser.consume(TokenKind::Number);
                parser.expect(TokenKind::Comma, "taylor: missing ','");
                if(!parser.match(TokenKind::Number)){
                    throw std::runtime_error("taylor: expected a number (i.e the order)");
                }
                auto order = static_cast<int>(parser.current().num);
                parser.consume(TokenKind::Number);
                parser.expect(TokenKind::RParen, "taylor: missing ')'");
                auto result = Creola::taylor(expr, var, center, order);
                Creola::println(std::cout, "===> ", result);
                return;
            }
            if(cmd == "limit"){
                // limit(expr, var, val)
                auto expr = parser.parse();
                parser.expect(TokenKind::Comma, "limit: missing ','");
                if(!parser.match(TokenKind::Ident)){
                    throw std::runtime_error("limit: expected variable");
                }
                auto var = parser.current().text;
                parser.consume(TokenKind::Ident);
                parser.expect(TokenKind::Comma, "limit: missing ','");
                if(!parser.match(TokenKind::Number)){
                    throw std::runtime_error("limit: expected a number (i.e the point where the limit is evaluated.)");
                }
                auto val = parser.current().num;
                parser.consume(TokenKind::Number);
                parser.expect(TokenKind::RParen, "limit: missing ')'");
                auto result = Creola::limit(expr, var, val);
                Creola::println(std::cout, "===> ", result);
                return;
            }
            if(cmd == "roots"){
                // roots(expr, var, [vmin], [vmax])
                auto expr = parser.parse();
                parser.expect(TokenKind::Comma, "roots: missing ','");
                if(!parser.match(TokenKind::Ident)){
                    throw std::runtime_error("roots: expected variable");
                }
                auto var = parser.current().text;
                parser.consume(TokenKind::Ident);
                parser.expect(TokenKind::RParen, "roots: missing ')'");
                auto result = Creola::roots(expr, var);
                //std::cout << "===> { ";
                Creola::print(std::cout, "===> { ");
                for(size_t i=0; i < result.size(); ++i){
                    if(i > 0){ std::cout << ", "; }
                    Creola::print(std::cout, result[i]);
                }
                Creola::println(std::cout, "}");
                return;
            }
            //! @todo implement `factor' command
        }
    }

    // default: just parse without evaluating the expression
    auto expr = parser.parse()->simplify();
    expr->print(*Creola::app, std::cout, 0);
    std::cout << std::endl;
}


// - parse_expression_only()
Expr Creola::parse(const std::string& src){
    Parser parser(src);
    return parser.parse()->simplify();
}


// -*-
Expr Creola::substitute(const Expr& expr, const std::string& var, const Expr& val){
    //if(auto _ = std::dynamic_point_cast<Number>(expr)){ return expr; }
    if(Creola::is_number_expr(expr)){ return expr; }
    if(auto sym = std::dynamic_pointer_cast<Symbol>(expr)){
        return sym->name() == var? val: expr;
    }

    if(auto ptr = std::dynamic_pointer_cast<Add>(expr)){
        Vec<Expr> terms{};
        for(auto& term: ptr->terms()){
            terms.push_back(this->substitute(term, var, val));
        }
        return std::make_shared<Add>(terms)->simplify();
    }
    if(auto ptr = std::dynamic_pointer_cast<Mul>(expr)){
        Vec<Expr> factors{};
        for(auto& factor: ptr->factors()){
            factors.push_back(this->substitute(factor, var, val));
        }
        return std::make_shared<Mul>(factors)->simplify();
    }
    if(auto ptr = std::dynamic_pointer_cast<Pow>(expr)){        
        return std::make_shared<Pow>(
            this->substitute(ptr->base(), var, val),
            this->substitute(ptr->expo(), var, val)
        )->simplify();
    }
    if(auto ptr = std::dynamic_pointer_cast<FuncCall>(expr)){  
        Vec<Expr> argv{};
        for(auto& expr: ptr->args()){
            argv.push_back(this->substitute(expr, var, val));
        }  
        return std::make_shared<FuncCall>(ptr->name(), argv)->simplify();
    }
    if(auto ptr = std::dynamic_pointer_cast<Neg>(expr)){
        return std::make_shared<Neg>(this->substitute(ptr->rhs(), var, val));
    }

    return expr;
}

// apply_user_func ==> apply 
Expr Creola::apply(const std::string& name, const Vec<Expr>& args){
    auto entry = this->m_funcs.find(name);
    if(entry == this->m_funcs.end()){ return nullptr; }
    if(args.size() != 1){ return nullptr; }
    return this->substitute(
        entry->second.body, entry->second.param, args[0]
    )->simplify();
}

// -*-
Expr Creola::simplify(const Expr& expr){
    return expr->simplify();
}

Expr Creola::diff(const Expr& expr, const std::string& var){
    return expr->diff(var);
}

Expr Creola::expand(const Expr& expr){
    return expr->expand();
}

// -*-
Expr Creola::factor(const Expr& expr, const std::string& var){
    return expr->factorize(var);
}

// -*-
Expr Creola::integrate(const Expr& expr, const std::string& var){
    return expr->integrate(var);
}

// -
f64 Creola::eval(const Expr& expr, const std::string& var, f64 val){
    return expr->eval(var, val);
}

// -
Vec<f64> Creola::roots(const Expr& expr, const std::string& var, f64 vmin, f64 vmax, int samples){
    constexpr int MAX_ITERATION = 60;
    Vec<f64> ans{};
    f64 step = (vmax - vmin)/samples;
    f64 x0 = vmin;
    f64 f0 = expr->eval(var, x0);
    for(int i=1; i <= samples; i++){
        auto x1 = vmin + i + step;
        auto f1 = expr->eval(var, x1);
        if(f0 == 0.0){ ans.push_back(x0); }
        if(f0 * f1 < 0){
            // bisection method
            auto lo = x0;
            auto hi = x1;
            for(int j=0; j < MAX_ITERATION; j++){
                auto mid = 0.5 * (lo - hi);
                auto fmid = expr->eval(var, mid);
                if(f0*fmid <= 0){
                    hi = mid;
                    f1 = fmid;
                }else{
                    lo = mid;
                    f0 = fmid;
                }
            }
            ans.push_back(0.5*(lo+hi));
        }
        x0 = x1;
        f0 = f1;
    }
    return ans;
}

// -*-
Expr Creola::taylor(const Expr& expr, const std::string& var, f64 val, int n){
    auto series = Creola::number(0);
    auto current = expr;
    for(int k=0; k <= n; ++k){
        auto coeff = current->eval(var, val) / std::tgamma(k+1.0); // k!
        Expr term{};
        if(k==0){ term =Creola::number(coeff); }
        else{
            term = std::make_shared<Mul>(Vec<Expr>{
                Creola::number(coeff),
                std::make_shared<Pow>(
                    std::make_shared<Add>(Vec<Expr>{
                        Creola::symbol(var), Creola::number(-val)
                    }),
                    Creola::number(k)
                )
            });
        }
        series = std::make_shared<Add>(Vec<Expr>{series, term})->simplify();
        current = current->diff(var)->simplify();
    }

    return series->simplify();
}

// -
f64 Creola::limit(const Expr& expr, const std::string& var, f64 val, f64 eps){
    // very basic: we'll approximate limits numerically from left/right
    auto left = expr->eval(var, val-eps);
    auto right = expr->eval(var, val+eps);
    return 0.5*(left + right);
}

// -----------------------------------------------------
// -*- INTERNAL DIFF & INTEGRATE OF COMMON FUNCTIONS -*-
// -----------------------------------------------------
// -*---------------------------*-
// -*- Trigonometric functions -*-
// -*---------------------------*-
// -*- Sin, Cos, Tan, Asin, Acos, Atan -*-
Expr Creola::diff_sin(Expr expr, const std::string& var){
    auto dx = expr->diff(var);
    return std::make_shared<Mul>(Vec<Expr>{
        std::make_shared<FuncCall>("cos", Vec<Expr>{expr}),
        dx
    })->simplify();
}

// ∫ sin(x) dx = -cos(x)
Expr Creola::integrate_sin(Expr expr, const std::string& var){
    auto sym = std::dynamic_pointer_cast<Symbol>(expr);
    if(sym && sym->name() == var){
        return std::make_shared<Neg>(
            std::make_shared<FuncCall>("cos", Vec<Expr>{Creola::symbol(var)})
        )->simplify();
    }
    // sin(expr) dvar
    return std::make_shared<FuncCall>("∫_" + var, Vec<Expr>{expr});
}

// -*-
Expr Creola::diff_cos(Expr expr, const std::string& var){
    auto dx = expr->diff(var);
    return std::make_shared<Mul>(Vec<Expr>{
        Creola::number(-1),
        std::make_shared<FuncCall>("sin", Vec<Expr>{expr}),
        dx
    })->simplify();
}

Expr Creola::integrate_cos(Expr expr, const std::string& var){
    auto sym = std::dynamic_pointer_cast<Symbol>(expr);
    if(sym && sym->name() == var){
        return std::make_shared<FuncCall>("sin", Vec<Expr>{expr})->simplify();
    }
    // cos(expr) dvar
    return std::make_shared<FuncCall>("∫_" + var, Vec<Expr>{expr});
}

Expr Creola::diff_tan(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}
Expr Creola::integrate_tan(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}

Expr Creola::diff_asin(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}
Expr Creola::integrate_asin(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}

Expr Creola::diff_acos(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}
Expr Creola::integrate_acos(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}

Expr Creola::diff_atan(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}
Expr Creola::integrate_atan(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}

// -*-----------------------------------------*-
// -*- Hyperbolic functions                  -*-
// -*- Sinh, Cosh, Tanh, Asinh, Acosh, Atanh -*-
// -*-----------------------------------------*-
Expr Creola::diff_sinh(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}
Expr Creola::integrate_sinh(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}
Expr Creola::diff_cosh(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}
Expr Creola::integrate_cosh(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}
Expr Creola::diff_tanh(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}
Expr Creola::integrate_tanh(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}
Expr Creola::diff_asinh(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}
Expr Creola::integrate_asinh(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}
Expr Creola::diff_acosh(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}
Expr Creola::integrate_acosh(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}
Expr Creola::diff_atanh(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}
Expr Creola::integrate_atanh(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}

// -*------------------------*-
// -*- Power functions      -*-
// -*- Pow, Sqrt, Cbrt, Exp -*-
// -*------------------------*-
Expr Creola::diff_pow(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}
Expr Creola::integrate_pow(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}
Expr Creola::diff_sqrt(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}
Expr Creola::integrate_sqrt(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}
Expr Creola::diff_cbrt(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}
Expr Creola::integrate_cbrt(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}
Expr Creola::diff_exp(Expr expr, const std::string& var){
    auto dx = expr->diff(var);
    return std::make_shared<Mul>(Vec<Expr>{
        std::make_shared<FuncCall>("exp", Vec<Expr>{expr}),
        dx
    })->simplify();
}

Expr Creola::integrate_exp(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}

Expr Creola::diff_ln(Expr expr, const std::string& var){
    auto dx = expr->diff(var);
    return std::make_shared<Mul>(Vec<Expr>{
        std::make_shared<Pow>(expr, Creola::number(-1)),
        dx
    })->simplify();
}


Expr Creola::integrate_ln(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}

// -*-
void Creola::visit(std::ostream& os, const Number& expr, int prec) const{
    Creola::display(os, expr, prec);
}

// -*-
void Creola::visit(std::ostream& os, const Symbol& expr, int prec) const{
    CREOLA_UNUSED(prec);
    Creola::display(os, expr, prec);
}

// -*-
void Creola::visit(std::ostream& os, const Neg& expr, int prec) const{
    Creola::display(os, expr, prec);
}

// -*-
void Creola::visit(std::ostream& os, const Add& expr, int prec) const{
    Creola::display(os, expr, prec);
}

// -*-
void Creola::visit(std::ostream& os, const Mul& expr, int prec) const{
    Creola::display(os, expr, prec);
}

// -*-
void Creola::visit(std::ostream& os, const Pow& expr, int prec) const{
    Creola::display(os, expr, prec);
}

// -*-
void Creola::visit(std::ostream& os, const FuncCall& expr, int prec) const{
    Creola::display(os, expr, prec);
}

// -*-
void Creola::display(std::ostream& os, const Number& expr, [[maybe_unused]] int prec){
    std::lock_guard<std::mutex> lock(Creola::app_mtx);
    os << expr;
}

// -*-
void Creola::display(std::ostream& os, const Symbol& expr, [[maybe_unused]] int prec){
    std::lock_guard<std::mutex> lock(Creola::app_mtx);
    os << expr;
}

// -*-
void Creola::display(std::ostream& os, const Add& expr, [[maybe_unused]] int prec){
    std::lock_guard<std::mutex> lock(Creola::app_mtx);
    os << expr;
}

// -*-
void Creola::display(std::ostream& os, const Mul& expr, [[maybe_unused]] int prec){
    std::lock_guard<std::mutex> lock(Creola::app_mtx);
    os << expr;
}

// -*-
void Creola::display(std::ostream& os, const Pow& expr, [[maybe_unused]] int prec){
    std::lock_guard<std::mutex> lock(Creola::app_mtx);
    os << expr;
}

// -*-
void Creola::display(std::ostream& os, const Neg& expr, [[maybe_unused]] int prec){
    std::lock_guard<std::mutex> lock(Creola::app_mtx);
    os << expr;
}

// -*-
void Creola::display(std::ostream& os, const FuncCall& expr, [[maybe_unused]] int prec){
    std::lock_guard<std::mutex> lock(Creola::app_mtx);
    os << expr;
}


// -*-
void Creola::display(std::ostream& os, const Expr& expr, [[maybe_unused]] int prec){
    if(auto num = std::dynamic_pointer_cast<Number>(expr)){
        Creola::display(os, *num, prec);
    }
    if(auto sym = std::dynamic_pointer_cast<Symbol>(expr)){
        Creola::display(os, *sym, prec);
    }
    if(auto add = std::dynamic_pointer_cast<Add>(expr)){
        Creola::display(os, *add, prec);
    }
    if(auto mul = std::dynamic_pointer_cast<Mul>(expr)){
        Creola::display(os, *mul, prec);
    }
    if(auto pow = std::dynamic_pointer_cast<Pow>(expr)){
        Creola::display(os, *pow, prec);
    }
    if(auto neg = std::dynamic_pointer_cast<Neg>(expr)){
        Creola::display(os, *neg, prec);
    }
    if(auto fcall = std::dynamic_pointer_cast<FuncCall>(expr)){
        Creola::display(os, *fcall, prec);
    }
}

// -*-
void Creola::as(Expr expr, Number& num){
    if(Creola::is_number_expr(expr)){
        num = *dynamic_cast<Number*>(expr.get());
    }
    throw std::runtime_error("TypeError: expected number expression");
}

// -*-
void Creola::as(Expr expr, Symbol& sym){
    if(Creola::is_symbol_expr(expr)){
        sym = *dynamic_cast<Symbol*>(expr.get());
    }
    throw std::runtime_error("TypeError: expected symbol expression");
}

// -*-
void Creola::as(Expr expr, Neg& neg){
    if(Creola::is_number_expr(expr)){
        neg = *dynamic_cast<Neg*>(expr.get());
    }
    throw std::runtime_error("TypeError: expected negate expression");
}

// -*-
void Creola::as(Expr expr, Add& add){
    if(Creola::is_number_expr(expr)){
        add = *dynamic_cast<Add*>(expr.get());
    }
    throw std::runtime_error("TypeError: expected add expression");
}

// -*-
void Creola::as(Expr expr, Mul& mul){
    if(Creola::is_number_expr(expr)){
        mul = *dynamic_cast<Mul*>(expr.get());
    }
    throw std::runtime_error("TypeError: expected mul expression.");
}

// -*-
void Creola::as(Expr expr, Pow& pow){
    if(Creola::is_number_expr(expr)){
        pow = *dynamic_cast<Pow*>(expr.get());
    }
    throw std::runtime_error("TypeError: expected power expression.");
}

// -*-
void Creola::as(Expr expr, FuncCall& fcall){
    if(Creola::is_number_expr(expr)){
        fcall = *dynamic_cast<FuncCall*>(expr.get());
    }
    throw std::runtime_error("TypeError: expected function-call expression");
}

// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola::core                                 -*-
// -*----------------------------------------------------------------*-