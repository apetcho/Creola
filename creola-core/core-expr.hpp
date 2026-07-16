#pragma once

#include "common.hpp"
#include "pprint.hpp"

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
namespace creola{
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

// -*- Display::display -*-
struct Display{
    explicit Display() = default;
    virtual ~Display() = default;
    virtual void display(std::ostream& os, int prec=0) const = 0;
};

// -* Simplifier::simplify -*-
struct Simplifier: Display{
    explicit Simplifier(const ExprBase* expr)
    : m_expr{expr} {}
    virtual ~Simplifier() = default;

    virtual Expr simplify(void) const = 0;

private:
    const ExprBase* m_expr;
};

// -*- Factorizer::factor -*-
struct Factorizer: Display{
    explicit Factorizer(const ExprBase* expr): m_expr{expr}
    {}

    virtual ~Factorizer() = default;

    virtual Expr factor(const std::string& var) const = 0;

private:
    const ExprBase* m_expr;
};

// -*- Expander::expand -*-
struct Expander: Display{
    explicit Expander(const ExprBase* expr): m_expr{expr}
    {}

    virtual ~Expander() = default;

    virtual Expr expand(void) const = 0;

private:
    const ExprBase* m_expr;
};

// -*- Integrator::integrate -*-
struct Integrator: Display {
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
};

// -*- Differentiator::diff -*-
struct Differentiator: Display{
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
};

// -*- Series::taylor -*-
struct Series: Display{
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
};

// -*- LimitFinder::limit -*-
struct LimitFinder: Display{
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
};

// -*- RootsFinder::roots -*-
struct RootsFinder: Display {
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
};


enum class ExprKind {
    Number, Symbol, Add, Mul, Pow, Neg, FuncCall,
};

// -*- Simplifier, Factorizer, Expander, Integrator, Differentiator, Series,
struct ExprBase:
    Simplifier, Factorizer, Expander, Integrator, Differentiator, Series,
    LimitFinder, RootsFinder,
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

    virtual f64 eval(const std::string& var, f64 val) const = 0;

    
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


protected:
    ExprKind m_kind;

    virtual void display(const ExprBase& expr, std::ostream& os, int prec) const = 0;
};

// -*-
struct Number : public ExprBase{
    f64 value;
    explicit Number(f64 val)
    : ExprBase{ExprKind::Number}, value{val}{}

    Expr simplify(void) const override;
    Expr diff(const std::string& var) const override;
    Expr integrate(const std::string& var) const override;
    Expr expand(void) const override;
    Expr factor(const std::string& var) const override;
    Expr taylor(const std::string& var, f64 val, int n) const override;
    Vec<f64> roots(const std::string& var, f64 vmin=-10, f64 vmax=10, int samples=100) const override;
    f64 limit(const std::string& var, f64 val, f64 eps=1e-6) const override;

    f64 eval(const std::string& var, f64 val) const override;

    void display(std::ostream& os, int prec=0) const override{
        CREOLA_UNUSED(prec);
        os << *this;
    }

    // Expr groebner(const Vec<Expr>& exprs) const override;
    // Box to_box(void) const override;
    // Box to_box_prec(int parent_prec) const override;

protected:
    void display(const ExprBase& expr, std::ostream& os, int prec) const override;
};


// -*-
struct Symbol : public ExprBase{
    std::string name;
    explicit Symbol(const std::string& name)
    : ExprBase{ExprKind::Symbol}, name{std::move(name)} {}

    Expr simplify(void) const override;
    Expr diff(const std::string& var) const override;
    Expr integrate(const std::string& var) const override;
    Expr expand(void) const override;
    Expr factor(const std::string& var) const override;
    Expr taylor(const std::string& var, f64 val, int n) const override;
    Vec<f64> roots(const std::string& var, f64 vmin=-10, f64 vmax=10, int samples=100) const override;
    f64 limit(const std::string& var, f64 val, f64 eps=1e-6) const override;

    f64 eval(const std::string& var, f64 val) const override;

    void display(std::ostream& os, int prec=0) const override{
        CREOLA_UNUSED(prec);
        os << *this;
    }

