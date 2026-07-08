#include "core-expr.hpp"

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
Expr Number::groebner(const Vec<Expr>& exprs) const{
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
    //! @todo
    return 0.0;
}

Box Number::to_box(void) const{
    //! @todo
    return Box();
}
Box Number::to_box_prec(int parent_prec) const{
    //! @todo
    return Box();
}

// -*-
void Number::print_unicode(std::ostream& os, int prec) const{
    CREOLA_UNUSED(prec);
    os << this->value;
    return;
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
    return std::move(ans);
}

// -*-
Expr Symbol::expand(void) const{
    return std::make_shared<Symbol>(*this);
}


Expr Symbol::factor(const std::string& var) const{
    //! @todo
    return nullptr;
}
Expr Symbol::groebner(const Vec<Expr>& exprs) const{
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
    //! @todo
    return 0.0;
}

Box Symbol::to_box(void) const{
    //! @todo
    return Box();
}

Box Symbol::to_box_prec(int parent_prec) const{
    //! @todo
    return Box();
}
void Symbol::print_unicode(std::ostream& os, int prec) const{
    CREOLA_UNUSED(prec);
    os << this->name;
    return;
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
    //! @todo
    return nullptr;
}

// -*-
Expr Neg::expand(void) const{
    return std::make_shared<Neg>(this->arg->expand());
}
Expr Neg::factor(const std::string& var) const{
    //! @todo
    return nullptr;
}
Expr Neg::groebner(const Vec<Expr>& exprs) const{
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

f64 Neg::eval(const std::string& var, f64 val) const{
    //! @todo
    return 0.0;
}

Box Neg::to_box(void) const{
    //! @todo
    return Box();
}
Box Neg::to_box_prec(int parent_prec) const{
    //! @todo
    return Box();
}
void Neg::print_unicode(std::ostream& os, int prec) const{
    CREOLA_UNUSED(prec);
    os << "-";
    this->arg->print_unicode(os, 3);
    return;
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
    //! @todo
    return nullptr;
}


Expr Add::expand(void) const{
    Vec<Expr> result{};
    result.reserve(this->terms.size());
    for(auto& term: this->terms){
        result.push_back(term->expand());
    }
    return std::make_shared<Add>(result)->simplify();
}


Expr Add::factor(const std::string& var) const{
    //! @todo
    return nullptr;
}

Expr Add::groebner(const Vec<Expr>& exprs) const{
    //! @todo
    return nullptr;
}

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
    //! @todo
    return 0.0;
}

Box Add::to_box(void) const{
    //! @todo
    return Box();
}

Box Add::to_box_prec(int parent_prec) const{
    //! @todo
    return Box();
}
void Add::print_unicode(std::ostream& os, int prec) const{
    CREOLA_UNUSED(prec);
    for(size_t i=0; i < this->terms.size(); ++i){
        if (i > 0){
            os << " + ";
        }
        this->terms[i]->print_unicode(os, 1);
    }
    return;
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
    //! @todo
    return;
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
Expr Mul::groebner(const Vec<Expr>& exprs) const{
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
    //! @todo
    return;
}

Box Mul::to_box(void) const{
    //! @todo
    return;
}
Box Mul::to_box_prec(int parent_prec) const{
    //! @todo
    return;
}
void Mul::print_unicode(std::ostream& os, int prec) const{
    CREOLA_UNUSED(prec);
    for(size_t i=0; i < this->factors.size(); ++i){
        if(i > 0){ os << "*"; } // or just "" for implicit multiplication
        this->factors[i]->print_unicode(os, 2);
    }
    return;
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
    //! @todo
    return nullptr;
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
Expr Pow::groebner(const Vec<Expr>& exprs) const{
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
    //! @todo
    return 0.0L;
}

Box Pow::to_box(void) const{
    //! @todo
    return Box();
}
Box Pow::to_box_prec(int parent_prec) const{
    //! @todo
    return Box();
}

// -*-
void Pow::print_unicode(std::ostream& os, int prec) const{
    CREOLA_UNUSED(prec);
    this->base->print_unicode(os, 3);
    os << "^";
    this->expo->print_unicode(os, 3);
    return;
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
    //! @todo
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
Expr FuncCall::groebner(const Vec<Expr>& exprs) const{
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
    //! @todo
    return 0.0;
}

Box FuncCall::to_box(void) const{
    //! @todo
    return Box();
}
Box FuncCall::to_box_prec(int parent_prec) const{
    //! @todo
    return Box();
}

void FuncCall::print_unicode(std::ostream& os, int prec=0) const{
    CREOLA_UNUSED(prec);
    os << this->name << "(";
    for(size_t i=0; i < args.size(); ++i){
        if(i > 0){ os << ", "; }
        this->args[i]->print_unicode(os, 0);
    }
    os << ")";
}

// ----------------------------------
// --- High level elper functions ---
// ----------------------------------
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
    //! @todo
    return;
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