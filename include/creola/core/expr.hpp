#pragma once

#include "creola/core/common.hpp"
#include "creola/core/pprint.hpp"
#include "creola/core/engine.hpp"

#include<functional>
#include<unordered_map>
#include<iostream>
#include<memory>
#include<vector>
#include<string>
#include<cmath>


// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola::core                               -*-
// -*----------------------------------------------------------------*-
namespace creola::core{
//
// -

/*
Table of Derivatives:
--------------------
x^a         ==> a x^(a-1)
exp(x)      ==> exp(x)
a^x         ==> ln a a^x
ln(x)       ==> 1/x
sin(x)      ==> cos x
cos(x)      ==> -sin x
tan(x)      ==> 1 + (tan(x))^2
asin(x)     ==> 1/(sqrt(1 - x^2))
acos(x)     ==> -1/(sqrt(1 - x^2))
atan(x)     ==> 1/(1 + x^2)
sinh(x)     ==> cosh(x)
cosh(x)     ==> sinh(x)
tanh(x)     ==> 1 - (tanh(x))^2


Integration Rules
-----------------
∫ c dx = cx + K
∫ c f(x) dx = c ∫f(x)dx
∫(f(x) + g(x)) dx = ∫f(x)dx + ∫g(x)dx
∫(f(x) - g(x)) dx = ∫f(x)dx - ∫g(x)dx 
∫ x^n dx = x^(n+1)/(n+1) + K, (n != -1)
∫ 1/x dx = ln(|x|) + K
∫ exp(x) dx = exp(x) + K
∫ a^x dx = a^(x)/(ln(a)) + K
∫ sin(x) dx = -cos(x) + K
∫ cos(x) dx = sin(x) + K
∫ (sec(x))^2 dx = tan(x) + K
∫ (csc(x))^2 dx = -cot(x) + K
∫ sec() tan(x) dx = sec(x) + K
∫ csc(x) cot(x) dx = -csc(x) + C
∫ 1/(x^2 + 1) dx = 1/(tan(x)) + K
∫ 1/(sqrt(1 - x^2)) dx = 1/(sin(x)) + K
∫ sinh(x) dx = cosh(x) + K
∫ cosh(x) dx = sinh(x) + K

Notations:
 csc(x) = 1/sin(x)
 sec(x) = 1/cos(x)
 cot(x) = 1/tan(x) = cos(x)/sin(x) = csc(x)/sec(x) 

*/

// -* Simplifier::simplify -*-
struct Simplifier {
    explicit Simplifier(const ExprBase* expr)
    : m_expr{expr} {}
    virtual ~Simplifier() = default;

    virtual Expr simplify(void) const = 0;

private:
    const ExprBase* m_expr;
    friend class Creola;
};

// -*- Factorizer::factor -*-
struct Factorizer{
    explicit Factorizer(const ExprBase* expr): m_expr{expr}
    {}

    virtual ~Factorizer() = default;

    virtual Expr factorize(const std::string& var) const = 0;

private:
    const ExprBase* m_expr;
    friend class Creola;
};

// -*- Expander::expand -*-
struct Expander{
    explicit Expander(const ExprBase* expr): m_expr{expr}
    {}

    virtual ~Expander() = default;

    virtual Expr expand(void) const = 0;

private:
    const ExprBase* m_expr;
    friend class Creola;
};

// -*- Integrator::integrate -*-
struct Integrator {
    explicit Integrator(const ExprBase* expr)
    : m_expr{expr}, m_var{"x"}
    {}
    virtual ~Integrator() = default;

    virtual Expr integrate(const std::string& var) const = 0;
    void update_integrate_config(const std::string& var){
        this->m_var = var;
    }


private:
    const ExprBase* m_expr;
    std::string m_var;
    friend class Creola;
};

// -*- Differentiator::diff -*-
struct Differentiator {
    explicit Differentiator(const ExprBase* expr)
    {}
    virtual ~Differentiator() = default;

    virtual Expr diff(const std::string& var) const = 0;

    void update_diff_var(const std::string& var){
        this->m_var = var;
    }

private:
    const ExprBase* m_expr;
    std::string m_var;
    friend class Creola;
};

// -*- Series::taylor -*-
struct Series {
    explicit Series(const ExprBase* expr)
    : m_expr{expr}, m_var{"x"}
    , m_val{std::numeric_limits<f64>::max()}
    , m_order{std::numeric_limits<int>::max()}
    {}

    virtual ~Series() = default;

