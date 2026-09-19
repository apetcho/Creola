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

static inline bool is_lambda(Expr expr){
    return std::dynamic_pointer_cast<Lambda>(expr) != nullptr;
}

static inline Symbol as_symbol(Expr expr){
    if(!is_symbol(expr)){
        throw std::runtime_error("Expected a symbol");
    }
    auto self = std::dynamic_pointer_cast<Symbol>(expr);
    return *self;
}

// -*-
static inline Negate as_negate(Expr expr){
    if(!is_negate(expr)){
        throw std::runtime_error("Expected a negate-expression");
    }
    auto self = std::dynamic_pointer_cast<Negate>(expr);
    return *self;
}

// -*-
static inline Number as_number(Expr expr){
    if(!is_number(expr)){
        throw std::runtime_error("Expected a number");
    }
    auto self = std::dynamic_pointer_cast<Number>(expr);
    return *self;
}

// -*-
static inline Add as_add(Expr expr){
    if(!is_add(expr)){
        throw std::runtime_error("Expected add-expression");
    }
    auto self = std::dynamic_pointer_cast<Add>(expr);
    return *self;
}

// -*-
static inline Sub as_sub(Expr expr){
    if(!is_sub(expr)){
        throw std::runtime_error("Expected sub-expression");
    }
    auto self = std::dynamic_pointer_cast<Sub>(expr);
    return *self;
}

// -*-
static inline Mul as_mul(Expr expr){
    if(!is_mul(expr)){
        throw std::runtime_error("Expected mul-expression");
    }
    auto self = std::dynamic_pointer_cast<Mul>(expr);
    return *self;
}

// -*-
static inline Div as_div(Expr expr){
    if(!is_div(expr)){
        throw std::runtime_error("Expected div-expression");
    }
    auto self = std::dynamic_pointer_cast<Div>(expr);
    return *self;
}

// -*-
static inline Pow as_pow(Expr expr){
    if(!is_pow(expr)){
        throw std::runtime_error("Expected pow-expression");
    }
    auto self = std::dynamic_pointer_cast<Pow>(expr);
    return *self;
}

// -*-
static inline Lambda as_lambda(Expr expr){
    if(!is_negate(expr)){
        throw std::runtime_error("Expected a lambda-expression");
    }
    auto self = std::dynamic_pointer_cast<Lambda>(expr);
    return *self;
}

// -*-
static inline Call as_call(Expr expr){
    if(!is_call(expr)){
        throw std::runtime_error("Expected call-expression");
    }
    auto self = std::dynamic_pointer_cast<Call>(expr);
    return *self;
}


// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola                                       -*-
// -*----------------------------------------------------------------*-