    // Expr groebner(const Vec<Expr>& exprs) const override;
    // Box to_box(void) const override;
    // Box to_box_prec(int parent_prec) const override;
};

// -*-
struct Neg : ExprBase {
    Expr arg;

    explicit Neg(Expr expr): ExprBase{ExprKind::Neg}, arg{std::move(expr)}{}

    Expr simplify(void) const override;
    Expr diff(const std::string& var) const override;
    Expr integrate(const std::string& var) const override;
    Expr expand(void) const override;
    Expr factor(const std::string& var) const override;
    Expr taylor(const std::string& var, f64 val, int n) const override;
    Vec<f64> roots(const std::string& var, f64 vmin=-10, f64 vmax=10, int samples=100) const override;
    f64 limit(const std::string& var, f64 val, f64 eps=1e-6) const override;

    f64 eval(const std::string& var, f64 val) const override;

    void display(std::ostream& os, int prec=0) const override{
        CREOLA_UNUSED(prec);
        os << "-" << *this;
    }

    // Expr groebner(const Vec<Expr>& exprs) const override;
    // Box to_box(void) const override;
    // Box to_box_prec(int parent_prec) const override;
};

// -*-
struct Add : public ExprBase{
    std::vector<Expr> terms;

    explicit Add(std::vector<Expr> exprs)
    : ExprBase{ExprKind::Add}, terms{std::move(exprs)}{}

    Expr simplify(void) const override;
    Expr diff(const std::string& var) const override;
    Expr integrate(const std::string& var) const override;
    Expr expand(void) const override;
    Expr factor(const std::string& var) const override;
    Expr taylor(const std::string& var, f64 val, int n) const override;
    Vec<f64> roots(const std::string& var, f64 vmin=-10, f64 vmax=10, int samples=100) const override;
    f64 limit(const std::string& var, f64 val, f64 eps=1e-6) const override;

    f64 eval(const std::string& var, f64 val) const override;

    void display(std::ostream& os, int prec=0) const override{
        CREOLA_UNUSED(prec);
        os << *this;
        /* for(size_t i=0; i < this->terms.size(); ++i){
            if (i > 0){
                os << " + ";
            }
            this->terms[i]->display(os, 1);
        } */
    }

    // Expr groebner(const Vec<Expr>& exprs) const override;
    // Box to_box(void) const override;
    // Box to_box_prec(int parent_prec) const override;
};

// -*-
struct Mul : public ExprBase{
    std::vector<Expr> factors;

    explicit Mul(std::vector<Expr> exprs)
    : ExprBase{ExprKind::Mul}, factors{std::move(exprs)}{}

    Expr simplify(void) const override;
    Expr diff(const std::string& var) const override;
    Expr integrate(const std::string& var) const override;
    Expr expand(void) const override;
    Expr factor(const std::string& var) const override;
    Expr taylor(const std::string& var, f64 val, int n) const override;
    Vec<f64> roots(const std::string& var, f64 vmin=-10, f64 vmax=10, int samples=100) const override;
    f64 limit(const std::string& var, f64 val, f64 eps=1e-6) const override;

    f64 eval(const std::string& var, f64 val) const override;

    void display(std::ostream& os, int prec=0) const override{
        CREOLA_UNUSED(prec);
        os << *this;
        /* for(size_t i=0; i < this->factors.size(); ++i){
            if(i > 0){ os << "*"; } // or just "" for implicit multiplication
            this->factors[i]->display(os, 2);
        } */
    }

    // Expr groebner(const Vec<Expr>& exprs) const override;
    // Box to_box(void) const override;
    // Box to_box_prec(int parent_prec) const override;
};

// -*-
struct Pow : public ExprBase {
    Expr base;
    Expr expo;

    explicit Pow(Expr b, Expr e)
    : ExprBase{ExprKind::Pow}, base{std::move(b)}, expo{std::move(e)}{}

