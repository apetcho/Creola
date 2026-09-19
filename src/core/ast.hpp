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
    Expr eval(EvalVisitor& visitor, Env&) const override;
};

struct Number final : public ExprBase {
    Value value;
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

    Call(std::string& name, std::vector<Expr>&& args)
    : name{name}, args{std::move(args)}
    {}

    Expr eval(EvalVisitor& visitor, Env& env) const override;
};

struct Lambda final : public ExprBase {
    std::vector<std::string> params;
    Expr body;

    Lambda(std::vector<std::string>& params, Expr&& body)
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

// -
static inline Stmt makeLet(const std::string& name, Expr expr){
    return std::make_shared<Let>(name, expr);
}

// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola                                       -*-
// -*----------------------------------------------------------------*-