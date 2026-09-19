#include "ast.hpp"
#include "evaluator.hpp"

#include<stdexcept>
#include<sstream>
#include<iomanip>
#include<cmath>
#include<set>


// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola                                     -*-
// -*----------------------------------------------------------------*-
namespace creola{
// -

struct MathFn{
    // -*-
    Expr operator()(const std::string& name, double x){
        if(name=="sin"){ return makeNumber(std::sin(x)); }
        if(name=="cos"){ return makeNumber(std::cos(x)); }
        if(name=="tan"){ return makeNumber(std::tan(x)); }
        if(name=="asin"){ return makeNumber(std::asin(x)); }
        if(name=="acos"){ return makeNumber(std::acos(x)); }
        if(name=="atan"){ return makeNumber(std::atan(x)); }
        if(name=="sinh"){ return makeNumber(std::sinh(x)); }
        if(name=="cosh"){ return makeNumber(std::cosh(x)); }
        if(name=="tanh"){ return makeNumber(std::tanh(x)); }
        if(name=="asinh"){ return makeNumber(std::asinh(x)); }
        if(name=="acosh"){ return makeNumber(std::acosh(x)); }
        if(name=="atanh"){ return makeNumber(std::atanh(x)); }
        if(name=="exp"){ return makeNumber(std::exp(x)); }
        if(name=="sqrt"){
            if(x < 0.0){
                throw std::runtime_error("'sqrt' expects positive argument.");
            }    
            return makeNumber(std::sqrt(x));
        }
        if(name=="ln"){
            if(x <= 0.0){
                throw std::runtime_error("'ln' expects strictly positive argument.");
            }
            return makeNumber(std::log(x));
        }
        if(name=="log"){
            if(x <= 0.0){
                throw std::runtime_error("'log' expects strictly positive argument.");
            }
            return makeNumber(std::log10(x));
        }
        if(name=="abs"){ return makeNumber(std::abs(x)); }
        throw std::runtime_error("not implemented unary math function '" + name + "'.");
    }

    Expr operator()(const std::string& name, std::complex<double> x){
        if(name=="sin"){ return makeNumber(std::sin(x)); }
        if(name=="cos"){ return makeNumber(std::cos(x)); }
        if(name=="tan"){ return makeNumber(std::tan(x)); }
        if(name=="asin"){ return makeNumber(std::asin(x)); }
        if(name=="acos"){ return makeNumber(std::acos(x)); }
        if(name=="atan"){ return makeNumber(std::atan(x)); }
        if(name=="sinh"){ return makeNumber(std::sinh(x)); }
        if(name=="cosh"){ return makeNumber(std::cosh(x)); }
        if(name=="tanh"){ return makeNumber(std::tanh(x)); }
        if(name=="asinh"){ return makeNumber(std::asinh(x)); }
        if(name=="acosh"){ return makeNumber(std::acosh(x)); }
        if(name=="atanh"){ return makeNumber(std::atanh(x)); }
        if(name=="exp"){ return makeNumber(std::exp(x)); }
        if(name=="sqrt"){ return makeNumber(std::sqrt(x)); }
        if(name=="ln"){ return makeNumber(std::log(x)); }
        if(name=="log"){ return makeNumber(std::log10(x)); }
        if(name=="abs"){ return makeNumber(std::abs(x)); }
        throw std::runtime_error("not implemented unary math function '" + name + "'.");
    }

    // -*-
    Expr operator()(const std::string& name, double x, double y){
        if(name=="pow"){ return makeNumber(std::pow(x, y)); }
        if(name=="min"){ return makeNumber(std::min(x, y)); }
        if(name=="max"){ return makeNumber(std::max(x, y)); }
        if(name=="hypot"){ return makeNumber(std::hypot(x, y)); }
        throw std::runtime_error("not implemented binary math function '" + name + "'.");
    }

    Expr operator()(const std::string& name, double x, std::complex<double> y){
        if(name=="pow"){ return makeNumber(std::pow(x, y)); }
        throw std::runtime_error("not implemented binary complex math function '" + name + "'.");
    }

    Expr operator()(const std::string& name, std::complex<double> x, double y){
        if(name=="pow"){ return makeNumber(std::pow(x, y)); }
        throw std::runtime_error("not implemented binary complex math function '" + name + "'.");
    }