    Expr simplify(void) const override;
    Expr diff(const std::string& var) const override;
    Expr integrate(const std::string& var) const override;
    Expr expand(void) const override;
    Expr factor(const std::string& var) const override;
    Expr taylor(const std::string& var, f64 val, int n) const override;
    Vec<f64> roots(const std::string& var, f64 vmin=-10, f64 vmax=10, int samples=100) const override;
    f64 limit(const std::string& var, f64 val, f64 eps=1e-6) const override;

    f64 eval(const std::string& var, f64 val) const override;

    void display(std::ostream& os, int prec=0) const override{
        CREOLA_UNUSED(prec);
        os << *this;
        /* this->base->display(os, 3);
        os << "^";
        this->expo->display(os, 3); */
    }

    // Expr groebner(const Vec<Expr>& exprs) const override;
    // Box to_box(void) const override;
    // Box to_box_prec(int parent_prec) const override;
};

// -*-
struct FuncCall : ExprBase{
    std::string name;
    Vec<Expr> args;

    explicit FuncCall(std::string n, Vec<Expr> a)
    : ExprBase{ExprKind::FuncCall}, name{std::move(n)}, args{std::move(a)}{}

    Expr simplify(void) const override;
    Expr diff(const std::string& var) const override;
    Expr integrate(const std::string& var) const override;
    Expr expand(void) const override;
    Expr factor(const std::string& var) const override;
    Expr taylor(const std::string& var, f64 val, int n) const override;
    Vec<f64> roots(const std::string& var, f64 vmin=-10, f64 vmax=10, int samples=100) const override;
    f64 limit(const std::string& var, f64 val, f64 eps=1e-6) const override;

    f64 eval(const std::string& var, f64 val) const override;
    void display(std::ostream& os, int prec=0) const override{
        CREOLA_UNUSED(prec);
        os << *this;

        /* os << this->name << "(";
        for(size_t i=0; i < args.size(); ++i){
            if(i > 0){ os << ", "; }
            this->args[i]->display(os, 0);
        }
        os << ")"; */
    }

    // Expr groebner(const Vec<Expr>& exprs) const override;
    // Box to_box(void) const override;
    // Box to_box_prec(int parent_prec) const override;
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


// -*-
struct FuncionDef{
    std::string param; // univariate function
    Expr body;
};

// -*-
class Creola final {
public:
    // - handle_line()
    void run(const std::string& src);
    // - parse_expression_only()
    Expr parse(const std::string& src);

private:
    // user-defined variables & functions
    HashMap<std::string, Expr> m_vars;
    HashMap<std::string, FuncionDef> m_funcs;

    // -
    Expr substitute(const Expr& expr, const std::string& var, const Expr& val);
    // apply_user_func ==> apply 
    Expr apply(const std::string& name, const Vec<Expr>& args);

public:
    static std::unordered_map<std::string, UnaryMathFun> UNARY_MATH_FUNCTIONS;
    static std::unordered_map<std::string, Func> COMMON_DIFF_TABLE;
    static std::unordered_map<std::string, Func> COMMON_INTEGRATION_TABLE;

    static inline Expr number(f64 val){
        return std::make_shared<Number>(val);
    }

    static inline Expr symbol(const std::string& var){
        return std::make_shared<Symbol>(var);
    }

    static inline bool is_zero(const Expr& expr){
        auto num = std::dynamic_pointer_cast<Number>(expr);
        return (num && std::fabsl(num->value==0.0L));
    }

    static inline bool is_one(const Expr& expr){
        auto num = std::dynamic_pointer_cast<Number>(expr);
        return (num && std::fabsl(num->value - 1.0L)==0.0L);
    }

    // -----------------------------------
    // -*- High level helper functions -*-
    // -----------------------------------
    static Expr simplify(const Expr& expr);
    static Expr diff(const Expr& expr, const std::string& var);
    static Expr expand(const Expr& expr);
    static Expr factor(const Expr& expr, const std::string& var);
    static Expr integrate(const Expr& expr, const std::string& var);
    static f64 eval(const Expr& expr, const std::string& var, f64 val);
    static Vec<f64> roots(const Expr& expr, const std::string& var, f64 vmin=-10, f64 vmax=10, int samples=200);
    static Expr taylor(const Expr& expr, const std::string& var, f64 val, int n);
    static f64 limit(const Expr& expr, const std::string& var, f64 val, f64 eps=1e-6);


private:
    static inline Expr sin(f64 x){ return number(std::sin(x)); }
    static inline Expr cos(f64 x){ return number(std::cos(x)); }
    static inline Expr tan(f64 x){ return number(std::tan(x)); }
    static inline Expr asin(f64 x){ return number(std::asin(x)); }
    static inline Expr acos(f64 x){ return number(std::acos(x)); }
    static inline Expr atan(f64 x){ return number(std::atan(x)); }

