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
    virtual Value eval(const Symbol& sym, Env& env) const = 0;
    virtual Value eval(const Number& num, Env& env) const = 0;
    virtual Value eval(const Negate& negate, Env& env) const = 0;
    virtual Value eval(const Add& add, Env& env) const = 0;
    virtual Value eval(const Sub& sub, Env& env) const = 0;
    virtual Value eval(const Mul& mul, Env& env) const = 0;
    virtual Value eval(const Div& div, Env& env) const = 0;
    virtual Value eval(const Pow& pow, Env& env) const = 0;
    virtual Value eval(const Call& call, Env& env) const = 0;
    virtual Value eval(const Lambda& lambda, Env& env) const = 0;
};

struct ExecuteVisitor {
    virtual ~ExecuteVisitor() = default;
    virtual void execute(const Let& let, Env& env) const = 0;
    virtual void execute(const Fun& fun, Env& env) const = 0;
};

class Interpreter final : public EvalVisitor, public ExecuteVisitor {
public:
    Value eval(const Symbol& sym, Env& env) const override;
    Value eval(const Number& num, Env& env) const override;
    Value eval(const Negate& negate, Env& env) const override;
    Value eval(const Add& add, Env& env) const override;
    Value eval(const Sub& sub, Env& env) const override;
    Value eval(const Mul& mul, Env& env) const override;
    Value eval(const Div& div, Env& env) const override;
    Value eval(const Pow& pow, Env& env) const override;
    Value eval(const Call& call, Env& env) const override;
    Value eval(const Lambda& lambda, Env& env) const override;

    void execute(const Let& let, Env& env) const override;
    void execute(const Fun& fun, Env& env) const override;

private:
    Value applyBinaryOp(Value lhs, Value rhs, std::function<double(double, double)> op);

};

// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola                                       -*-
// -*----------------------------------------------------------------*-