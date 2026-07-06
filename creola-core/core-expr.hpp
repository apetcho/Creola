#pragma once

#include "common.hpp"
#include "pprint.hpp"

#include<unordered_map>
#include<iostream>
#include<memory>
#include<vector>
#include<string>


// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola                                     -*-
// -*----------------------------------------------------------------*-
namespace creola{
//
struct ExprBase;
using Expr = Shared<ExprBase>;

enum class ExprKind {
    Number, Symbol, Add, Mul, Pow, Neg, FuncCall,
};

struct ExprBase : std::enable_shared_from_this<ExprBase> {
    explicit ExprBase(ExprKind kind): m_kind{kind}{}
    virtual ~ExprBase() = default;

    virtual Expr simplify(void) const = 0;
    virtual Expr diff(const std::string& var) const = 0;
    virtual Expr integrate(const std::string& var) const = 0;
    virtual Expr expand(void) const = 0;
    virtual Expr factor(const std::string& var) const = 0;
    virtual Expr groebner(const Vec<Expr>& exprs) const = 0;
    virtual Expr taylor(const std::string& var, f64 val, int n) const = 0;
    virtual Vec<f64> roots(const std::string& var, f64 vmin=-10, f64 vmax=10, int samples=100) const = 0;
    virtual f64 limit(const std::string& var, f64 val, f64 eps=1e-6) const = 0;

    virtual f64 eval(const std::string& var, f64 val) const = 0;

    virtual Box to_box(void) const = 0;
    virtual Box to_box_prec(int parent_prec) const = 0;
    virtual void print_unicode(std::ostream& os, int prec=0) const = 0;

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
    }

protected:
    ExprKind m_kind;
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
    Expr groebner(const Vec<Expr>& exprs) const override;
    Expr taylor(const std::string& var, f64 val, int n) const override;
    Vec<f64> roots(const std::string& var, f64 vmin=-10, f64 vmax=10, int samples=100) const override;
    f64 limit(const std::string& var, f64 val, f64 eps=1e-6) const override;

    f64 eval(const std::string& var, f64 val) const override;

    Box to_box(void) const override;
    Box to_box_prec(int parent_prec) const override;
    void print_unicode(std::ostream& os, int prec=0) const override;
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
    Expr groebner(const Vec<Expr>& exprs) const override;
    Expr taylor(const std::string& var, f64 val, int n) const override;
    Vec<f64> roots(const std::string& var, f64 vmin=-10, f64 vmax=10, int samples=100) const override;
    f64 limit(const std::string& var, f64 val, f64 eps=1e-6) const override;

    f64 eval(const std::string& var, f64 val) const override;

    Box to_box(void) const override;
    Box to_box_prec(int parent_prec) const override;
    void print_unicode(std::ostream& os, int prec=0) const override;
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
    Expr groebner(const Vec<Expr>& exprs) const override;
    Expr taylor(const std::string& var, f64 val, int n) const override;
    Vec<f64> roots(const std::string& var, f64 vmin=-10, f64 vmax=10, int samples=100) const override;
    f64 limit(const std::string& var, f64 val, f64 eps=1e-6) const override;

    f64 eval(const std::string& var, f64 val) const override;

    Box to_box(void) const override;
    Box to_box_prec(int parent_prec) const override;
    void print_unicode(std::ostream& os, int prec=0) const override;
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
    Expr groebner(const Vec<Expr>& exprs) const override;
    Expr taylor(const std::string& var, f64 val, int n) const override;
    Vec<f64> roots(const std::string& var, f64 vmin=-10, f64 vmax=10, int samples=100) const override;
    f64 limit(const std::string& var, f64 val, f64 eps=1e-6) const override;

    f64 eval(const std::string& var, f64 val) const override;

    Box to_box(void) const override;
    Box to_box_prec(int parent_prec) const override;
    void print_unicode(std::ostream& os, int prec=0) const override;
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
    Expr groebner(const Vec<Expr>& exprs) const override;
    Expr taylor(const std::string& var, f64 val, int n) const override;
    Vec<f64> roots(const std::string& var, f64 vmin=-10, f64 vmax=10, int samples=100) const override;
    f64 limit(const std::string& var, f64 val, f64 eps=1e-6) const override;

    f64 eval(const std::string& var, f64 val) const override;

    Box to_box(void) const override;
    Box to_box_prec(int parent_prec) const override;
    void print_unicode(std::ostream& os, int prec=0) const override;
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
    Expr groebner(const Vec<Expr>& exprs) const override;
    Expr taylor(const std::string& var, f64 val, int n) const override;
    Vec<f64> roots(const std::string& var, f64 vmin=-10, f64 vmax=10, int samples=100) const override;
    f64 limit(const std::string& var, f64 val, f64 eps=1e-6) const override;

    f64 eval(const std::string& var, f64 val) const override;

    Box to_box(void) const override;
    Box to_box_prec(int parent_prec) const override;
    void print_unicode(std::ostream& os, int prec=0) const override;
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
    Expr groebner(const Vec<Expr>& exprs) const override;
    Expr taylor(const std::string& var, f64 val, int n) const override;
    Vec<f64> roots(const std::string& var, f64 vmin=-10, f64 vmax=10, int samples=100) const override;
    f64 limit(const std::string& var, f64 val, f64 eps=1e-6) const override;

    f64 eval(const std::string& var, f64 val) const override;

    Box to_box(void) const override;
    Box to_box_prec(int parent_prec) const override;
    void print_unicode(std::ostream& os, int prec=0) const override;
};


// ------------------------
// -*- Helper functions -*-
// ------------------------
inline Expr number(f64 val){
    return std::make_shared<Number>(val);
}

inline Expr symbol(const std::string& var){
    return std::make_shared<Symbol>(var);
}

inline bool is_zero(const Expr& expr){
    auto num = std::dynamic_pointer_cast<Number>(expr);
    return (num && std::fabsl(num->value==0.0L));
}

inline bool is_one(const Expr& expr){
    auto num = std::dynamic_pointer_cast<Number>(expr);
    return (num && std::fabsl(num->value - 1.0L)==0.0L);
}

Expr simplify(const Expr& expr);
Expr diff(const Expr& expr);
Expr expand(const Expr& expr);
Expr factor(const Expr& expr, const std::string& var);
Expr integrate(const Expr& expr, const std::string& var);
f64 eval(const Expr& expr, const std::string& var, f64 val);
Vec<f64> roots(const Expr& expr, const std::string& var, f64 vmin=-10, f64 vmax=10, int samples=200);
Expr taylor(const Expr& expr, const std::string& var, f64 val, int n);
f64 limit(const Expr& expr, const std::string& var, f64 val, int max_iter=5);


// -*-
struct FunctionDef{
    std::string param;
    Expr body;
};

// -*-
HashMap<std::string, Expr> VARS;
HashMap<std::string, FunctionDef> FUNCTIONS;

Expr substitute(const Expr& expr, const std::string& var, const Expr& val);
Expr apply_user_fun(const std::string& name, const Vec<Expr>& args);

// helper
bool is_fraction(const Expr& expr, Expr& numerator, Expr& denominator);


enum class LimitKind {
    Finite, PosInf, NegInf, NaN,
};

LimitKind classify_value(f64 val);
LimitKind classify_at(const Expr& expr, const std::string& var, f64 val);

// L'Hôpital-based limit
Expr limit_symbolic(const Expr& expr, const std::string& var, f64 val, int max_iter=5);


// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola                                       -*-
// -*----------------------------------------------------------------*-