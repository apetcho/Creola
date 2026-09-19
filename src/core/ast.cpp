#include "ast.hpp"
#include "env.hpp"
#include "evaluator.hpp"

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola                                     -*-
// -*----------------------------------------------------------------*-
namespace creola{
// -

// ----------------------- Expr Implementations -----------------------
Expr Symbol::eval(EvalVisitor& visitor, Env& env) const {
    return visitor.eval(*this, env);
}

/*

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

struct Pow final : public ExprBase {
    Expr base;
    Expr expo;
    Expr eval(EvalVisitor& visitor, Env& env) const override;
};

struct Call final : public ExprBase {
    std::string name;
    std::vector<Expr> args;
    Expr eval(EvalVisitor& visitor, Env& env) const override;
};

struct Lambda final : public ExprBase {
    std::vector<std::string> params;
    Expr body;
    Expr eval(EvalVisitor& visitor, Env& env) const override;
};

// --- Concrete Statements ---
struct Let final : public StmtBase {
    std::string name;
    Expr expr;
    void execute(ExecuteVisitor& visitor, Env& env) const override;
};


struct Fun final : public StmtBase {
    std::string name;
    Lambda lambda;
    void execute(ExecuteVisitor& visitor, Env& env) const override;
};

*/

// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola                                       -*-
// -*----------------------------------------------------------------*-