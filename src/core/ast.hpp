#pragma once

#include<memory>
#include<string>
#include<vector>
#include<variant>
#include<complex>
#include<iostream>


// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola                                     -*-
// -*----------------------------------------------------------------*-
namespace creola{
// -

// Forward declarations
struct ExprBase;
struct StmtBase;
struct Env;
struct EvalVisitor;
struct ExecuteVisitor;

using Expr = std::shared_ptr<ExprBase>;
using Stmt = std::shared_ptr<StmtBase>;
using Value = std::variant<std::int64_t, double, std::complex<double>>;


// --- Abstract AST Base ---
struct ExprBase {
    virtual ~ExprBase() = default;
    virtual Expr eval(EvalVisitor&, Env&) const = 0;
};

struct StmtBase {
    virtual ~StmtBase() = default;
    virtual void execute(ExecuteVisitor&, Env&) const = 0;
};

struct Symbol final : public ExprBase {
    std::string name;

    Symbol(const std::string& name)
    : name{name}
    {}


    Expr eval(EvalVisitor& visitor, Env&) const override;
};

struct Number final : public ExprBase {
    Value value;

    Number()
    : value{std::int64_t(0)}
    {}

    Number(std::int64_t num)
    : value{num}
    {}

    Number(double num)
    : value{num}
    {}

    Number(std::complex<double> z)
    : value{z}
    {}

    Number(const Value& val)
    : value{val}
    {}

    Expr eval(EvalVisitor& visitor, Env& env) const override;
};

struct Negate final : public ExprBase {
    Expr arg;

    Negate(Expr&& arg)
    : arg{std::move(arg)}
    {}

    Expr eval(EvalVisitor& visitor, Env& env) const override;
};

struct Add final : public ExprBase {
    Expr lhs;
    Expr rhs;

    Add(Expr&& left, Expr&& right)
    : lhs{std::move(left)}, rhs(std::move(right))
    {}


    Expr eval(EvalVisitor& visitor, Env& env) const override;
};

struct Sub final : public ExprBase {
    Expr lhs;
    Expr rhs;

    Sub(Expr&& left, Expr&& right)
    : lhs{std::move(left)}, rhs(std::move(right))
    {}


    Expr eval(EvalVisitor& visitor, Env& env) const override;
};

struct Mul final : public ExprBase {
    Expr lhs;
    Expr rhs;

    Mul(Expr&& left, Expr&& right)
    : lhs{std::move(left)}, rhs(std::move(right))
    {}


    Expr eval(EvalVisitor& visitor, Env& env) const override;
};

struct Div final : public ExprBase {
    Expr lhs;
    Expr rhs;

    Div(Expr&& left, Expr&& right)
    : lhs{std::move(left)}, rhs(std::move(right))
    {}

    Expr eval(EvalVisitor& visitor, Env& env) const override;
};

struct Pow final : public ExprBase {
    Expr base;
    Expr expo;

    Pow(Expr&& b, Expr&& e)
    : base{std::move(b)}, expo{std::move(e)}
    {}

    Expr eval(EvalVisitor& visitor, Env& env) const override;
};

struct Call final : public ExprBase {
    std::string name;
    std::vector<Expr> args;

    Call(const std::string& name, std::vector<Expr>&& args)
    : name{name}, args{std::move(args)}
    {}

    Expr eval(EvalVisitor& visitor, Env& env) const override;
};

struct Lambda final : public ExprBase {
    std::vector<std::string> params;
    Expr body;

    Lambda(const std::vector<std::string>& params, Expr&& body)
    : params{params}, body{std::move(body)}
    {}

    Expr eval(EvalVisitor& visitor, Env& env) const override;
};

// --- Concrete Statements ---
struct Let final : public StmtBase {
    std::string name;
    Expr expr;
    Let(const std::string& name, Expr&& e)
    : name{name}, expr{std::move(e)}{}

    void execute(ExecuteVisitor& visitor, Env& env) const override;
};


struct Fun final : public StmtBase {
    std::string name;
    Lambda lambda;

    Fun(const std::string& name, const Lambda& lambda)
    : name{name}, lambda{lambda}
    {}

    void execute(ExecuteVisitor& visitor, Env& env) const override;
};


static inline Expr makeSymbol(const std::string& name){
    return std::make_shared<Symbol>(name);
}

static inline Expr makeNumber(){
    return std::make_shared<Number>();
}

static inline Expr makeNumber(std::int64_t num){
    return std::make_shared<Number>(num);
}

static inline Expr makeNumber(double num){
    return std::make_shared<Number>(num);
}

static inline Expr makeNumber(std::complex<double> z){
    return std::make_shared<Number>(z);
}

static inline Expr makeNumber(const Value& val){
    return std::make_shared<Number>(val);
}

static inline Expr makeNegate(Expr&& arg){
    return std::make_shared<Negate>(std::move(arg));
}

static inline Expr makeBinary(char op, Expr&& lhs, Expr&& rhs){
    if(op=='+'){
        return std::make_shared<Add>(std::move(lhs), std::move(rhs));
    }
    if(op=='-'){
        return std::make_shared<Sub>(std::move(lhs), std::move(rhs));
    }
    if(op=='*'){
        return std::make_shared<Mul>(std::move(lhs), std::move(rhs));
    }
    if(op=='/'){
        return std::make_shared<Div>(std::move(lhs), std::move(rhs));
    }
    if(op=='^'){
        return std::make_shared<Pow>(std::move(lhs), std::move(rhs));
    }
    throw std::runtime_error("unknown binary operator '" + std::string(1, op)+"'");
}

// -
static inline Expr makeLambda(const std::vector<std::string>& params, Expr&& body){
    return std::make_shared<Lambda>(params, std::move(body));
}

// -
static inline Expr makeCall(const std::string& name, std::vector<Expr>&& args){
    return std::make_shared<Call>(name, std::move(args));
}

// -
static inline Stmt makeFun(const std::string& name, const Lambda& lambda){
    return std::make_shared<Fun>(name, lambda);
}

// -
static inline Stmt makeLet(const std::string& name, Expr expr){
    return std::make_shared<Let>(name, expr);
}

// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola                                       -*-
// -*----------------------------------------------------------------*-