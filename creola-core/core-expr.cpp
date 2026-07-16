#include "core-expr.hpp"

//! @todo: Implement Expr->roots(...) -> Vec<f64>
//! @todo: Implement Expr->taylor(...) -> Expr
//! @todo: Implement Expr->limit(...) -> f64


// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola::core                               -*-
// -*----------------------------------------------------------------*-
namespace creola::core{
//
// -------------------------
// --- Number Expression ---
// -------------------------
Expr Number::simplify(void) const{
    return std::make_shared<Number>(*this);
}

// -*-
Expr Number::diff(const std::string& var) const{
    CREOLA_UNUSED(var);
    return std::make_shared<Number>(0.0);
}

// -*-
Expr Number::integrate(const std::string& var) const{
    // ∫ c dx = c x
    return std::make_shared<Mul>(Vec<Expr>{number(this->value), symbol(var)});
}

// -*-
Expr Number::expand(void) const{
    return std::make_shared<Number>(*this);
}

Expr Number::factor(const std::string& var) const{
    //! @todo
    return nullptr;
}

Expr Number::taylor(const std::string& var, f64 val, int n) const{
    //! @todo
    return nullptr;
}
Vec<f64> Number::roots(const std::string& var, f64 vmin, f64 vmax, int samples) const{
    //! @todo
    return {};
}
f64 Number::limit(const std::string& var, f64 val, f64 eps=1e-6) const{
    //! @todo
    return 0.0;
}

f64 Number::eval(const std::string& var, f64 val) const{
    CREOLA_UNUSED(var);
    CREOLA_UNUSED(val);
    return this->value;
}

// -------------------------
// --- Symbol Expression ---
// -------------------------
Expr Symbol::simplify(void) const{
    return std::make_shared<Symbol>(*this);
}

//
Expr Symbol::diff(const std::string& var) const{
    return std::make_shared<Number>(this->name==var ? 1.0 : 0.0);
}

Expr Symbol::integrate(const std::string& var) const{
    // let var := x
    // ∫ sym dx => (sym == var) ? 1/2 x^2 : sym x 
    Expr ans = nullptr;
    if(this->name==var){
        auto c = number(0.5);
        auto x2 = std::make_shared<Pow>(symbol(var), number(2.0));
        ans = std::make_shared<Mul>(Vec<Expr>{c, x2});
    }else{
        auto sym = symbol(this->name);
        auto x = symbol(var);
        ans = std::make_shared<Mul>(Vec<Expr>{sym, x});
    }
    return std::move(ans->simplify());
}

// -*-
Expr Symbol::expand(void) const{
    return std::make_shared<Symbol>(*this);
}


Expr Symbol::factor(const std::string& var) const{
    //! @todo
    return nullptr;
}
Expr Symbol::taylor(const std::string& var, f64 val, int n) const{
    //! @todo
    return nullptr;
}
Vec<f64> Symbol::roots(const std::string& var, f64 vmin, f64 vmax, int samples) const{
    //! @todo
    return {};
}
f64 Symbol::limit(const std::string& var, f64 val, f64 eps) const{
    //! @todo
    return 0.0;
}

f64 Symbol::eval(const std::string& var, f64 val) const{
    //! @todo: should through 'RuntimeError: undefined symbol `${var}`'
    return (this->name==var) ? val : 0.0;
}

// ---------------------------
// --- Neg[ate] Expression ---
// ---------------------------
Expr Neg::simplify(void) const{
    auto expr = this->arg->simplify();
    if (auto num = std::dynamic_pointer_cast<Number>(expr)){
        return number(-num->value);
    }
    return std::make_shared<Neg>(expr);
}

// -*-
Expr Neg::diff(const std::string& var) const{
    return std::make_shared<Neg>(this->arg->diff(var));
}

// -*-
Expr Neg::integrate(const std::string& var) const{
    // ∫- expr dx = - ∫ expr dx
    return std::make_shared<Neg>(this->arg->integrate(var))->simplify();
}

// -*-
Expr Neg::expand(void) const{
    return std::make_shared<Neg>(this->arg->expand());
}
Expr Neg::factor(const std::string& var) const{
    //! @todo
    return nullptr;
}
Expr Neg::taylor(const std::string& var, f64 val, int n) const{
    //! @todo
    return nullptr;
}
Vec<f64> Neg::roots(const std::string& var, f64 vmin, f64 vmax, int samples) const{
    //! @todo
    return {};
}
f64 Neg::limit(const std::string& var, f64 val, f64 eps=1e-6) const{
    //! @todo
    return 0.0;
}

// -*-
f64 Neg::eval(const std::string& var, f64 val) const{
    if(auto sym = std::dynamic_pointer_cast<Symbol>(this->arg)){
        if(sym->name==var){ return -1.0 * val; }
    }
    return 0.0;
}

// ----------------------
// --- Add Expression ---
// ----------------------
Expr Add::simplify(void) const{
    Vec<Expr> result{};
    f64 acc{0.0L};
    for(auto& term: this->terms){
        auto expr = term->simplify();
        if (auto e = std::dynamic_pointer_cast<Add>(expr)){
            result.insert(result.end(), e->terms.begin(), e->terms.end());
        }else if (auto num = std::dynamic_pointer_cast<Number>(expr)){
            acc += num->value;
        }else{
            result.push_back(expr);
        }
    }

    if (std::fabsl(acc) != 0.0L){ result.push_back(number(acc)); }
    if (result.empty()){ return number(0); }
    if (result.size() == 1) { return result[0]; }
    return std::make_shared<Add>(result);
}


Expr Add::diff(const std::string& var) const{
    Vec<Expr> result{};
    result.reserve(this->terms.size());
    for(auto& term: this->terms){
        result.push_back(term->diff(var));
    }
    return std::make_shared<Add>(result)->simplify();
}


Expr Add::integrate(const std::string& var) const{
    // ∫(f + g)dx = ∫ f dx + ∫ g dx
    Vec<Expr> result{};
    result.reserve(this->terms.size());
    for(auto& term: this->terms){
        result.push_back(term->integrate(var));
    }
    return std::make_shared<Add>(result)->simplify();
}


Expr Add::expand(void) const{
    Vec<Expr> result{};
    result.reserve(this->terms.size());
    for(auto& term: this->terms){
        result.push_back(term->expand());
    }
    return std::make_shared<Add>(result)->simplify();
}

// -*-
Expr Add::factor(const std::string& var) const{
    //! @todo
    return nullptr;
}

// -*-
Expr Add::taylor(const std::string& var, f64 val, int n) const{
    //! @todo
    return nullptr;
}

Vec<f64> Add::roots(const std::string& var, f64 vmin, f64 vmax, int samples) const{
    //! @todo
    return {};
}
f64 Add::limit(const std::string& var, f64 val, f64 eps) const{
    //! @todo
    return 0.0;
}

f64 Add::eval(const std::string& var, f64 val) const{
    f64 ans = 0.0;
    for(auto& term: this->terms){
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
    for(auto& factor: this->factors){
        auto expr = factor->simplify();
        if(auto e = std::dynamic_pointer_cast<Mul>(expr)){
            result.insert(result.end(), e->factors.begin(), e->factors.end());
        }else if(auto num = std::dynamic_pointer_cast<Number>(expr)){
            acc *= num->value;
        }else{
            result.push_back(expr);
        }
    }

    if(acc == 0.0L){ return number(0.0L); }
    if(acc != 1.0L || result.empty()){
        result.push_back(number(acc));
    }
    if(result.empty()){ return number(1.0L); }
    if(result.size()==1){ return result[0]; }

    return std::make_shared<Mul>(result);
}

// -*-
Expr Mul::diff(const std::string& var) const{
    // Product rule: (f1*...*fn)' = sum_i (f1*...*fi'*...*fn)
    Vec<Expr> sum_terms{};
    for(size_t i=0; i < this->factors.size(); ++i){
        Vec<Expr> product{};
        product.reserve(this->factors.size());
        for(size_t j=0; j < this->factors.size(); ++j){
            if(i == j){
                product.push_back(this->factors[j]->diff(var));
            }else{
                product.push_back(this->factors[j]);
            }
        }
        sum_terms.push_back(std::make_shared<Mul>(product));
    }
    return std::make_shared<Add>(sum_terms)->simplify();
}


Expr Mul::integrate(const std::string& var) const{
    // ∫ f * g dx
    //  (1) typeof(f) := Number
    //      ==> n * ∫ g dx
    //  (2) typeof(g) := Number
    //      ==> n * ∫ f dx
    //  (3) Fallback: FuncCall("∫_" + var, this->factors)
    if(this->factors.size()==2){
        auto num = std::dynamic_pointer_cast<Number>(this->factors[0]);
        if(num){
            auto n = num->value;
            auto F = this->factors[1]->integrate(var)->simplify();
            return std::make_shared<Mul>(Vec<Expr>{number(n), F})->simplify();
        }
        num = std::dynamic_pointer_cast<Number>(this->factors[1]);
        if(num){
            auto n = num->value;
            auto F = this->factors[0]->integrate(var)->simplify();
            return std::make_shared<Mul>(Vec<Expr>{number(n), F})->simplify();
        }
    }
    auto self = this->simplify();
    Vec<Expr> exprs{};
    for(auto& expr: this->factors){
        exprs.push_back(expr->integrate(var)->simplify());
    }
    return std::make_shared<Mul>(exprs); // fallback
}

// -*-
Expr Mul::expand(void) const{
    if(this->factors.empty()){ return number(1.0L); }
    Expr acc = this->factors[0]->expand();
    for(size_t i=1; i < this->factors.size(); ++i){
        Expr next = this->factors[i]->expand();
        // distribute acc * next
        auto xadd = std::dynamic_pointer_cast<Add>(acc);
        auto yadd = std::dynamic_pointer_cast<Add>(next);
        Vec<Expr> neo_terms{};
        if(xadd && yadd){
            for(auto& xterm: xadd->terms){
                for(auto& yterm: yadd->terms){
                    neo_terms.push_back(std::make_shared<Mul>(Vec<Expr>{xterm, yterm}));
                }
            }
            acc = std::make_shared<Add>(neo_terms)->simplify();
        }else if (xadd){
            for(auto& xterm: xadd->terms){
                neo_terms.push_back(std::make_shared<Mul>(Vec<Expr>{xterm, next}));
            }
            acc = std::make_shared<Add>(neo_terms)->simplify();
        }else if (yadd){
            for(auto& yterm: yadd->terms){
                neo_terms.push_back(std::make_shared<Mul>(Vec<Expr>{acc, yterm}));
            }
            acc = std::make_shared<Add>(neo_terms)->simplify();
        }else{
            acc = std::make_shared<Mul>(Vec<Expr>{acc, next})->simplify();
        }
    }
    return acc;
}

Expr Mul::factor(const std::string& var) const{
    //! @todo
    return;
}

Expr Mul::taylor(const std::string& var, f64 val, int n) const{
    //! @todo
    return;
}

Vec<f64> Mul::roots(const std::string& var, f64 vmin, f64 vmax, int samples) const{
    //! @todo
    return;
}
f64 Mul::limit(const std::string& var, f64 val, f64 eps) const{
    //! @todo
    return;
}

f64 Mul::eval(const std::string& var, f64 val) const{
    f64 ans = 1.0;
    for(auto& expr: this->factors){
        ans *= expr->eval(var, val);
    }
    return ans;
}


// ----------------------
// --- Pow Expression ---
// ----------------------
Expr Pow::simplify(void) const{
    auto b = this->base->simplify();
    auto e = this->expo->simplify();
    if(auto num = std::dynamic_pointer_cast<Number>(e)){
        if(num->value == 1.0L){ return b; }
        if(num->value == 0.0L){ return number(1.0L); }
    }
    return std::make_shared<Pow>(b, e);
}

// -*-
Expr Pow::diff(const std::string& var) const{
    //! Only handle power with numeric exponent: (f^n)' = n f^(n-1) f'
    auto num = std::dynamic_pointer_cast<Number>(this->expo);
    if(num){
        auto _expo = number(num->value-1);
        auto fprime = this->base->diff(var);
        return std::make_shared<Mul>(Vec<Expr>{
            number(num->value),
            std::make_shared<Pow>(this->base, _expo),
            fprime
        })->simplify();
    }
    // general case: (f^g)' = f^g (g' ln f + g f'/f)
    auto ln_f = std::make_shared<FuncCall>("ln", Vec<Expr>{this->base});
    auto gprime = this->expo->diff(var);
    auto fprime = this->base->diff(var);
    auto term1 = std::make_shared<Mul>(Vec<Expr>{gprime, ln_f});
    auto term2 = std::make_shared<Mul>(Vec<Expr>{
        this->expo, fprime, std::make_shared<Pow>(this->base, number(-1))
    });
    auto sum = std::make_shared<Add>(Vec<Expr>{term1, term2});
    return std::make_shared<Mul>(
        Vec<Expr>{std::make_shared<Pow>(this->base, this->expo), sum}
    )->simplify();
}

Expr Pow::integrate(const std::string& var) const{
    // ∫ b^e dx
    // (1) typeof(e) == Number && (typeof(b) = Symbol && b == var)
    //      ∫ f^n dx = 1/(n+1) x^(n+1)
    // (2) typeof(b) == Number && (typeof(e) == Symbol && e == var)
    //      ∫ b^x dx = b^x/ln(b)
    // (3)  fallback
    auto b = std::dynamic_pointer_cast<Symbol>(this->base);
    auto e = std::dynamic_pointer_cast<Number>(this->expo);
    if(b && e && b->name==var){
        auto n = e->value + 1;
        auto ans = std::make_shared<Mul>(Vec<Expr>{
            number(1.0/n),
            std::make_shared<Pow>(symbol(var), number(n))
        });
        return ans->simplify();
    }
    auto xb = std::dynamic_pointer_cast<Number>(this->base);
    auto xe = std::dynamic_pointer_cast<Symbol>(this->expo);
    if(xb && xe && xe->name==var){// lhs * rhs where lhs = 1/ln(b) and rhs = b^var
        auto lhs = number(1.0/std::log(xb->value));
        auto rhs = std::make_shared<Pow>(
            number(xb->value), symbol(var)
        );
        auto ans = std::make_shared<Mul>(Vec<Expr>{lhs, rhs});
        return ans->simplify();
    }
    // fallback
    // ∫base^expo dvar
    return std::make_shared<FuncCall>("∫_"+var, Vec<Expr>{
        std::make_shared<Pow>(this->base, this->expo)->simplify()
    });
}

Expr Pow::expand(void) const{
    //! naive: only expand integer exponent >= 0
    auto num = std::dynamic_pointer_cast<Number>(this->expo);
    if(!num){
        return std::make_shared<Pow>(
            this->base->expand(), this->expo->expand()
        );
    }
    i64 k = std::llround(num->value);
    if(k < 0 || std::fabs(num->value - k) > 1e-12){
        return std::make_shared<Pow>(this->base->expand(), this->expo->expand());
    }

    Expr acc = number(1);
    for(i64 i=0; i < k; i++){
        acc = std::make_shared<Mul>(Vec<Expr>{acc, this->base})->expand();
    }
    return acc->simplify();
}

// -*-
Expr Pow::factor(const std::string& var) const{
    //! @todo
    return nullptr;
}

Expr Pow::taylor(const std::string& var, f64 val, int n) const{
    //! @todo
    return nullptr;
}
Vec<f64> Pow::roots(const std::string& var, f64 vmin, f64 vmax, int samples) const{
    //! @todo
    return {};
}
f64 Pow::limit(const std::string& var, f64 val, f64 eps) const{
    //! @todo
    return 0.0L;
}

f64 Pow::eval(const std::string& var, f64 val) const{
    auto b = this->base->eval(var, val);
    auto e = this->expo->eval(var, val);
    return std::pow(b, e);
}


// ---------------------------
// --- FuncCall Expression ---
// ---------------------------
Expr FuncCall::simplify(void) const{
    Vec<Expr> argv{};
    argv.reserve(this->args.size());
    for(auto& arg: args){
        argv.push_back(arg->simplify());
    }
    if(argv.size()==1){
        auto num = std::dynamic_pointer_cast<Number>(argv[0]);
        if(num){
            f64 val = num->value;
            auto entry = MFUNCS_TABLE.find(this->name);
            if(entry != MFUNCS_TABLE.end()){
                auto ans = entry->second(val);
                return number(ans);
            }
        }
    }

    return std::make_shared<FuncCall>(this->name, argv);
}

// -*-
Expr FuncCall::diff(const std::string& var) const{
    if(this->args.size() != 1){
        return number(0.0); // keep it simple
    }
    auto x = this->args[0];
    auto dx = x->diff(var);
    if(name=="sin"){
        return std::make_shared<Mul>(
            Vec<Expr>{
                std::make_shared<FuncCall>("cos", Vec<Expr>{x}),
                dx
            }
        )->simplify();
    }

    if(name=="cos"){
        return std::make_shared<Mul>(
            Vec<Expr>{
                number(-1.0),
                std::make_shared<FuncCall>("sin", Vec<Expr>{x}),
                dx
            }
        )->simplify();
    }

    if(name=="exp"){
        return std::make_shared<Mul>(
            std::make_shared<FuncCall>("expr", Vec<Expr>{x}),
            dx
        )->simplify();
    }

    if(name=="ln"){
        return std::make_shared<Mul>(
            Vec<Expr>{std::make_shared<Pow>(x, number(-1.0)), dx}
        )->simplify();
    }

    // ...

    // user-defined functions: f(x) ==> f'(x) * dx
    // We'll handle later
    return std::make_shared<FuncCall>(this->name + "'", Vec<Expr>{x, dx});
}

Expr FuncCall::integrate(const std::string& var) const{
    if(this->name=="sin"){
        //! @todo
    }

    if(this->name=="cos"){
        //! @todo
    }

    if(this->name=="tan"){
        //! @todo
    }

    if(this->name=="asin"){
        //! @todo
    }

    if(this->name=="acos"){
        //! @todo
    }

    if(this->name=="atan"){
        //! @todo
    }

    if(this->name=="sinh"){
        //! @todo
    }

    if(this->name=="cosh"){
        //! @todo
    }

    if(this->name=="tanh"){
        //! @todo
    }

    if(this->name=="asinh"){
        //! @todo
    }

    if(this->name=="acosh"){
        //! @todo
    }

    if(this->name=="atanh"){
        //! @todo
    }

    if(this->name=="exp"){
        //! @todo
    }

    if(this->name=="sqrt"){
        //! @todo
    }

    return nullptr;
}


Expr FuncCall::expand(void) const{
    Vec<Expr> argv{};
    argv.reserve(this->args.size());
    for(auto& arg: args){
        argv.push_back(arg->expand());
    }
    return std::make_shared<FuncCall>(this->name, argv);
}

Expr FuncCall::factor(const std::string& var) const{
    //! @todo
    return nullptr;
}
Expr FuncCall::taylor(const std::string& var, f64 val, int n) const{
    //! @todo
    return nullptr;
}
Vec<f64> FuncCall::roots(const std::string& var, f64 vmin, f64 vmax, int samples) const{
    //! @todo
    return {};
}
f64 FuncCall::limit(const std::string& var, f64 val, f64 eps) const{
    //! @todo
    return 0.0;
}

f64 FuncCall::eval(const std::string& var, f64 val) const{
    if(this->name=="sin"){ return std::sin(this->args[0]->eval(var, val)); }
    if(this->name=="cos"){ return std::cos(this->args[0]->eval(var, val)); }
    if(this->name=="tan"){ return std::tan(this->args[0]->eval(var, val)); }
    if(this->name=="asin"){ return std::asin(this->args[0]->eval(var, val)); }
    if(this->name=="acos"){ return std::acos(this->args[0]->eval(var, val)); }
    if(this->name=="atan"){ return std::atan(this->args[0]->eval(var, val)); }
    if(this->name=="sinh"){ return std::sinh(this->args[0]->eval(var, val)); }
    if(this->name=="cosh"){ return std::cosh(this->args[0]->eval(var, val)); }
    if(this->name=="tanh"){ return std::tanh(this->args[0]->eval(var, val)); }
    if(this->name=="asinh"){ return std::asinh(this->args[0]->eval(var, val)); }
    if(this->name=="acosh"){ return std::acosh(this->args[0]->eval(var, val)); }
    if(this->name=="atanh"){ return std::atanh(this->args[0]->eval(var, val)); }
    if(this->name=="exp"){ return std::exp(this->args[0]->eval(var, val)); }
    if(this->name=="sqrt"){ return std::sqrt(this->args[0]->eval(var, val)); }
    return 0.0;
}


// ----------------------------------
// --- High level elper functions ---
// ----------------------------------

/*
std::ostream& operator<<(std::ostream& os, const Number& rhs);
std::ostream& operator<<(std::ostream& os, const Symbol& rhs);
std::ostream& operator<<(std::ostream& os, const Neg& rhs);
std::ostream& operator<<(std::ostream& os, const Add& rhs);
std::ostream& operator<<(std::ostream& os, const Mul& rhs);
std::ostream& operator<<(std::ostream& os, const Pow& rhs);
std::ostream& operator<<(std::ostream& os, const FuncCall& rhs);
*/

Expr simplify(const Expr& expr){
    return expr->simplify();
}

Expr diff(const Expr& expr, const std::string& var){
    return expr->diff(var);
}

Expr expand(const Expr& expr){
    return expr->expand();
}

Expr factor(const Expr& expr, const std::string& var){
    //! @todo
    return;
}
Expr integrate(const Expr& expr, const std::string& var){
    //! @todo
    return;
}
f64 eval(const Expr& expr, const std::string& var, f64 val){
    //! @todo
    return;
}
Vec<f64> roots(const Expr& expr, const std::string& var, f64 vmin, f64 vmax, int samples){
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

Expr taylor(const Expr& expr, const std::string& var, f64 val, int n){
    //! @todo
    return;
}
f64 limit(const Expr& expr, const std::string& var, f64 val, int max_iter=5){
    //! @todo
    return;
}


// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola::core                                 -*-
// -*----------------------------------------------------------------*-