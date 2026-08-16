#include "creola/core/common.hpp"
#include "creola/core/expr.hpp"
#include "creola/core/engine.hpp"

#include<memory>
#include<vector>

//! @todo: Implement Expr->roots(...) -> Vec<f64>
//! @todo: Implement Expr->taylor(...) -> Expr
//! @todo: Implement Expr->limit(...) -> f64


// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola::core                               -*-
// -*----------------------------------------------------------------*-
namespace creola::core{
// -*-

// -------------------------
// --- Number Expression ---
// -------------------------
Expr Number::simplify(void) const{
    return Creola::make_number_expr(this->value());
}

// -*-
Expr Number::diff(const std::string& var) const{
    CREOLA_UNUSED(var);
    return Creola::make_number_expr(0.0);
}

// -*-
Expr Number::integrate(const std::string& var) const{
    // ∫ c dx = c x
    return Creola::make_mul_expr(Vec<Expr>{
        Creola::make_number_expr(this->value()),
        Creola::make_symbol_expr(var)
    });
}

// -*-
Expr Number::expand(void) const{
    return Creola::make_number_expr(this->value());
}

Expr Number::factorize(const std::string& var) const{
    //! @todo: implement integer factorization here.
    //! @note: Hard-coded for now
    return Creola::make_number_expr(this->value());
}

Expr Number::taylor(const std::string& var, f64 val, u32 n) const{
    //! @todo: 
    //! @note: Hard-coded for now
    return Creola::make_number_expr(this->value());
}

Vec<f64> Number::roots(const std::string& var, f64 vmin, f64 vmax, int samples) const{
    //! @todo
    //! @note: Hard-coded for now
    return {};
}

f64 Number::limit(const std::string& var, f64 val, f64 eps) const{
    //! @todo
    //! @note: probably throw a `RuntimeError`
    return 0.0;
}

f64 Number::eval(const std::string& var, f64 val) const{
    CREOLA_UNUSED(var);
    CREOLA_UNUSED(val);
    return this->value();
}

// -------------------------
// --- Symbol Expression ---
// -------------------------
Expr Symbol::simplify(void) const{
    return Creola::make_symbol_expr(this->name());
}

//
Expr Symbol::diff(const std::string& var) const{
    return Creola::make_number_expr(this->name()==var ? 1.0 : 0.0);
}

Expr Symbol::integrate(const std::string& var) const{
    // let var := x
    // ∫ sym dx => (sym == var) ? 1/2 x^2 : sym x 
    Expr ans = nullptr;
    if(this->name()==var){
        auto c = Creola::make_number_expr(0.5);
        auto x2 = Creola::make_pow_expr(
            Creola::make_symbol_expr(var),
            Creola::make_number_expr(2.0)
        );
        ans = Creola::make_mul_expr(Vec<Expr>{c, x2});
    }else{
        auto sym = Creola::make_symbol_expr(this->name());
        auto x = Creola::make_symbol_expr(var);
        ans = Creola::make_mul_expr(Vec<Expr>{sym, x});
    }
    return std::move(ans->simplify());
}

// -*-
Expr Symbol::expand(void) const{
    return Creola::make_symbol_expr(this->name());
}


Expr Symbol::factorize(const std::string& var) const{
    //! @todo
    return nullptr;
}
Expr Symbol::taylor(const std::string& var, f64 val, u32 n) const{
    //! @todo
    return nullptr;
}
Vec<f64> Symbol::roots(const std::string& var, f64 vmin, f64 vmax, int samples) const{
    //! @todo
    //! @note: Hard-coded for now
    return {};
}
f64 Symbol::limit(const std::string& var, f64 val, f64 eps) const{
    //! @todo
    //! @note: Hard-coded for now
    return 0.0;
}

f64 Symbol::eval(const std::string& var, f64 val) const{
    //! @todo: should through 'RuntimeError: undefined symbol `${var}`'
    return (this->name()==var) ? val : 0.0;
}

// ---------------------------
// --- Neg[ate] Expression ---
// ---------------------------
Expr Neg::simplify(void) const{
    auto expr = this->rhs()->simplify();
    if (auto num = std::dynamic_pointer_cast<Number>(expr)){
        return Creola::make_number_expr(-num->value());
    }
    return Creola::make_neg_expr(expr);
}

// -*-
Expr Neg::diff(const std::string& var) const{
    return Creola::make_neg_expr(this->rhs()->diff(var));
}

// -*-
Expr Neg::integrate(const std::string& var) const{
    // ∫- expr dx = - ∫ expr dx
    return Creola::make_neg_expr(this->rhs()->integrate(var))->simplify();
}

// -*-
Expr Neg::expand(void) const{
    return Creola::make_neg_expr(this->rhs()->expand());
}

Expr Neg::factorize(const std::string& var) const{
    //! @todo
    return nullptr;
}
Expr Neg::taylor(const std::string& var, f64 val, u32 n) const{
    //! @todo
    return nullptr;
}
Vec<f64> Neg::roots(const std::string& var, f64 vmin, f64 vmax, int samples) const{
    //! @todo
    //! @note: Hard-coded for now
    return {};
}
f64 Neg::limit(const std::string& var, f64 val, f64 eps) const{
    //! @todo
    //! @note: Hard-coded for now
    return 0.0;
}

// -*-
f64 Neg::eval(const std::string& var, f64 val) const{
    if(auto sym = std::dynamic_pointer_cast<Symbol>(this->rhs())){
        if(sym->name()==var){ return -1.0 * val; }
    }
    return 0.0;
}

// ----------------------
// --- Add Expression ---
// ----------------------
Expr Add::simplify(void) const{
    Vec<Expr> result{};
    f64 acc{0.0L};
    for(auto& term: this->terms()){
        auto expr = term->simplify();
        if (auto e = std::dynamic_pointer_cast<Add>(expr)){
            result.insert(result.end(), e->terms().begin(), e->terms().end());
        }else if (auto num = std::dynamic_pointer_cast<Number>(expr)){
            acc += num->value();
        }else{
            result.push_back(expr);
        }
    }

    if (std::fabsl(acc) != 0.0L){ result.push_back(Creola::make_number_expr(acc)); }
    if (result.empty()){ return Creola::make_number_expr(0.0); }
    if (result.size() == 1) { return result[0]; }
    return Creola::make_add_expr(result);
}

// -*-
Expr Add::diff(const std::string& var) const{
    Vec<Expr> result{};
    result.reserve(this->terms().size());
    for(auto& term: this->terms()){
        result.push_back(term->diff(var));
    }
    return Creola::make_add_expr(result)->simplify();
}

// -*-
Expr Add::integrate(const std::string& var) const{
    // ∫(f + g)dx = ∫ f dx + ∫ g dx
    Vec<Expr> result{};
    result.reserve(this->terms().size());
    for(auto& term: this->terms()){
        result.push_back(term->integrate(var));
    }
    return Creola::make_add_expr(result)->simplify();
}

// -*-
Expr Add::expand(void) const{
    Vec<Expr> result{};
    result.reserve(this->terms().size());
    for(auto& term: this->terms()){
        result.push_back(term->expand());
    }
    return Creola::make_add_expr(result)->simplify();
}

// -*-
Expr Add::factorize(const std::string& var) const{
    //! @todo
    return nullptr;
}

// -*-
Expr Add::taylor(const std::string& var, f64 val, u32 n) const{
    //! @todo
    return nullptr;
}

Vec<f64> Add::roots(const std::string& var, f64 vmin, f64 vmax, int samples) const{
    //! @todo
    //! @note: Hard-coded for now
    return {};
}
f64 Add::limit(const std::string& var, f64 val, f64 eps) const{
    //! @todo
    //! @note: Hard-coded for now
    return 0.0;
}

// -*-
f64 Add::eval(const std::string& var, f64 val) const{
    f64 ans = 0.0;
    for(auto& term: this->terms()){
        ans += term->eval(var, val);
    }
    return ans;
}


// ----------------------
// --- Mul Expression ---
// ----------------------
Expr Mul::simplify(void) const{
    Vec<Expr> result{};
    f64 acc{1.0L};
    for(auto& factor: this->factors()){
        auto expr = factor->simplify();
        if(auto e = std::dynamic_pointer_cast<Mul>(expr)){
            result.insert(result.end(), e->factors().begin(), e->factors().end());
        }else if(auto num = std::dynamic_pointer_cast<Number>(expr)){
            acc *= num->value();
        }else{
            result.push_back(expr);
        }
    }

    if(acc == 0.0L){ return Creola::make_number_expr(0.0L); }
    if(acc != 1.0L || result.empty()){
        result.push_back(Creola::make_number_expr(acc));
    }
    if(result.empty()){ return Creola::make_number_expr(1.0); }
    if(result.size()==1){ return result[0]; }

    return Creola::make_mul_expr(result);
}

// -*-
Expr Mul::diff(const std::string& var) const{
    // Product rule: (f1*...*fn)' = sum_i (f1*...*fi'*...*fn)
    Vec<Expr> sum_terms{};
    for(size_t i=0; i < this->factors().size(); ++i){
        Vec<Expr> product{};
        product.reserve(this->factors().size());
        for(size_t j=0; j < this->factors().size(); ++j){
            if(i == j){
                product.push_back(this->factors()[j]->diff(var));
            }else{
                product.push_back(this->factors()[j]);
            }
        }
        sum_terms.push_back(Creola::make_mul_expr(product));
    }
    return Creola::make_add_expr(sum_terms)->simplify();
}


Expr Mul::integrate(const std::string& var) const{
    // ∫ f * g dx
    //  (1) typeof(f) := Number
    //      ==> n * ∫ g dx
    //  (2) typeof(g) := Number
    //      ==> n * ∫ f dx
    //  (3) Fallback: FuncCall("∫_" + var, this->factors)
    if(this->factors().size()==2){
        auto num = std::dynamic_pointer_cast<Number>(this->factors()[0]);
        if(num){
            auto n = num->value();
            auto F = this->factors()[1]->integrate(var)->simplify();
            return Creola::make_mul_expr(Vec<Expr>{Creola::make_number_expr(n), F})->simplify();
        }
        num = std::dynamic_pointer_cast<Number>(this->factors()[1]);
        if(num){
            auto n = num->value();
            auto F = this->factors()[0]->integrate(var)->simplify();
            return Creola::make_mul_expr(Vec<Expr>{Creola::make_number_expr(n), F})->simplify();
        }
    }
    auto self = this->simplify();
    Vec<Expr> exprs{};
    for(auto& expr: this->factors()){
        exprs.push_back(expr->integrate(var)->simplify());
    }
    return Creola::make_mul_expr(exprs); // fallback
}

// -*-
Expr Mul::expand(void) const{
    if(this->factors().empty()){ return Creola::make_number_expr(1.0); }
    Expr acc = this->factors()[0]->expand();
    for(size_t i=1; i < this->factors().size(); ++i){
        Expr next = this->factors()[i]->expand();
        // distribute acc * next
        auto xadd = std::dynamic_pointer_cast<Add>(acc);
        auto yadd = std::dynamic_pointer_cast<Add>(next);
        Vec<Expr> neo_terms{};
        if(xadd && yadd){
            for(auto& xterm: xadd->terms()){
                for(auto& yterm: yadd->terms()){
                    neo_terms.push_back(Creola::make_mul_expr(Vec<Expr>{xterm, yterm}));
                }
            }
            acc = Creola::make_add_expr(neo_terms)->simplify();
        }else if (xadd){
            for(auto& xterm: xadd->terms()){
                neo_terms.push_back(Creola::make_mul_expr(Vec<Expr>{xterm, next}));
            }
            acc = Creola::make_add_expr(neo_terms)->simplify();
        }else if (yadd){
            for(auto& yterm: yadd->terms()){
                neo_terms.push_back(Creola::make_mul_expr(Vec<Expr>{acc, yterm}));
            }
            acc = Creola::make_add_expr(neo_terms)->simplify();
        }else{
            acc = Creola::make_mul_expr(Vec<Expr>{acc, next})->simplify();
        }
    }
    return acc;
}

Expr Mul::factorize(const std::string& var) const{
    //! @todo
    return nullptr;
}

Expr Mul::taylor(const std::string& var, f64 val, u32 n) const{
    //! @todo
    return nullptr;
}

Vec<f64> Mul::roots(const std::string& var, f64 vmin, f64 vmax, int samples) const{
    //! @todo
    //! @note: Hard-coded for now
    return {};
}
f64 Mul::limit(const std::string& var, f64 val, f64 eps) const{
    //! @todo
    //! @note: Hard-coded for now
    return 0.0;
}

f64 Mul::eval(const std::string& var, f64 val) const{
    f64 ans = 1.0;
    for(auto& expr: this->factors()){
        ans *= expr->eval(var, val);
    }
    return ans;
}


// ----------------------
// --- Pow Expression ---
// ----------------------
Expr Pow::simplify(void) const{
    auto b = this->base()->simplify();
    auto e = this->expo()->simplify();
    if(auto num = std::dynamic_pointer_cast<Number>(e)){
        if(num->value() == 1.0L){ return b; }
        if(num->value() == 0.0L){ return Creola::make_number_expr(1.0L); }
    }
    return Creola::make_pow_expr(b, e);
}

// -*-
Expr Pow::diff(const std::string& var) const{
    //! Only handle power with numeric exponent: (f^n)' = n f^(n-1) f'
    auto num = std::dynamic_pointer_cast<Number>(this->expo());
    if(num){
        auto _expo = Creola::make_number_expr(num->value()-1);
        auto fprime = this->base()->diff(var);
        return Creola::make_mul_expr(Vec<Expr>{
            Creola::make_number_expr(num->value()),
            Creola::make_pow_expr(this->base(), _expo),
            fprime
        })->simplify();
    }
    // general case: (f^g)' = f^g (g' ln f + g f'/f)
    auto ln_f = Creola::make_funcall_expr("ln", Vec<Expr>{this->base()});
    auto gprime = this->expo()->diff(var);
    auto fprime = this->base()->diff(var);
    auto term1 = Creola::make_mul_expr(Vec<Expr>{gprime, ln_f});
    auto term2 = Creola::make_mul_expr(Vec<Expr>{
        this->expo(),
        fprime,
        Creola::make_pow_expr(this->base(), Creola::make_number_expr(-1.0))
    });
    auto sum = Creola::make_add_expr(Vec<Expr>{term1, term2});
    return Creola::make_mul_expr(
        Vec<Expr>{Creola::make_pow_expr(this->base(), this->expo()), sum}
    )->simplify();
}

Expr Pow::integrate(const std::string& var) const{
    // ∫ b^e dx
    // (1) typeof(e) == Number && (typeof(b) = Symbol && b == var)
    //      ∫ f^n dx = 1/(n+1) x^(n+1)
    // (2) typeof(b) == Number && (typeof(e) == Symbol && e == var)
    //      ∫ b^x dx = b^x/ln(b)
    // (3)  fallback
    auto b = std::dynamic_pointer_cast<Symbol>(this->base());
    auto e = std::dynamic_pointer_cast<Number>(this->expo());
    if(b && e && b->name()==var){
        auto n = e->value() + 1;
        auto ans = Creola::make_mul_expr(Vec<Expr>{
            Creola::make_number_expr(1.0/n),
            Creola::make_pow_expr(Creola::make_symbol_expr(var), Creola::make_number_expr(n))
        });
        return ans->simplify();
    }
    auto xb = std::dynamic_pointer_cast<Number>(this->base());
    auto xe = std::dynamic_pointer_cast<Symbol>(this->expo());
    if(xb && xe && xe->name()==var){// lhs * rhs where lhs = 1/ln(b) and rhs = b^var
        auto lhs = Creola::make_number_expr(1.0/std::log(xb->value()));
        auto rhs = Creola::make_pow_expr(
            Creola::make_number_expr(xb->value()), Creola::make_symbol_expr(var)
        );
        auto ans = Creola::make_mul_expr(Vec<Expr>{lhs, rhs});
        return ans->simplify();
    }
    // fallback
    // ∫base^expo dvar
    return Creola::make_funcall_expr("∫_"+var, Vec<Expr>{
        Creola::make_pow_expr(this->base(), this->expo())->simplify()
    });
}

Expr Pow::expand(void) const{
    //! naive: only expand integer exponent >= 0
    auto num = std::dynamic_pointer_cast<Number>(this->expo());
    if(!num){
        return Creola::make_pow_expr(
            this->base()->expand(), this->expo()->expand()
        );
    }
    i64 k = std::llround(num->value());
    if(k < 0 || std::fabs(num->value() - k) > 1e-12){
        return Creola::make_pow_expr(
            this->base()->expand(),
            this->expo()->expand()
        );
    }

    Expr acc = Creola::make_number_expr(1.0);
    for(i64 i=0; i < k; i++){
        acc = Creola::make_mul_expr(Vec<Expr>{acc, this->base()})->expand();
    }
    return acc->simplify();
}

// -*-
Expr Pow::factorize(const std::string& var) const{
    //! @todo
    return nullptr;
}

Expr Pow::taylor(const std::string& var, f64 val, u32 n) const{
    //! @todo
    return nullptr;
}
Vec<f64> Pow::roots(const std::string& var, f64 vmin, f64 vmax, int samples) const{
    //! @todo
    //! @note: Hard-coded for now
    return {};
}
f64 Pow::limit(const std::string& var, f64 val, f64 eps) const{
    //! @todo
    //! @note: Hard-coded for now
    return 0.0L;
}

f64 Pow::eval(const std::string& var, f64 val) const{
    auto b = this->base()->eval(var, val);
    auto e = this->expo()->eval(var, val);
    return std::pow(b, e);
}


// ---------------------------
// --- FuncCall Expression ---
// ---------------------------
Expr FuncCall::simplify(void) const{
    Vec<Expr> argv{};
    argv.reserve(this->args().size());
    for(auto& arg: this->args()){
        argv.push_back(arg->simplify());
    }
    if(argv.size()==1){
        auto num = std::dynamic_pointer_cast<Number>(argv[0]);
        if(num){
            f64 val = num->value();
            auto node = Creola::UNARY_MATH_FUNCTIONS.find(this->name());
            if(node != Creola::UNARY_MATH_FUNCTIONS.end()){
                return node->second(val);
            }
        }
    }

    return Creola::make_funcall_expr(this->name(), argv);
}

// -*-
Expr FuncCall::diff(const std::string& var) const{
    if(this->args().size() != 1){
        return Creola::make_number_expr(0.0); // keep it simple
    }
    auto x = this->args()[0];
    auto dx = x->diff(var);
    if(name()=="sin"){
        return Creola::make_mul_expr(
            Vec<Expr>{
                Creola::make_funcall_expr("cos", Vec<Expr>{x}),
                dx
            }
        )->simplify();
    }

    if(name()=="cos"){
        return Creola::make_mul_expr(
            Vec<Expr>{
                Creola::make_number_expr(-1.0),
                Creola::make_funcall_expr("sin", Vec<Expr>{x}),
                dx
            }
        )->simplify();
    }

    if(name()=="exp"){
        return Creola::make_mul_expr(
            Vec<Expr>{Creola::make_funcall_expr("expr", Vec<Expr>{x}), dx}
        )->simplify();
    }

    if(name()=="ln"){
        return Creola::make_mul_expr(
            Vec<Expr>{Creola::make_pow_expr(x, Creola::make_number_expr(-1.0)), dx}
        )->simplify();
    }

    // ...

    // user-defined functions: f(x) ==> f'(x) * dx
    // We'll handle later
    return Creola::make_funcall_expr(this->name() + "'", Vec<Expr>{x, dx});
}

// -*-
Expr FuncCall::integrate(const std::string& var) const{
    if(this->name()=="sin"){
        //! @todo
    }

    if(this->name()=="cos"){
        //! @todo
    }

    if(this->name()=="tan"){
        //! @todo
    }

    if(this->name()=="asin"){
        //! @todo
    }

    if(this->name()=="acos"){
        //! @todo
    }

    if(this->name()=="atan"){
        //! @todo
    }

    if(this->name()=="sinh"){
        //! @todo
    }

    if(this->name()=="cosh"){
        //! @todo
    }

    if(this->name()=="tanh"){
        //! @todo
    }

    if(this->name()=="asinh"){
        //! @todo
    }

    if(this->name()=="acosh"){
        //! @todo
    }

    if(this->name()=="atanh"){
        //! @todo
    }

    if(this->name()=="exp"){
        //! @todo
    }

    if(this->name()=="sqrt"){
        //! @todo
    }

    return nullptr;
}

// -*-
Expr FuncCall::expand(void) const{
    Vec<Expr> argv{};
    argv.reserve(this->args().size());
    for(auto& arg: this->args()){
        argv.push_back(arg->expand());
    }
    return Creola::make_funcall_expr(this->name(), argv);
}

// -*-
Expr FuncCall::factorize(const std::string& var) const{
    //! @todo
    return nullptr;
}

// -*-
Expr FuncCall::taylor(const std::string& var, f64 val, u32 n) const{
    //! @todo
    //! @note: Hard-coded for now
    return nullptr;
}

// -*-
Vec<f64> FuncCall::roots(const std::string& var, f64 vmin, f64 vmax, int samples) const{
    //! @todo
    //! @note: Hard-coded for now
    return {};
}

// -*-
f64 FuncCall::limit(const std::string& var, f64 val, f64 eps) const{
    //! @todo
    //! @note: Hard-coded for now
    return 0.0;
}

// -*-
f64 FuncCall::eval(const std::string& var, f64 val) const{
    if(this->name()=="sin"){ return std::sin(this->args()[0]->eval(var, val)); }
    if(this->name()=="cos"){ return std::cos(this->args()[0]->eval(var, val)); }
    if(this->name()=="tan"){ return std::tan(this->args()[0]->eval(var, val)); }
    if(this->name()=="asin"){ return std::asin(this->args()[0]->eval(var, val)); }
    if(this->name()=="acos"){ return std::acos(this->args()[0]->eval(var, val)); }
    if(this->name()=="atan"){ return std::atan(this->args()[0]->eval(var, val)); }
    if(this->name()=="sinh"){ return std::sinh(this->args()[0]->eval(var, val)); }
    if(this->name()=="cosh"){ return std::cosh(this->args()[0]->eval(var, val)); }
    if(this->name()=="tanh"){ return std::tanh(this->args()[0]->eval(var, val)); }
    if(this->name()=="asinh"){ return std::asinh(this->args()[0]->eval(var, val)); }
    if(this->name()=="acosh"){ return std::acosh(this->args()[0]->eval(var, val)); }
    if(this->name()=="atanh"){ return std::atanh(this->args()[0]->eval(var, val)); }
    if(this->name()=="exp"){ return std::exp(this->args()[0]->eval(var, val)); }
    if(this->name()=="sqrt"){ return std::sqrt(this->args()[0]->eval(var, val)); }
    return 0.0;
}


// ----------------------------------
// --- High level elper functions ---
// ----------------------------------
std::ostream& operator<<(std::ostream& os, const Number& num){
    os << num.value();
    return os;
}

std::ostream& operator<<(std::ostream& os, const Symbol& sym){
    os << sym.name();
    return os;
}

// -
std::ostream& operator<<(std::ostream& os, const Neg& neg){
    os << "-";
    Creola::display(os, neg.rhs());
    return os;
}

std::ostream& operator<<(std::ostream& os, const Add& add){
    for(size_t i=0; i < add.terms().size(); ++i){
        auto term = add.terms()[i];
        if (i > 0){
            if(auto num = std::dynamic_pointer_cast<Number>(term)){
                Creola::print(os, (num->value() < 0 ? "-": "+"), std::fabs(num->value()));
                continue;
            }
            Creola::print(os, " + ");
        }
        Creola::print(os, add.terms()[i]);
    }
    return os;
}

// -
std::ostream& operator<<(std::ostream& os, const Mul& mul){
    for(size_t i=0; i < mul.factors().size(); ++i){
        if (i > 0){
            if(auto num = std::dynamic_pointer_cast<Number>(mul.factors()[i])){
                auto val = num->value();
                auto test = (val < 0);
                std::string str = (test ? "(-":"") + std::to_string(std::fabs(val));
                str += test ? ")": "";
                Creola::print(os, "*", str);
                continue;
            }
            Creola::print(os, "*");
        }
        //os << *mul.factors()[i];
        Creola::print(os, mul.factors()[i]);
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Pow& pow){
    if(auto num = std::dynamic_pointer_cast<Number>(pow.base())){
        auto val = num->value();
        auto test = (val < 0);
        std::string txt = ( test ? "(-": "") + std::to_string(std::fabs(val));
        txt += (test ? ")": "");
        Creola::print(os, txt);
    }else{
        Creola::print(os, pow.base());
    }
    Creola::print(os, "^");
    
    if(auto num = std::dynamic_pointer_cast<Number>(pow.expo())){
        auto val = num->value();
        auto test = (val < 0);
        std::string txt = ( test ? "(-": "") + std::to_string(std::fabs(val));
        txt += (test ? ")": "");
        Creola::print(os, txt);
    }else{
        Creola::print(os, pow.expo());
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const FuncCall& fcall){
    os << fcall.name() << "(";
    for(size_t i=0; i < fcall.args().size(); ++i){
        if(i > 0){ os << ", "; }
        // os << *fcall.args()[i];
        Creola::print(os, fcall.args()[i]);
    }
    os << ")";
    return os;
}

// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola::core                                 -*-
// -*----------------------------------------------------------------*-