    virtual Expr taylor(const std::string& var, f64 val, int n) const = 0;
    void update_series_config(const std::string& var, f64 val, int order){
        this->m_var = var;
        this->m_val = val;
        this->m_order = order;
    }

    friend std::ostream& operator<<(std::ostream& os, const Series& series);

private:
    const ExprBase* m_expr;
    std::string m_var;
    f64 m_val;
    int m_order;
    friend class Creola;
};

// -*- LimitFinder::limit -*-
struct LimitFinder {
    explicit LimitFinder(const ExprBase* expr)
    : m_expr{expr}, m_var{"x"}
    , m_val{std::numeric_limits<f64>::max()}
    , m_eps{1e-6}
    {}

    virtual ~LimitFinder() = default;

    virtual f64 limit(const std::string& var, f64 val, f64 eps=1e-6) const = 0;

    void update_limit_config(const std::string& var, f64 val, f64 eps){
        this->m_var = var;
        this->m_val = val;
        this->m_eps = eps;
    }

private:
    const ExprBase* m_expr;
    std::string m_var;
    f64 m_val;
    f64 m_eps;
    friend class Creola;
};

// -*- RootsFinder::roots -*-
struct RootsFinder {
    explicit RootsFinder(const ExprBase* expr)
    : m_expr{expr}, m_var{"x"}
    , m_vmin{std::numeric_limits<f64>::max()}
    , m_vmax{std::numeric_limits<f64>::min()}
    , m_samples{100}
    {}
    virtual ~RootsFinder() = default;

    virtual Vec<f64> roots(const std::string& var, f64 vmin=-10, f64 vmax=10, int samples=100) const = 0;
    void update_roots_config(const std::string& var, f64 vmin, f64 vmax, int samples){
        this->m_var = var;
        this->m_vmin = vmin;
        this->m_vmax = vmax;
        this->m_samples = samples;
    }

private:
    const ExprBase* m_expr;
    std::string m_var;
    f64 m_vmin;
    f64 m_vmax;
    int m_samples;

    friend class Creola;
};


// -*- Simplifier, Factorizer, Expander, Integrator, Differentiator, Series,
struct ExprBase:
    Simplifier, Factorizer, Expander, Integrator,
    Differentiator, Series, LimitFinder, RootsFinder,
    std::enable_shared_from_this<ExprBase> {
    explicit ExprBase(ExprKind kind)
    : Simplifier(this)
    , Factorizer(this) 
    , Expander(this)
    , Integrator(this)
    , Differentiator(this)
    , Series(this)
    , LimitFinder(this)
    , RootsFinder(this)
    , m_kind{kind}{}
    virtual ~ExprBase() = default;

    virtual f64 eval( const std::string& var, f64 val) const = 0;
    virtual void print(PrintVisitor& visitor, std::ostream& os, [[maybe_unused]] int prec) const = 0;

    
    // virtual void print_unicode(std::ostream& os, int prec=0) const = 0;

    // virtual Expr groebner(const Vec<Expr>& exprs) const = 0;
    /* virtual Box to_box(void) const = 0;
    virtual Box to_box_prec(int parent_prec) const = 0;

    int precedence(void) const{
        switch (m_kind){
        case ExprKind::Number:
        case ExprKind::Symbol:
            //return 4;
        case ExprKind::FuncCall:
            return 4;
        case ExprKind::Pow:
            return 3;
        case ExprKind::Mul:
            return 2;
        case ExprKind::Add:
        case ExprKind::Neg:
            return 1;
        }

        return 0;
    } */

    ExprKind kind(void) const { return this->m_kind; }

protected:
    ExprKind m_kind;
};

// -*-
struct Number : public ExprBase {
    explicit Number(f64 val)
    : ExprBase{ExprKind::NUM}, m_value{val}{}

    Expr simplify(void) const override;
    Expr diff(const std::string& var) const override;
    Expr integrate(const std::string& var) const override;
    Expr expand(void) const override;
    Expr factorize(const std::string& var) const override;
    Expr taylor(const std::string& var, f64 val, int n) const override;
    Vec<f64> roots(const std::string& var, f64 vmin=-10, f64 vmax=10, int samples=100) const override;
    f64 limit(const std::string& var, f64 val, f64 eps=1e-6) const override;

    f64 eval(const std::string& var, f64 val) const override;

    // -*-
    void print(PrintVisitor& visitor, std::ostream& os, [[maybe_unused]] int prec) const override{
        visitor.visit(os, *this, prec);
    }

    // Expr groebner(const Vec<Expr>& exprs) const override;
    // Box to_box(void) const override;
    // Box to_box_prec(int parent_prec) const override;