    static inline Expr sinh(f64 x){ return number(std::sinh(x)); }
    static inline Expr cosh(f64 x){ return number(std::cosh(x)); }
    static inline Expr tanh(f64 x){ return number(std::tanh(x)); }
    static inline Expr asinh(f64 x){ return number(std::asinh(x)); }
    static inline Expr acosh(f64 x){ return number(std::acosh(x)); }
    static inline Expr atanh(f64 x){ return number(std::atanh(x)); }

    static inline Expr sqrt(f64 x){ return number(std::sqrt(x)); }
    static inline Expr cbrt(f64 x){ return number(std::cbrt(x)); }
    static inline Expr exp(f64 x){ return number(std::exp(x)); }
    static inline Expr ln(f64 x){ return number(std::log(x)); }

    // -*---------------------------*-
    // -*- Trigonometric functions -*-
    // -*---------------------------*-
    // -*- Sin, Cos, Tan, Asin, Acos, Atan -*-
    static Expr diff_sin(Expr expr, const std::string& var);
    static Expr integrate_sin(Expr expr, const std::string& var);

    static Expr diff_cos(Expr expr, const std::string& var);
    static Expr integrate_cos(Expr expr, const std::string& var);

    static Expr diff_tan(Expr expr, const std::string& var);
    static Expr integrate_tan(Expr expr, const std::string& var);

    static Expr diff_asin(Expr expr, const std::string& var);
    static Expr integrate_asin(Expr expr, const std::string& var);

    static Expr diff_acos(Expr expr, const std::string& var);
    static Expr integrate_acos(Expr expr, const std::string& var);

    static Expr diff_atan(Expr expr, const std::string& var);
    static Expr integrate_atan(Expr expr, const std::string& var);
    
    // Hyperbolic functions
    // -*- Sinh, Cosh, Tanh, Asinh, Acosh, Atanh -*-
    static Expr diff_sinh(Expr expr, const std::string& var);
    static Expr integrate_sinh(Expr expr, const std::string& var);
    static Expr diff_cosh(Expr expr, const std::string& var);
    static Expr integrate_cosh(Expr expr, const std::string& var);
    static Expr diff_tanh(Expr expr, const std::string& var);
    static Expr integrate_tanh(Expr expr, const std::string& var);
    static Expr diff_asinh(Expr expr, const std::string& var);
    static Expr integrate_asinh(Expr expr, const std::string& var);
    static Expr diff_acosh(Expr expr, const std::string& var);
    static Expr integrate_acosh(Expr expr, const std::string& var);
    static Expr diff_atanh(Expr expr, const std::string& var);
    static Expr integrate_atanh(Expr expr, const std::string& var);


    // Power functions
    // -*- Pow, Sqrt, Cbrt, Exp -*-
    static Expr diff_pow(Expr expr, const std::string& var);
    static Expr integrate_pow(Expr expr, const std::string& var);
    static Expr diff_sqrt(Expr expr, const std::string& var);
    static Expr integrate_sqrt(Expr expr, const std::string& var);
    static Expr diff_cbrt(Expr expr, const std::string& var);
    static Expr integrate_cbrt(Expr expr, const std::string& var);
    static Expr diff_exp(Expr expr, const std::string& var);
    static Expr integrate_exp(Expr expr, const std::string& var);
    static Expr diff_ln(Expr expr, const std::string& var);
    static Expr integrate_ln(Expr expr, const std::string& var);
};

// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola.                                      -*-
// -*----------------------------------------------------------------*-