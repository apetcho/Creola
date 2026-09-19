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


// -
Expr Negate::eval(EvalVisitor& visitor, Env& env) const {
    return visitor.eval(*this, env);
}

// -
Expr Number::eval(EvalVisitor& visitor, Env& env) const{
    return visitor.eval(*this, env);
}

// -
Expr Add::eval(EvalVisitor& visitor, Env& env) const {
    return visitor.eval(*this, env);
}

// -
Expr Sub::eval(EvalVisitor& visitor, Env& env) const {
    return visitor.eval(*this, env);
}

// -
Expr Mul::eval(EvalVisitor& visitor, Env& env) const {
    return visitor.eval(*this, env);
}

// -
Expr Div::eval(EvalVisitor& visitor, Env& env) const {
    return visitor.eval(*this, env);
}

// -
Expr Pow::eval(EvalVisitor& visitor, Env& env) const {
    return visitor.eval(*this, env);
}

// -
Expr Call::eval(EvalVisitor& visitor, Env& env) const {
    return visitor.eval(*this, env);
}


/*
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