    const f64& value(void) const{ return this->m_value; }
    f64& value(void){ return this->m_value; }

private:
    f64 m_value;
};


// -*-
struct Symbol : public ExprBase{
    explicit Symbol(const std::string& name)
    : ExprBase{ExprKind::SYM}, m_name{std::move(name)} {}

    Expr simplify(void) const override;
    Expr diff(const std::string& var) const override;
    Expr integrate(const std::string& var) const override;
    Expr expand(void) const override;
    Expr factorize(const std::string& var) const override;
    Expr taylor(const std::string& var, f64 val, int n) const override;
    Vec<f64> roots(const std::string& var, f64 vmin=-10, f64 vmax=10, int samples=100) const override;
    f64 limit(const std::string& var, f64 val, f64 eps=1e-6) const override;

    f64 eval(const std::string& var, f64 val) const override;

    // -*-
    void print(PrintVisitor& visitor, std::ostream& os, [[maybe_unused]] int prec) const override{
        visitor.visit(os, *this, prec);
    }

    // Expr groebner(const Vec<Expr>& exprs) const override;
    // Box to_box(void) const override;
    // Box to_box_prec(int parent_prec) const override;

    std::string& name(void){ return this->m_name; }
    const std::string& name(void) const { return this->m_name; }

private:
    std::string m_name;
};

// -*-
struct Neg : public ExprBase {
    explicit Neg(Expr expr)
    : ExprBase{ExprKind::NEG}, m_rhs{std::move(expr)}{}

    Expr simplify(void) const override;
    Expr diff(const std::string& var) const override;
    Expr integrate(const std::string& var) const override;
    Expr expand(void) const override;
    Expr factorize(const std::string& var) const override;
    Expr taylor(const std::string& var, f64 val, int n) const override;
    Vec<f64> roots(const std::string& var, f64 vmin=-10, f64 vmax=10, int samples=100) const override;
    f64 limit(const std::string& var, f64 val, f64 eps=1e-6) const override;

    f64 eval(const std::string& var, f64 val) const override;

    // -*-
    void print(PrintVisitor& visitor, std::ostream& os, [[maybe_unused]] int prec) const override{
        visitor.visit(os, *this, prec);
    }

    // Expr groebner(const Vec<Expr>& exprs) const override;
    // Box to_box(void) const override;
    // Box to_box_prec(int parent_prec) const override;

    Expr& rhs(void){ return this->m_rhs; }
    const Expr& rhs(void) const { return this->m_rhs; }

private:
    Expr m_rhs;
};

// -*-
struct Add : public ExprBase{
    explicit Add(Vec<Expr> exprs)
    : ExprBase{ExprKind::ADD}
    , m_terms{std::move(exprs)}{}

    Expr simplify(void) const override;
    Expr diff(const std::string& var) const override;
    Expr integrate(const std::string& var) const override;
    Expr expand(void) const override;
    Expr factorize(const std::string& var) const override;
    Expr taylor(const std::string& var, f64 val, int n) const override;
    Vec<f64> roots(const std::string& var, f64 vmin=-10, f64 vmax=10, int samples=100) const override;
    f64 limit(const std::string& var, f64 val, f64 eps=1e-6) const override;

    f64 eval(const std::string& var, f64 val) const override;

    // -*-
    void print(PrintVisitor& visitor, std::ostream& os, [[maybe_unused]] int prec) const override{
        visitor.visit(os, *this, prec);
    }

    // Expr groebner(const Vec<Expr>& exprs) const override;
    // Box to_box(void) const override;
    // Box to_box_prec(int parent_prec) const override;

    Vec<Expr>& terms(void){ return this->m_terms; }
    const Vec<Expr>& terms(void) const { return this->m_terms; }

private:
    Vec<Expr> m_terms;
};

// -*-
struct Mul : public ExprBase{
    explicit Mul(Vec<Expr> exprs)
    : ExprBase{ExprKind::MUL}
    , m_factors{exprs}{}

    Expr simplify(void) const override;
    Expr diff(const std::string& var) const override;
    Expr integrate(const std::string& var) const override;
    Expr expand(void) const override;
    Expr factorize(const std::string& var) const override;
    Expr taylor(const std::string& var, f64 val, int n) const override;
    Vec<f64> roots(const std::string& var, f64 vmin=-10, f64 vmax=10, int samples=100) const override;
    f64 limit(const std::string& var, f64 val, f64 eps=1e-6) const override;

    f64 eval(const std::string& var, f64 val) const override;

    // -*-
    void print(PrintVisitor& visitor, std::ostream& os, [[maybe_unused]] int prec) const override{
        visitor.visit(os, *this, prec);
    }

