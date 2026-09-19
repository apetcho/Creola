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

// -*-
Expr Interpreter::eval(const Negate& negate, Env& env) const{
    auto expr = this->eval(negate.arg, env);
    if(is_number(expr)){
        auto num = as_number(expr);
        if(std::holds_alternative<std::int64_t>(num.value)){
            num = -1 * std::get<std::int64_t>(num.value);
        }else if(std::holds_alternative<double>(num.value)){
            num = -1.0 * std::get<double>(num.value);
        }else{
            num = -1.0 * std::get<std::complex<double>>(num.value);
        }
        return makeNumber(num.value);
    }

    return makeNegate(std::move(expr));
}

// -
Expr Interpreter::eval(const Add& add, Env& env) const{
    auto lhs = this->eval(add.lhs, env);
    auto rhs = this->eval(add.rhs, env);
    if(is_number(lhs) && is_number(rhs)){
        auto xnum = as_number(lhs);
        auto ynum = as_number(rhs);
        if(std::holds_alternative<std::int64_t>(xnum.value) && std::holds_alternative<std::int64_t>(ynum.value)){
            auto num = std::get<std::int64_t>(xnum.value) + std::get<std::int64_t>(ynum.value);
            return makeNumber(num);
        }
        if(std::holds_alternative<double>(xnum.value) && std::holds_alternative<std::int64_t>(ynum.value)){
            auto num = std::get<double>(xnum.value) + static_cast<double>(std::get<std::int64_t>(ynum.value));
            return makeNumber(num);
        }
        if(std::holds_alternative<std::int64_t>(xnum.value) && std::holds_alternative<double>(ynum.value)){
            auto num = static_cast<double>(std::get<std::int64_t>(xnum.value)) + std::get<double>(ynum.value);
            return makeNumber(num);
        }
        if(std::holds_alternative<double>(xnum.value) && std::holds_alternative<double>(ynum.value)){
            auto num = std::get<double>(xnum.value) + std::get<double>(ynum.value);
            return makeNumber(num);
        }
        if(std::holds_alternative<std::complex<double>>(xnum.value)){
            auto x = std::get<std::complex<double>>(xnum.value);
            if(std::holds_alternative<std::int64_t>(ynum.value)){
                auto y = static_cast<double>(std::get<std::int64_t>(ynum.value));
                auto num = x + y;
                return makeNumber(num);
            }
            if(std::holds_alternative<double>(ynum.value)){
                auto y = std::get<double>(ynum.value);
                auto num = x + y;
                return makeNumber(num);
            }
            auto y = std::get<std::complex<double>>(ynum.value);
            auto num = x + y;
            return makeNumber(num);
        }
    }

    return makeBinary('+', std::move(lhs), std::move(rhs));
}

/*
class Interpreter final : public EvalVisitor, public ExecuteVisitor {
public:
Expr Interpreter::eval(Expr expr, Env& env){}
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