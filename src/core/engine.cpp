#include "creola/core/engine.hpp"
#include "creola/core/parser.hpp"

#include<stdexcept>
#include<sstream>
#include<limits>
#include<cmath>
#include<set>

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

std::string Creola::ps1 = "creola> ";
std::string Creola::ps2 = " =====> ";
std::set<std::string> Creola::keywords;
std::set<std::string> Creola::builtin_commands;

// -
void Creola::define_keywords(void){
    Creola::keywords = {
        "let", "fun",
    };
}

void Creola::define_builtin_commands(void){
    Creola::builtin_commands = {
        "simplify", "diff", "integrate", "taylor",
        "expand", "factor", "limit", "greobner",
        "rewrite", "roots", "solve", "solve_system",
        "equation", "system", "factorial", "fibonacci",
        // utility functions
        "help", "print", "config", "show", "",
    };
}

// -*-
Expr Creola::make_number_expr(f64 val){
    return std::make_shared<Number>(val);
}

// -*-
Expr Creola::make_symbol_expr(const std::string& var){
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
Expr Creola::make_neg_expr(const Expr& expr){
    return std::make_shared<Neg>(expr);
}

// -*-
Expr Creola::make_add_expr(const Vec<Expr>& terms){
    return std::make_shared<Add>(terms);
}

// -*-
Expr Creola::make_mul_expr(const Vec<Expr>& factors){
    return std::make_shared<Mul>(factors);
}

// -*-
Expr Creola::make_pow_expr(const Expr& base, const Expr& expo){
    return std::make_shared<Pow>(base, expo);
}

// -*-
Expr Creola::make_funcall_expr(const std::string& name, const Vec<Expr>& argv){
    return std::make_shared<FuncCall>(name, argv);
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

// // -
// Expr Creola::handle_expr(const Expr& expr){
//     if(Creola::is_symbol_expr(expr)){// we have a symbol that represent either a variable or a function name.
//         Symbol sym{""};
//         Creola::as(expr, sym);
//         return this->get_var(sym.name());
//     }

//     return expr;
// }

// - handle_line()
Result Creola::run(const std::string& src){
    if(src.empty()){ return Result::ok(""); }

    auto startsWithCmd = [](const std::string& data){
        for(auto cmd: Creola::builtin_commands){
            if(creola::core::starts_with(data, cmd)){ return true; }
        }
        return false;
    };

    auto input = creola::core::trim(src);
    if(creola::core::starts_with(input, "let") || creola::core::starts_with(input, "fun")){
        std::cerr << "Processing 'let' or 'fun' statements" << std::endl;
        std::ostringstream oss;
        try{
            auto ans = this->process_keyword(input);
            return Result::ok(ans);
        }catch(const CreolaError& err){
            Creola::print(oss, err.what());
            return Result::err(oss.str());
        }catch(...){
            Creola::print(oss, "unexpected error found.");
            return Result::err(oss.str());
        }
    }else if(startsWithCmd(input)){
        std::ostringstream oss;
        try{
            auto ans = this->process_command(input);
            return Result::ok(ans);
        }catch(const CreolaError& err){
            Creola::print(oss, err.what());
            return Result::err(oss.str());
        }catch(...){
            Creola::print(oss, "unexpected error found.");
            return Result::err(oss.str());
        }
    }else{
        //! @todo: evaluate a symbol and yield and an error if not defined
        // Just echo the src as is if it is not a symbol.
        std::ostringstream oss;
        Creola::print(oss, src);
        return Result::ok(oss.str());
    }
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
        return sym->name() == var ? val: expr;
    }

    if(auto ptr = std::dynamic_pointer_cast<Add>(expr)){
        Vec<Expr> terms{};
        for(auto& term: ptr->terms()){
            terms.push_back(this->substitute(term, var, val));
        }
        return Creola::make_add_expr(terms)->simplify();
    }
    if(auto ptr = std::dynamic_pointer_cast<Mul>(expr)){
        Vec<Expr> factors{};
        for(auto& factor: ptr->factors()){
            factors.push_back(this->substitute(factor, var, val));
        }
        return Creola::make_mul_expr(factors)->simplify();
    }
    if(auto ptr = std::dynamic_pointer_cast<Pow>(expr)){        
        return Creola::make_pow_expr(
            this->substitute(ptr->base(), var, val),
            this->substitute(ptr->expo(), var, val)
        )->simplify();
    }
    if(auto ptr = std::dynamic_pointer_cast<FuncCall>(expr)){  
        Vec<Expr> argv{};
        for(auto& expr: ptr->args()){
            argv.push_back(this->substitute(expr, var, val));
        }  
        return Creola::make_funcall_expr(ptr->name(), argv)->simplify();
    }
    if(auto ptr = std::dynamic_pointer_cast<Neg>(expr)){
        return Creola::make_neg_expr(this->substitute(ptr->rhs(), var, val));
    }

    return expr;
}

// apply_user_func ==> apply 
Expr Creola::apply(const std::string& name, const Vec<Expr>& args){
    auto entry = this->m_funcs.find(name);
    if(entry == this->m_funcs.end()){
        std::ostringstream ess;
        ess << "undefined function with the name " << std::quoted(name);
        throw CreolaError(ess.str());
    }
    if(args.size() != entry->second.params.size()){
        std::ostringstream ess;
        ess << "incompatible number of argument passed to function " << std::quoted(name);
        throw CreolaError(ess.str());
    }
    for(usize i=0; i < args.size(); ++i){
        auto ans = this->substitute(entry->second.body, entry->second.params[i], args[i])->simplify();
        entry->second.body = ans;
    }
    return entry->second.body->simplify();
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
Expr Creola::taylor(const Expr& expr, const std::string& var, f64 val, u32 n){
    auto series = Creola::make_number_expr(0.0);
    auto current = expr;
    for(int k=0; k <= n; ++k){
        auto coeff = current->eval(var, val) / std::tgamma(k+1.0); // k!
        Expr term{};
        if(k==0){ term = Creola::make_number_expr(coeff); }
        else{
            term = Creola::make_mul_expr(Vec<Expr>{
                Creola::make_number_expr(coeff),
                Creola::make_pow_expr(
                    Creola::make_add_expr(Vec<Expr>{
                        Creola::make_symbol_expr(var), Creola::make_number_expr(-val)
                    }),
                    Creola::make_number_expr(k)
                )
            });
        }
        series = Creola::make_add_expr(Vec<Expr>{series, term})->simplify();
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
    return Creola::make_mul_expr(Vec<Expr>{
        Creola::make_funcall_expr("cos", Vec<Expr>{expr}),
        dx
    })->simplify();
}

// ∫ sin(x) dx = -cos(x)
Expr Creola::integrate_sin(Expr expr, const std::string& var){
    auto sym = std::dynamic_pointer_cast<Symbol>(expr);
    if(sym && sym->name() == var){
        return Creola::make_neg_expr(
            Creola::make_funcall_expr("cos", Vec<Expr>{Creola::make_symbol_expr(var)})
        )->simplify();
    }
    // sin(expr) dvar
    return Creola::make_funcall_expr("∫_" + var, Vec<Expr>{expr});
}

// -*-
Expr Creola::diff_cos(Expr expr, const std::string& var){
    auto dx = expr->diff(var);
    return Creola::make_mul_expr(Vec<Expr>{
        Creola::make_number_expr(-1),
        Creola::make_funcall_expr("sin", Vec<Expr>{expr}),
        dx
    })->simplify();
}

Expr Creola::integrate_cos(Expr expr, const std::string& var){
    auto sym = std::dynamic_pointer_cast<Symbol>(expr);
    if(sym && sym->name() == var){
        return Creola::make_funcall_expr("sin", Vec<Expr>{expr})->simplify();
    }
    // cos(expr) dvar
    return Creola::make_funcall_expr("∫_" + var, Vec<Expr>{expr});
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
    return Creola::make_mul_expr(Vec<Expr>{
        Creola::make_funcall_expr("exp", Vec<Expr>{expr}),
        dx
    })->simplify();
}

Expr Creola::integrate_exp(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}

Expr Creola::diff_ln(Expr expr, const std::string& var){
    auto dx = expr->diff(var);
    return Creola::make_mul_expr(Vec<Expr>{
        Creola::make_pow_expr(expr, Creola::make_number_expr(-1)),
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
    throw CreolaError("TypeError: expected number expression");
}

// -*-
void Creola::as(Expr expr, Symbol& sym){
    if(Creola::is_symbol_expr(expr)){
        sym = *dynamic_cast<Symbol*>(expr.get());
    }
    throw CreolaError("TypeError: expected symbol expression");
}

// -*-
void Creola::as(Expr expr, Neg& neg){
    if(Creola::is_number_expr(expr)){
        neg = *dynamic_cast<Neg*>(expr.get());
    }
    throw CreolaError("TypeError: expected negate expression");
}

// -*-
void Creola::as(Expr expr, Add& add){
    if(Creola::is_number_expr(expr)){
        add = *dynamic_cast<Add*>(expr.get());
    }
    throw CreolaError("TypeError: expected add expression");
}

// -*-
void Creola::as(Expr expr, Mul& mul){
    if(Creola::is_number_expr(expr)){
        mul = *dynamic_cast<Mul*>(expr.get());
    }
    throw CreolaError("TypeError: expected mul expression.");
}

// -*-
void Creola::as(Expr expr, Pow& pow){
    if(Creola::is_number_expr(expr)){
        pow = *dynamic_cast<Pow*>(expr.get());
    }
    throw CreolaError("TypeError: expected power expression.");
}

// -*-
void Creola::as(Expr expr, FuncCall& fcall){
    if(Creola::is_number_expr(expr)){
        fcall = *dynamic_cast<FuncCall*>(expr.get());
    }
    throw CreolaError("TypeError: expected function-call expression");
}

// ------------------------------------------------------------------
// -*- IMPLEMENTATION OF HELPER METHODS                           -*-
// ------------------------------------------------------------------
std::string Creola::trim_command(const std::string& src, const char* cmd){
    auto prefix = std::string(cmd);
    return creola::core::ltrim(src);
}

// -
std::string Creola::process_command(const std::string& src){
    const auto& _commands = Creola::builtin_commands;
    auto commands = Vec<std::string>(_commands.cbegin(), _commands.cend());


    std::string result{};
    for(auto& cmd: commands){
        if(cmd=="simplify"){
            result = this->handle_command_simplify(src);
            break;
        }
        if(cmd=="diff"){
            result = this->handle_command_diff(src);
            break;
        }
        if(cmd=="integrate"){
            result = this->handle_command_integrate(src);
            break;
        }
        if(cmd=="taylor"){
            result = this->handle_command_taylor(src);
            break;
        }
        if(cmd=="expand"){
            result = this->handle_command_expand(src);
            break;
        }
        if(cmd=="factor"){
            result = this->handle_command_factor(src);
            break;
        }
        if(cmd=="limit"){
            result = this->handle_command_limit(src);
            break;
        }
        if(cmd=="groebner"){
            result = this->handle_command_groebner(src);
            break;
        }
        if(cmd=="rewrite"){
            result = this->handle_command_rewrite(src);
            break;
        }
        if(cmd=="roots"){
            result = this->handle_command_roots(src);
            break;
        }
        if(cmd=="solve"){
            result = this->handle_command_solve(src);
            break;
        }
        if(cmd=="solve_system"){
            result = this->handle_command_solve_system(src);
            break;
        }
        if(cmd=="partfrac"){
            result = this->handle_command_partfrac(src);
            break;
        }
        if(cmd=="equation"){
            result = this->handle_command_equation(src);
            break;
        }
        if(cmd=="system"){
            result = this->handle_command_system(src);
            break;
        }
        if(cmd=="factorial"){
            result = this->handle_command_factorial(src);
            break;
        }
        if(cmd=="fibonacci"){
            result = this->handle_command_fibonacci(src);
            break;
        }
        if(cmd=="help"){
            result = this->handle_command_help(src);
            break;
        }
        if(cmd=="print"){
            result = this->handle_command_print(src);
            break;
        }
        if(cmd=="config"){
            result = this->handle_command_config(src);
            break;
        }
        if(cmd=="show"){
            result = this->handle_command_show(src);
            break;
        }
    }

    return result;
}

// -
std::string Creola::process_keyword(const std::string& src){
    if(creola::core::starts_with(src, "let")){
        return this->handle_keyword_let(src);
    }else{
        return this->handle_keyword_fun(src);
    }
}

// -
std::string Creola::handle_keyword_let(const std::string& src){
    std::cerr << "Creola::handle_keyword_let(): <src value=" << std::quoted(src) << ">" << std::endl;
    Parser parser(src);
    parser.expect(TokenKind::KwLet, "expected the keyword `let`.");
    parser.consume(TokenKind::KwLet);
    parser.expect(TokenKind::Ident, "expected and identifier");
    auto var = parser.current().text;
    parser.consume(TokenKind::Ident);
    std::cerr << "Creola::handle_keyword_let(): <token " << std::quoted(var) << ">" << std::endl;

    parser.expect(TokenKind::Equal, "expected `=' after variable name.");
    parser.consume(TokenKind::Equal);
    std::cerr << "Creola::handle_keyword_let(): <token " << std::quoted("=") << ">" << std::endl;

    auto expr = parser.parse()->simplify();
    this->m_vars[var] = expr;
    
    std::ostringstream oss;
    Creola::print(oss, var, "=", expr);
    auto myExpr = oss.str();
    std::cerr << "Creola::handle_keyword_let(): <expr " << std::quoted(myExpr) << ">" << std::endl;
    return myExpr;
}

// -
std::string Creola::handle_keyword_fun(const std::string& src){
    std::cerr << "Creola::handle_keyword_fun(): <src value=" << std::quoted(src) << ">" << std::endl;
    Parser parser(src);
    parser.expect(TokenKind::KwFun, "expected the keyword `fun`.");
    parser.consume(TokenKind::KwFun);
    std::cout << "Consumed the keyword 'fun'" << std::endl;
    parser.expect(TokenKind::Ident, "expected and identifier");
    auto fname = parser.current().text;
    parser.consume(TokenKind::Ident);
    std::cout << "Consumed the IDENTIFIER " << std::quoted(fname) << std::endl;

    parser.expect(TokenKind::LParen, "expected '('");
    parser.consume(TokenKind::LParen);
    std::cout << "Consumed the LParen '('" << std::endl;
    Vec<std::string> params{};
    while(true){
        parser.expect(TokenKind::Ident, "expected function parameter to be a symbol.");
        auto param = parser.current().text;
        params.push_back(param);
        parser.consume(TokenKind::Ident);

        std::cout << "Consumed the IDENTIFIER " << std::quoted(param) << std::endl;
        if(parser.current().kind == TokenKind::Comma){
            parser.consume(TokenKind::Comma);
            continue;
        }
        parser.expect(TokenKind::RParen, "expected function parameter to be a symbol.");
        parser.consume(TokenKind::Comma);
        break;
    }
    
    parser.expect(TokenKind::Equal, "expected '='");
    parser.consume(TokenKind::Equal);
    auto expr = parser.parse();
    this->m_funcs[fname] = {params, expr};

    std::ostringstream oss;
    Creola::print(oss, fname, "=", expr);

    return oss.str();
}

// -
std::string Creola::handle_command_simplify(const std::string& src){
    // simplify(expr)
    auto code = this->trim_command(src, "simplify"); // "(expr)"
    Parser parser(code);
    parser.expect(TokenKind::LParen, "expected '('");
    parser.consume(TokenKind::LParen);
    auto expr = parser.parse();
    parser.expect(TokenKind::RParen, "expected ')'");
    parser.consume(TokenKind::RParen);

    std::ostringstream oss;
    Creola::print(oss, expr->simplify());

    return oss.str();
}

// -
std::string Creola::handle_command_diff(const std::string& src){
    // diff(expr, var)
    auto code = this->trim_command(src, "diff"); // "(expr)"
    Parser parser(code);
    parser.expect(TokenKind::LParen, "expected '('");
    parser.consume(TokenKind::LParen);
    auto expr = parser.parse();
    parser.expect(TokenKind::Comma, "expected ','");
    parser.consume(TokenKind::Comma);

    parser.expect(TokenKind::Ident, "expected a variable name");
    auto var = parser.current().text;
    parser.consume(TokenKind::Ident);
    parser.expect(TokenKind::RParen, "expected ')'");
    parser.consume(TokenKind::RParen);

    auto result = expr->diff(var);
    std::ostringstream oss;
    Creola::print(oss, result->simplify());
    return oss.str();
}

// -
std::string Creola::handle_command_integrate(const std::string& src){
    // (1) integrate(expr, var)             
    // (2) integrate(expr, var, vmin, vmax)
    auto code = this->trim_command(src, "integrate");
    bool evaled{false};
    Parser parser(code);
    parser.expect(TokenKind::LParen, "expected '('");
    parser.consume(TokenKind::LParen);

    auto expr = parser.parse();

    parser.expect(TokenKind::Comma, "expected ','");
    parser.consume(TokenKind::Comma);

    parser.expect(TokenKind::Ident, "expected a variable name");
    auto var = parser.current().text;
    parser.consume(TokenKind::Ident);

    f64 vmin, vmax;
    if(parser.match(TokenKind::Comma)){
        parser.expect(TokenKind::Comma, "expected ','");
        parser.consume(TokenKind::Comma);

        // read the value of vmin
        if(parser.match(TokenKind::Ident)){
            auto vname = parser.current().text;
            parser.consume(TokenKind::Ident);
            this->validate_var(vname);
            auto numexpr = this->m_vars[vname];
            if(!Creola::is_number_expr(numexpr)){
                std::stringstream ess;
                ess << "invalid variable " << std::quoted(vname);
                throw CreolaError(ess.str());
            }
            Number num{0.0};
            Creola::as(numexpr, num);
            vmin = num.value();
        }else{
            parser.expect(TokenKind::Number, "expected a number");
            vmin = parser.current().num;
            parser.consume(TokenKind::Number);
        }

        parser.expect(TokenKind::Comma, "expected ','");
        parser.consume(TokenKind::Comma);

        // read the value of vmax
        if(parser.match(TokenKind::Ident)){
            auto vname = parser.current().text;
            parser.consume(TokenKind::Ident);
            this->validate_var(vname);
            
            auto numexpr = this->m_vars[vname];
            if(!Creola::is_number_expr(numexpr)){
                std::stringstream ess;
                ess << "invalid variable " << std::quoted(vname);
                throw CreolaError(ess.str());
            }
            Number num{0.0};
            Creola::as(numexpr, num);
            vmax = num.value();
        }else{
            parser.expect(TokenKind::Number, "expected a number");
            vmax = parser.current().num;
            parser.consume(TokenKind::Number);
        }

        evaled = true;
    }
    
    parser.expect(TokenKind::RParen, "expected ')'");
    parser.consume(TokenKind::RParen);

    auto F = expr->integrate(var);
    if(evaled){
        auto result = F->eval(var, vmax) - F->eval(var, vmin);
        return std::to_string(result);
    }

    std::ostringstream oss;
    Creola::print(oss, F->simplify());
    return oss.str();
}

// -
std::string Creola::handle_command_taylor(const std::string& src){
    // taylor(expr, var, center, order)
    auto code = this->trim_command(src, "taylor");
    Parser parser(code);
    parser.expect(TokenKind::LParen, "expected '('.");
    parser.consume(TokenKind::LParen);

    auto expr = parser.parse();

    parser.expect(TokenKind::Comma, "expected a ','.");
    parser.consume(TokenKind::Comma);

    parser.expect(TokenKind::Ident, "expected a variable.");
    auto var = parser.current().text;
    parser.consume(TokenKind::Ident);

    // read the center's value
    f64 center{};
    if(parser.match(TokenKind::Ident)){
        auto vname = parser.current().text;
        parser.consume(TokenKind::Ident);
        this->validate_var(vname);
        auto numexpr = this->m_vars[vname];
        if(!Creola::is_number_expr(numexpr)){
            std::ostringstream ess;
            ess << "invalid variable " << std::quoted(vname);
            throw CreolaError(ess.str());
        }
        Number num{0.0};
        Creola::as(numexpr, num);
        center = num.value();
    }else{
        parser.expect(TokenKind::Number, "expected a number");
        center = parser.current().num;
        parser.consume(TokenKind::Number);
    }

    parser.expect(TokenKind::Comma, "expected ','.");
    parser.consume(TokenKind::Comma);

    // read the order's value
    u32 order{};
    if(parser.match(TokenKind::Ident)){
        auto vname = parser.current().text;
        parser.consume(TokenKind::Ident);
        this->validate_var(vname);
        auto numexpr = this->m_vars[vname];
        if(!Creola::is_number_expr(numexpr)){
            std::ostringstream ess;
            ess << "invalid variable " << std::quoted(vname);
            throw CreolaError(ess.str());
        }
        Number num{0.0};
        Creola::as(numexpr, num);
        auto val = num.value();
        auto n1 = static_cast<u64>(std::ceil(val));
        auto n2 = static_cast<u64>(val);
        bool ok = (n1==n2) ? true : false;
        
        if(!ok){
            std::ostringstream ess;
            ess << "expected the last argument of `taylor` command to be an integer"; 
            throw CreolaError(ess.str());
        }
        order = n1;
    }

    parser.expect(TokenKind::RParen, "expected ')'.");
    parser.consume(TokenKind::RParen);

    auto result = expr->taylor(var, center, order);
    std::ostringstream oss;
    Creola::println(oss, result->simplify());
    return oss.str();
}

// 
std::string Creola::handle_command_expand(const std::string& src){
    // expand(expr)
    auto code = this->trim_command(src, "expand");
    Parser parser(code);
    parser.expect(TokenKind::LParen, "expected '('.");
    parser.consume(TokenKind::LParen);

    auto expr = parser.parse();

    parser.expect(TokenKind::RParen, "expected ')'.");
    parser.consume(TokenKind::RParen);

    std::ostringstream oss;
    Creola::print(oss, expr->expand()->simplify());
    return oss.str();
}

// -
std::string Creola::handle_command_factor(const std::string& src){
    // factor(expr, var)
    auto code = this->trim_command(src, "factor");
    Parser parser(code);
    parser.expect(TokenKind::LParen, "expected '('.");
    parser.consume(TokenKind::LParen);

    auto expr = parser.parse();

    parser.expect(TokenKind::Comma, "expected ','.");
    parser.consume(TokenKind::Comma);

    parser.expect(TokenKind::Ident, "expected a varibale name.");
    auto var = parser.current().text;
    parser.consume(TokenKind::Ident);

    parser.expect(TokenKind::RParen, "expected ')'.");
    parser.consume(TokenKind::RParen);

    std::ostringstream oss;
    Creola::print(oss, expr->factorize(var)->simplify());
    return oss.str();
}

// -
std::string Creola::handle_command_limit(const std::string& src){
    // limit(expr, var, val)
    auto code = this->trim_command(src, "limit");
    Parser parser(code);

    parser.expect(TokenKind::LParen, "expected '('.");
    parser.consume(TokenKind::LParen);

    auto expr = parser.parse();

    parser.expect(TokenKind::Comma, "expected ','.");
    parser.consume(TokenKind::Comma);

    parser.expect(TokenKind::Ident, "expected a variable name.");
    auto var = parser.current().text;
    parser.consume(TokenKind::RParen);

    parser.expect(TokenKind::Comma, "expected ','.");
    parser.consume(TokenKind::Comma);

    f64 val{};
    if(parser.match(TokenKind::Ident)){
        auto vname = parser.current().text;
        parser.consume(TokenKind::Ident);
        this->validate_var(vname);
        auto numexpr = this->m_vars[vname];
        if(!Creola::is_number_expr(numexpr)){
            std::ostringstream ess;
            ess << "invalid variable " << std::quoted(vname) << "'s type. Expected a number.";
            throw CreolaError(ess.str());
        }
        Number num{0.0};
        Creola::as(numexpr, num);
        val = num.value();
    }else{
        parser.expect(TokenKind::Number, "expected a number.");
        val = parser.current().num;
        parser.consume(TokenKind::Number);
    }

    parser.expect(TokenKind::RParen, "expected ')'.");
    parser.consume(TokenKind::RParen);

    auto ans = expr->limit(var, val);
    return std::to_string(ans);
}

//! @todo implement the helper method `handle_groebner()`
std::string Creola::handle_command_groebner(const std::string& src){
    //! @todo
    throw CreolaError("not implemented yet.");
}

//! @todo implement the helper method `handle_rewrite()`
std::string Creola::handle_command_rewrite(const std::string& src){
    //! @todo
    throw CreolaError("not implemented yet.");
}

//! @todo implement the helper method `handle_roots()`
std::string Creola::handle_command_roots(const std::string& src){
    //! @todo
    throw CreolaError("not implemented yet.");
}

//! @todo implement the helper method `handle_solve()`
std::string Creola::handle_command_solve(const std::string& src){
    //! @todo
    throw CreolaError("not implemented yet.");
}

//! @todo implement the helper method `handle_solve_system()`
std::string Creola::handle_command_solve_system(const std::string& src){
    //! @todo
    throw CreolaError("not implemented yet.");
}

//! @todo implement the helper method `handle_parfrac()`
std::string Creola::handle_command_partfrac(const std::string& src){
    //! @todo
    throw CreolaError("not implemented yet.");
}

std::string Creola::handle_command_equation(const std::string& src){
    //! @todo
    throw CreolaError("not implemented yet.");
}

std::string Creola::handle_command_system(const std::string& src){
    //! @todo
    throw CreolaError("not implemented yet.");
}

// -
std::string Creola::handle_command_factorial(const std::string& src){
    //! @todo
    throw CreolaError("not implemented yet.");
}

std::string Creola::handle_command_fibonacci(const std::string& src){
    //! @todo
    throw CreolaError("not implemented yet.");
}

// -
std::string Creola::handle_command_help(const std::string& src){
    //! @todo
    throw CreolaError("not implemented yet.");
}

// -
std::string Creola::handle_command_print(const std::string& src){
    //! @todo
    throw CreolaError("not implemented yet.");
}

// -
std::string Creola::handle_command_config(const std::string& src){
    //! @todo
    // >> config(limit.eps)
    // >> config(limit.epsilon, 0.000000001)
    // >> config(roots.epsilon, 0.0000000000001)
    // >> config(print.fancy, "true")
    // >> config(number.precision, 3)
    // >> config(number.format, "long"); "short", "none", "scientific", "long"
    // >> config(theme.number, "green")
    // >> config(theme.keyword, "blue")
    // >> config(theme.command, "yellow")
    // >> config(theme.background, "white")
    // >> config(theme.foreground, "black")
    // >> config(theme.fontFamily, "times")
    // >> config(theme.fontSize, "14")
    // >> config(prompt.ps1, ">> ")
    // >> config(prompt.ps2, "")
    // >> config(plot.linewidth, 2)
    // >> config(plot.linecolor, "blue")
    // >> config(plot.markerSize, 3)
    // >> config(plot.markerFontFamily, "verdana")
    // >> ...
    throw CreolaError("not implemented yet.");
}

// -
std::string Creola::handle_command_show(const std::string& src){
    //! @todo
    // >> show("keywords")
    // >> show("symbols")
    // >> show("constants")
    // >> show("builtins")
    // >> show("env")
    // >> show("variables")
    // >> show("functions")
    throw CreolaError("not implemented yet.");
}

// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola::core                                 -*-
// -*----------------------------------------------------------------*-