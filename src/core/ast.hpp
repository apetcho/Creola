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

struct Number final : public ExprBase {
    Value value;
    Expr eval(EvalVisitor& visitor, Env& env) const override;
};

struct Negate final : public ExprBase {
    Expr arg;
    Expr eval(EvalVisitor& visitor, Env& env) const override;
};

struct Add final : public ExprBase {
    Expr lhs;
    Expr rhs;
    Expr eval(EvalVisitor& visitor, Env& env) const override;
};

struct Sub final : public ExprBase {
    Expr lhs;
    Expr rhs;
    Expr eval(EvalVisitor& visitor, Env& env) const override;
};

struct Mul final : public ExprBase {
    Expr lhs;
    Expr rhs;
    Expr eval(EvalVisitor& visitor, Env& env) const override;
};

struct Div final : public ExprBase {
    Expr lhs;
    Expr rhs;
    Expr eval(EvalVisitor& visitor, Env& env) const override;
};




// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola                                       -*-
// -*----------------------------------------------------------------*-