    Expr operator()(const std::string& name, std::complex<double> x, std::complex<double> y){
        if(name=="pow"){ return makeNumber(std::pow(x, y)); }
        throw std::runtime_error("not implemented binary complex math function '" + name + "'.");
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

// -*-
Expr Interpreter::eval(const Sub& sub, Env& env) const{
    auto lhs = this->eval(sub.lhs, env);
    auto rhs = this->eval(sub.rhs, env);
    if(is_number(lhs) && is_number(rhs)){
        auto xnum = as_number(lhs);
        auto ynum = as_number(rhs);
        if(std::holds_alternative<std::int64_t>(xnum.value) && std::holds_alternative<std::int64_t>(ynum.value)){
            auto num = std::get<std::int64_t>(xnum.value) - std::get<std::int64_t>(ynum.value);
            return makeNumber(num);
        }
        if(std::holds_alternative<double>(xnum.value) && std::holds_alternative<std::int64_t>(ynum.value)){
            auto num = std::get<double>(xnum.value) - static_cast<double>(std::get<std::int64_t>(ynum.value));
            return makeNumber(num);
        }
        if(std::holds_alternative<std::int64_t>(xnum.value) && std::holds_alternative<double>(ynum.value)){
            auto num = static_cast<double>(std::get<std::int64_t>(xnum.value)) - std::get<double>(ynum.value);
            return makeNumber(num);
        }
        if(std::holds_alternative<double>(xnum.value) && std::holds_alternative<double>(ynum.value)){
            auto num = std::get<double>(xnum.value) - std::get<double>(ynum.value);
            return makeNumber(num);
        }
        if(std::holds_alternative<std::complex<double>>(xnum.value)){
            auto x = std::get<std::complex<double>>(xnum.value);
            if(std::holds_alternative<std::int64_t>(ynum.value)){
                auto y = static_cast<double>(std::get<std::int64_t>(ynum.value));
                auto num = x - y;
                return makeNumber(num);
            }
            if(std::holds_alternative<double>(ynum.value)){
                auto y = std::get<double>(ynum.value);
                auto num = x - y;
                return makeNumber(num);
            }
            auto y = std::get<std::complex<double>>(ynum.value);
            auto num = x - y;
            return makeNumber(num);
        }
    }

    return makeBinary('-', std::move(lhs), std::move(rhs));
}

// -*-
Expr Interpreter::eval(const Mul& mul, Env& env) const{
    auto lhs = this->eval(mul.lhs, env);
    auto rhs = this->eval(mul.rhs, env);
    if(is_number(lhs) && is_number(rhs)){
        auto xnum = as_number(lhs);
        auto ynum = as_number(rhs);
        if(std::holds_alternative<std::int64_t>(xnum.value) && std::holds_alternative<std::int64_t>(ynum.value)){
            auto num = std::get<std::int64_t>(xnum.value) * std::get<std::int64_t>(ynum.value);
            return makeNumber(num);
        }
        if(std::holds_alternative<double>(xnum.value) && std::holds_alternative<std::int64_t>(ynum.value)){
            auto num = std::get<double>(xnum.value) * static_cast<double>(std::get<std::int64_t>(ynum.value));
            return makeNumber(num);
        }
        if(std::holds_alternative<std::int64_t>(xnum.value) && std::holds_alternative<double>(ynum.value)){
            auto num = static_cast<double>(std::get<std::int64_t>(xnum.value)) * std::get<double>(ynum.value);
            return makeNumber(num);
        }
        if(std::holds_alternative<double>(xnum.value) && std::holds_alternative<double>(ynum.value)){
            auto num = std::get<double>(xnum.value) * std::get<double>(ynum.value);
            return makeNumber(num);
        }
        if(std::holds_alternative<std::complex<double>>(xnum.value)){
            auto x = std::get<std::complex<double>>(xnum.value);
            if(std::holds_alternative<std::int64_t>(ynum.value)){
                auto y = static_cast<double>(std::get<std::int64_t>(ynum.value));
                auto num = x * y;
                return makeNumber(num);
            }
            if(std::holds_alternative<double>(ynum.value)){
                auto y = std::get<double>(ynum.value);
                auto num = x * y;
                return makeNumber(num);
            }
            auto y = std::get<std::complex<double>>(ynum.value);
            auto num = x * y;
            return makeNumber(num);
        }
    }

    return makeBinary('*', std::move(lhs), std::move(rhs));
}

// -*-
Expr Interpreter::eval(const Div& div, Env& env) const{
    auto lhs = this->eval(div.lhs, env);
    auto rhs = this->eval(div.rhs, env);
    if(is_number(lhs) && is_number(rhs)){
        auto xnum = as_number(lhs);
        auto ynum = as_number(rhs);
        if(std::holds_alternative<std::int64_t>(xnum.value) && std::holds_alternative<std::int64_t>(ynum.value)){
            auto deno = std::get<std::int64_t>(ynum.value);
            if(deno == 0){
                throw std::runtime_error("Division by zero");
            }
            auto num = std::get<std::int64_t>(xnum.value) / deno;
            return makeNumber(num);
        }
        if(std::holds_alternative<double>(xnum.value) && std::holds_alternative<std::int64_t>(ynum.value)){
            auto deno = std::get<std::int64_t>(ynum.value);
            if(deno == 0){
                throw std::runtime_error("Division by zero");
            }
            auto num = std::get<double>(xnum.value) / static_cast<double>(deno);
            return makeNumber(num);
        }
        if(std::holds_alternative<std::int64_t>(xnum.value) && std::holds_alternative<double>(ynum.value)){
            auto deno = std::get<double>(ynum.value);
            if(deno == 0.0){
                throw std::runtime_error("Division by zero");
            }
            auto num = static_cast<double>(std::get<std::int64_t>(xnum.value)) / deno;
            return makeNumber(num);
        }
        if(std::holds_alternative<double>(xnum.value) && std::holds_alternative<double>(ynum.value)){
            auto deno = std::get<double>(ynum.value);
            if(deno == 0.0){
                throw std::runtime_error("Division by zero");
            }
            auto num = std::get<double>(xnum.value) / deno;
            return makeNumber(num);
        }
        if(std::holds_alternative<std::complex<double>>(xnum.value)){
            auto x = std::get<std::complex<double>>(xnum.value);
            if(std::holds_alternative<std::int64_t>(ynum.value)){
                auto y = static_cast<double>(std::get<std::int64_t>(ynum.value));
                if(y == 0.0){
                    throw std::runtime_error("Division by zero");
                }
                auto num = x / y;
                return makeNumber(num);
            }
            if(std::holds_alternative<double>(ynum.value)){
                auto y = std::get<double>(ynum.value);
                if(y == 0.0){
                    throw std::runtime_error("Division by zero");
                }
                auto num = x / y;
                return makeNumber(num);
            }
            auto y = std::get<std::complex<double>>(ynum.value);
            if(y == 0.0){
                throw std::runtime_error("Division by zero");
            }
            auto num = x / y;
            return makeNumber(num);
        }
    }

    return makeBinary('/', std::move(lhs), std::move(rhs));
}

// -*-
Expr Interpreter::eval(const Pow& pow, Env& env) const{
    auto lhs = this->eval(pow.base, env);
    auto rhs = this->eval(pow.expo, env);
    if(is_number(lhs) && is_number(rhs)){
        auto xnum = as_number(lhs);
        auto ynum = as_number(rhs);
        if(std::holds_alternative<std::int64_t>(xnum.value) && std::holds_alternative<std::int64_t>(ynum.value)){
            auto deno = std::get<std::int64_t>(ynum.value);
            auto num = std::pow(std::get<std::int64_t>(xnum.value), deno);
            return makeNumber(num);
        }
        if(std::holds_alternative<double>(xnum.value) && std::holds_alternative<std::int64_t>(ynum.value)){
            auto deno = std::get<std::int64_t>(ynum.value);
            auto num = std::pow(std::get<double>(xnum.value), static_cast<double>(deno));
            return makeNumber(num);
        }
        if(std::holds_alternative<std::int64_t>(xnum.value) && std::holds_alternative<double>(ynum.value)){
            auto deno = std::get<double>(ynum.value);
            auto num = std::pow(static_cast<double>(std::get<std::int64_t>(xnum.value)), deno);
            return makeNumber(num);
        }
        if(std::holds_alternative<double>(xnum.value) && std::holds_alternative<double>(ynum.value)){
            auto deno = std::get<double>(ynum.value);
            auto num = std::pow(std::get<double>(xnum.value), deno);
            return makeNumber(num);
        }
        if(std::holds_alternative<std::complex<double>>(xnum.value)){
            auto x = std::get<std::complex<double>>(xnum.value);
            if(std::holds_alternative<std::int64_t>(ynum.value)){
                auto y = static_cast<double>(std::get<std::int64_t>(ynum.value));
                auto num = std::pow(x, y);
                return makeNumber(num);
            }
            if(std::holds_alternative<double>(ynum.value)){
                auto y = std::get<double>(ynum.value);
                auto num = std::pow(x, y);
                return makeNumber(num);
            }
            auto y = std::get<std::complex<double>>(ynum.value);
            auto num = std::pow(x, y);
            return makeNumber(num);
        }
    }

    return makeBinary('^', std::move(lhs), std::move(rhs));
}

// -*-
Expr Interpreter::eval(const Call& call, Env& env) const{
    static std::set<std::string> unaryFn = {
        "sin", "cos", "tan", "asin", "acos", "atan",
        "sinh", "cosh", "tanh", "asinh", "acosh", "atanh",
        "sqrt", "exp", "ln", "log", "abs",
    };

    static std::set<std::string> binaryFn = {
        "min", "max", "pow", "hypot",
    };

    auto name = call.name;
    auto args = call.args;
    if(unaryFn.find(name) != unaryFn.end()){
        MathFn fun;
        if(args.size()!=1){
            std::stringstream stream;
            stream << "'" << name << "' expects 1 arguments, got " << args.size();
            throw std::runtime_error(stream.str());
        }
        auto expr = this->eval(args[0], env);
        if(is_number(expr)){
            auto num = as_number(expr);
            if(std::holds_alternative<std::int64_t>(num.value)){
                auto x = static_cast<double>(std::get<std::int64_t>(num.value));
                return fun(name, x);
            }
            if(std::holds_alternative<double>(num.value)){
                auto x = std::get<double>(num.value);
                return fun(name, x);
            }
            auto z = std::get<std::complex<double>>(num.value);
            return fun(name, z);
        }

        return makeCall(name, std::move(args));
    }

    if(binaryFn.find(name) != binaryFn.end()){
        MathFn fun;
        if(args.size()!=2){
            std::stringstream stream;
            stream << "'" << name << "' expects 2 arguments, got " << args.size();
            throw std::runtime_error(stream.str());
        }
        auto xexpr = this->eval(args[0], env);
        auto yexpr = this->eval(args[1], env);
        if(is_number(xexpr) && is_number(yexpr)){
            auto xnum = as_number(xexpr);
            auto ynum = as_number(yexpr);
            if(std::holds_alternative<std::int64_t>(xnum.value)){
                auto x = static_cast<double>(std::get<double>(xnum.value));
                if(std::holds_alternative<std::int64_t>(ynum.value)){
                    auto y = static_cast<double>(std::get<double>(ynum.value));
                    return fun(name, x, y);
                }
                if(std::holds_alternative<double>(ynum.value)){
                    auto y = std::get<double>(ynum.value);
                    return fun(name, x, y);
                }
                auto y = std::get<std::complex<double>>(ynum.value);
                return fun(name, x, y);
            }
            if(std::holds_alternative<double>(xnum.value)){
                auto x = std::get<double>(xnum.value);
                if(std::holds_alternative<std::int64_t>(ynum.value)){
                    auto y = static_cast<double>(std::get<double>(ynum.value));
                    return fun(name, x, y);
                }
                if(std::holds_alternative<double>(ynum.value)){
                    auto y = std::get<double>(ynum.value);
                    return fun(name, x, y);
                }
                auto y = std::get<std::complex<double>>(ynum.value);
                return fun(name, x, y);
            }
            auto x = std::get<std::complex<double>>(xnum.value);
            if(std::holds_alternative<std::int64_t>(ynum.value)){
                auto y = static_cast<double>(std::get<std::int64_t>(ynum.value));
                return fun(name, x, y);
            }
            if(std::holds_alternative<double>(ynum.value)){
                auto y = std::get<double>(ynum.value);
                return fun(name, x, y);
            }
            auto y = std::get<std::complex<double>>(ynum.value);
            return fun(name, x, y);
        }

        return makeCall(name, std::move(args));
    }

    auto expr = env.get(name);
    if(!is_lambda(expr)){
        std::stringstream stream;
        stream << std::quoted(name) << " is not a function.";
        throw std::runtime_error(stream.str());
    }
    auto lambda = as_lambda(expr);
    if(lambda.params.size() != args.size()){
        std::stringstream stream;
        stream << std::quoted(name) << " expects " << lambda.params.size();
        stream << " arguments, but got " << args.size();
        throw std::runtime_error(stream.str());
    }

    Env localEnv(&env);
    for(std::size_t i=0; i < lambda.params.size(); ++i){
        auto key = lambda.params[i];
        auto val = args[i];
        localEnv.put(key, val);
    }

    auto ans = this->eval(lambda.body, localEnv);

    return std::move(ans);
}

// -
Expr Interpreter::eval(const Lambda& lambda, Env& env) const{
    auto params = lambda.params;
    auto body = lambda.body;
    return makeLambda(params, std::move(body));
}

/*
class Interpreter final : public EvalVisitor, public ExecuteVisitor {
public:
Expr Interpreter::eval(Expr expr, Env& env){}

void Interpreter::execute(const Let& let, Env& env) const{}
void Interpreter::execute(const Fun& fun, Env& env) const{}

private:
Value Interpreter::applyBinaryOp(Value lhs, Value rhs, std::function<double(double, double)> op){}

};

*/


// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola                                       -*-
// -*----------------------------------------------------------------*-