    // Expr groebner(const Vec<Expr>& exprs) const override;
    // Box to_box(void) const override;
    // Box to_box_prec(int parent_prec) const override;

    Vec<Expr>& factors(void){ return this->m_factors; }
    const Vec<Expr>& factors(void) const { return this->m_factors; }

private:
    Vec<Expr> m_factors;
};

// -*-
struct Pow : public ExprBase {
    explicit Pow(Expr b, Expr e)
    : ExprBase{ExprKind::POW}
    , m_base{std::move(b)}
    , m_expo{std::move(e)}{}

    Expr simplify(void) const override;
    Expr diff(const std::string& var) const override;
    Expr integrate(const std::string& var) const override;
    Expr expand(void) const override;
    Expr factorize(const std::string& var) const override;
    Expr taylor(const std::string& var, f64 val, int n) const override;
    Vec<f64> roots(const std::string& var, f64 vmin=-10, f64 vmax=10, int samples=100) const override;
    f64 limit(const std::string& var, f64 val, f64 eps=1e-6) const override;

    f64 eval(const std::string& var, f64 val) const override;

    // -*-
    void print(PrintVisitor& visitor, std::ostream& os, [[maybe_unused]] int prec) const override{
        visitor.visit(os, *this, prec);
    }

    // Expr groebner(const Vec<Expr>& exprs) const override;
    // Box to_box(void) const override;
    // Box to_box_prec(int parent_prec) const override;

    Expr& base(void){ return this->m_base; }
    Expr& expo(void){ return this->m_expo; }
    const Expr& base(void) const{ return this->m_base; }
    const Expr& expo(void) const{ return this->m_expo; }

private:
    Expr m_base;
    Expr m_expo;
};

// -*-
struct FuncCall : public ExprBase{
    explicit FuncCall(std::string n, Vec<Expr> a)
    : ExprBase{ExprKind::CALL}
    , m_name{std::move(n)}
    , m_args{std::move(a)}{}

    Expr simplify(void) const override;
    Expr diff(const std::string& var) const override;
    Expr integrate(const std::string& var) const override;
    Expr expand(void) const override;
    Expr factorize(const std::string& var) const override;
    Expr taylor(const std::string& var, f64 val, int n) const override;
    Vec<f64> roots(const std::string& var, f64 vmin=-10, f64 vmax=10, int samples=100) const override;
    f64 limit(const std::string& var, f64 val, f64 eps=1e-6) const override;

    f64 eval(const std::string& var, f64 val) const override;

    // -*-
    void print(PrintVisitor& visitor, std::ostream& os, [[maybe_unused]] int prec) const override{
        visitor.visit(os, *this, prec);
    }

    // Expr groebner(const Vec<Expr>& exprs) const override;
    // Box to_box(void) const override;
    // Box to_box_prec(int parent_prec) const override;

    std::string& name(void){ return this->m_name; }
    Vec<Expr>& args(void){ return this->m_args; }
    const std::string& name(void) const{ return this->m_name; }
    const Vec<Expr>& args(void) const{ return this->m_args; }

private:
    std::string m_name;
    Vec<Expr> m_args;
};


// ------------------------
// -*- Helper functions -*-
// ------------------------
std::ostream& operator<<(std::ostream& os, const Number& rhs);
std::ostream& operator<<(std::ostream& os, const Symbol& rhs);
std::ostream& operator<<(std::ostream& os, const Neg& rhs);
std::ostream& operator<<(std::ostream& os, const Add& rhs);
std::ostream& operator<<(std::ostream& os, const Mul& rhs);
std::ostream& operator<<(std::ostream& os, const Pow& rhs);
std::ostream& operator<<(std::ostream& os, const FuncCall& rhs);


// // -*-
// HashMap<std::string, Expr> VARS;
// HashMap<std::string, FunctionDef> FUNCTIONS;

// Expr substitute(const Expr& expr, const std::string& var, const Expr& val);
// Expr apply_user_fun(const std::string& name, const Vec<Expr>& args);

// // helper
// bool is_fraction(const Expr& expr, Expr& numerator, Expr& denominator);


// enum class LimitKind {
//     Finite, PosInf, NegInf, NaN,
// };

// LimitKind classify_value(f64 val);
// LimitKind classify_at(const Expr& expr, const std::string& var, f64 val);

// // L'Hôpital-based limit
// Expr limit_symbolic(const Expr& expr, const std::string& var, f64 val, int max_iter=5);


// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola::core                                 -*-
// -*----------------------------------------------------------------*-