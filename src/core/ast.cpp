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

// -
Expr Lambda::eval(EvalVisitor& visitor, Env& env) const {
    return visitor.eval(*this, env);
}

// ----------------------- Stmt implementations -----------------------

void Let::execute(ExecuteVisitor& visitor, Env& env) const {
    visitor.execute(*this, env);
}

// -
void Fun::execute(ExecuteVisitor& visitor, Env& env) const {
    visitor.execute(*this, env);
}


// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola                                       -*-
// -*----------------------------------------------------------------*-