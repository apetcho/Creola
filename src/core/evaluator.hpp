#pragma once

#include "ast.hpp"
#include "env.hpp"
#include<cmath>
#include<complex>
#include<functional>


// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola                                     -*-
// -*----------------------------------------------------------------*-
namespace creola{
// -

struct EvalVisitor{
    virtual ~EvalVisitor() = default;
    virtual Expr eval(const Symbol& sym, Env& env) const = 0;
    virtual Expr eval(const Number& num, Env& env) const = 0;
    virtual Expr eval(const Negate& negate, Env& env) const = 0;
    virtual Expr eval(const Add& add, Env& env) const = 0;
    virtual Expr eval(const Sub& sub, Env& env) const = 0;
    virtual Expr eval(const Mul& mul, Env& env) const = 0;
    virtual Expr eval(const Div& div, Env& env) const = 0;
    virtual Expr eval(const Pow& pow, Env& env) const = 0;
    virtual Expr eval(const Call& call, Env& env) const = 0;
    virtual Expr eval(const Lambda& lambda, Env& env) const = 0;
};

struct ExecuteVisitor {
    virtual ~ExecuteVisitor() = default;
    virtual void execute(const Let& let, Env& env) const = 0;
    virtual void execute(const Fun& fun, Env& env) const = 0;
};

class Interpreter final : public EvalVisitor, public ExecuteVisitor {
public:
    Expr eval(const Symbol& sym, Env& env) const override;
    Expr eval(const Number& num, Env& env) const override;
    Expr eval(const Negate& negate, Env& env) const override;
    Expr eval(const Add& add, Env& env) const override;
    Expr eval(const Sub& sub, Env& env) const override;
    Expr eval(const Mul& mul, Env& env) const override;
    Expr eval(const Div& div, Env& env) const override;
    Expr eval(const Pow& pow, Env& env) const override;
    Expr eval(const Call& call, Env& env) const override;
    Expr eval(const Lambda& lambda, Env& env) const override;

    void execute(const Let& let, Env& env) const override;
    void execute(const Fun& fun, Env& env) const override;

private:
    Value applyBinaryOp(Value lhs, Value rhs, std::function<double(double, double)> op);

};

// -*-
static inline bool is_symbol(Expr expr){
    return std::dynamic_pointer_cast<Symbol>(expr) != nullptr;
}

// -*-
static inline bool is_number(Expr expr){
    return std::dynamic_pointer_cast<Number>(expr) != nullptr;
}

// -*-
static inline bool is_negate(Expr expr){
    return std::dynamic_pointer_cast<Negate>(expr) != nullptr;
}

// -*-
static inline bool is_add(Expr expr){
    return std::dynamic_pointer_cast<Add>(expr) != nullptr;
}

static inline bool is_sub(Expr expr){
    return std::dynamic_pointer_cast<Sub>(expr) != nullptr;
}

static inline bool is_mul(Expr expr){
    return std::dynamic_pointer_cast<Mul>(expr) != nullptr;
}

static inline bool is_div(Expr expr){
    return std::dynamic_pointer_cast<Div>(expr) != nullptr;
}

static inline bool is_pow(Expr expr){
    return std::dynamic_pointer_cast<Pow>(expr) != nullptr;
}

static inline bool is_call(Expr expr){
    return std::dynamic_pointer_cast<Call>(expr) != nullptr;
}


// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola                                       -*-
// -*----------------------------------------------------------------*-