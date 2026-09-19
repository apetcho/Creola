#include "ast.hpp"
#include "evaluator.hpp"

#include<stdexcept>
#include<cmath>


// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola                                     -*-
// -*----------------------------------------------------------------*-
namespace creola{
// -

struct MathFn{
    // -*-
    double operator()(const std::string& name, double x){
        if(name=="sin"){ return std::sin(x); }
        if(name=="cos"){ return std::cos(x); }
        if(name=="tan"){ return std::tan(x); }
        if(name=="asin"){ return std::asin(x); }
        if(name=="acos"){ return std::acos(x); }
        if(name=="atan"){ return std::atan(x); }
        if(name=="sinh"){ return std::sinh(x); }
        if(name=="cosh"){ return std::cosh(x); }
        if(name=="tanh"){ return std::tanh(x); }
        if(name=="asinh"){ return std::asinh(x); }
        if(name=="acosh"){ return std::acosh(x); }
        if(name=="atanh"){ return std::atanh(x); }
        if(name=="exp"){ return std::exp(x); }
        if(name=="sqrt"){
            if(x < 0.0){
                throw std::runtime_error("'sqrt' expects positive argument.");
            }    
            return std::sqrt(x);
        }
        if(name=="ln"){
            if(x <= 0.0){
                throw std::runtime_error("'ln' expects strictly positive argument.");
            }
            return std::log(x);
        }
        if(name=="log"){
            if(x <= 0.0){
                throw std::runtime_error("'log' expects strictly positive argument.");
            }
            return std::log10(x);
        }
        if(name=="abs"){ return std::abs(x); }
        throw std::runtime_error("not implemented unary math function '" + name + "'.");
    }

    // -*-
    double operator()(const std::string& name, double x, double y){
        if(name=="pow"){ return std::pow(x, y); }
        if(name=="min"){ return std::min(x, y); }
        if(name=="max"){ return std::max(x, y); }
        throw std::runtime_error("not implemented binary math function '" + name + "'.");
    }
};

// -*-
Expr Interpreter::eval(const Symbol& sym, Env& env) const{
    return env.get(sym.name);
}

// -*-
Expr Interpreter::eval(const Number& num, Env& env) const{
    return makeNumber(num.value);
}

/*
class Interpreter final : public EvalVisitor, public ExecuteVisitor {
public:
Expr Interpreter::eval(const Negate& negate, Env& env) const{}
Expr Interpreter::eval(const Add& add, Env& env) const{}
Expr Interpreter::eval(const Sub& sub, Env& env) const{}
Expr Interpreter::eval(const Mul& mul, Env& env) const{}
Expr Interpreter::eval(const Div& div, Env& env) const{}
Expr Interpreter::eval(const Pow& pow, Env& env) const{}
Expr Interpreter::eval(const Call& call, Env& env) const{}
Expr Interpreter::eval(const Lambda& lambda, Env& env) const{}

void Interpreter::execute(const Let& let, Env& env) const{}
void Interpreter::execute(const Fun& fun, Env& env) const{}

private:
Value Interpreter::applyBinaryOp(Value lhs, Value rhs, std::function<double(double, double)> op){}

};

*/


// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola                                       -*-
// -*----------------------------------------------------------------*-