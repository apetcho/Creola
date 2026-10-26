#include "expr.hpp"

#include<algorithm>
#include<stdexcept>
#include<cstdio>
#include<map>

// ----------------------------------------------------------------------------
// begin::namespace::creola
// ----------------------------------------------------------------------------
namespace creola{
// -

namespace {
    Expr make(Kind kind, f64 val, std::string name, std::vector<Expr> args){
        auto self = std::make_shared<Node>();
        self->kind = kind;
        self->value = val;
        self->name = std::move(name);
        self->args = std::move(args);
        return self;
    }

    /**
     * @brief Return the degree of an expression
     * 
     * @param expr 
     * @return f64 
     */
    f64 degree(const Expr& expr){
        switch(expr->kind){
        case Kind::Sym:
            return expr->name == "i" ? 0 : 1;
        case Kind::Pow:
            return isNumber(expr->args[1]) ? expr->args[1]->value*degree(expr->args[0]) : 0;
        case Kind::Mul:{
                f64 d = 0;
                for(auto& arg: expr->args){
                    d += degree(arg);
                }
                return d;
            }//
        default:
            return 0;
        }
        return 0;
    }

    /**
     * @brief Split an expression into its numeric coefficient and terms
     * 
     * @param expr 
     * @return std::pair<f64, Expr> 
     */
    std::pair<f64, Expr> split(const Expr& expr){
        if(expr->kind == Kind::Mul && isNumber(expr->args[0])){
            std::vector<Expr> rest(expr->args.begin()+1, expr->args.end());
            return {
                expr->args[0]->value,
                rest.size() == 1 ? rest[0] : make(Kind::Mul, 0, {}, rest)
            };
        }
        return {1, expr};
    }
} // namespace

// -
double g_displayTol = 1e-12;

// -
std::optional<std::pair<i64, i64>> asFraction(f64 val){
    if(isInteger(val) || !std::isfinite(val)){
        return std::nullopt;
    }

    //! @todo move this to the global settings with the default value set to 1e-9,
    // fractional.eps = 1e-9
    constexpr f64 EPS = 1e-9;
    auto a = std::fabs(val);
    // integer up to rounding noise
    if(std::fabs(a - std::round(a)) < EPS*std::max(1.0, a)){
        return std::nullopt;
    }

    // transform a floating-point number into a fractional number.
    //! @todo: add fractional.maxIterations = 1000
    constexpr i64 MAX_ITERATIONS = 1000; 
    for(i64 q=2; q <= MAX_ITERATIONS; ++q){
        auto p = a * q;
        auto r = std::round(p);
        if(r > 0 && std::fabs(p-r) < EPS){
            return std::make_pair((val < 0 ? -1 : 1) * static_cast<i64>(r), q);
        }
    }

    return std::nullopt;
}

namespace {
    i32 g_precision = 10;

    // -
    std::optional<std::pair<i64, i64>> frac(f64 val){
        return asFraction(val);
    }

    // -
    std::string numstr(f64 val){
        if(std::fabs(val) < g_displayTol){ val = 0; }
        if(std::isfinite(val) && std::fabs(val) < 1e15 && std::fabs(val-std::round(val)) < 1e-9*std::max(1.0, std::fabs(val))){
            val = std::round(val) + 0.0;
        }
        if(auto f = frac(val)){
            return std::to_string(f->first) + "/" + std::to_string(f->second);
        }
        char buf[64];
        if(isInteger(val) && std::fabs(val) < 1e15){
            std::snprintf(buf, sizeof(buf), "%.0f", val);
        }else{
            std::snprintf(buf, sizeof(buf), "%.*g", g_precision, val);
        }
        return buf;
    }

    bool isAtom(const Expr& expr){
        return (
            expr->kind==Kind::Sym || expr->kind==Kind::Fun ||
            (expr->kind==Kind::Num && isInteger(expr->value) && expr->value >= 0)
        );
    }

    Expr negate(const Expr& expr){
        return make_mul({make_number(-1), expr});
    }
} // namespace

// -
Expr make_number(f64 val){
    return make(Kind::Num, val==0 ? 0.0 : val, {}, {});
}

// -
Expr make_symbol(const std::string& name){
    return make(Kind::Sym, 0, name, {});
}

// -
bool isNumber(const Expr& expr){
    return expr->kind == Kind::Num;
}

// -
bool isNumber(const Expr& expr, f64 val){
    return isNumber(expr) && expr->value == val;
}

// -
Expr make_add(std::vector<Expr> terms){
    f64 c = 0;
    std::vector<Expr> flat;
    for(auto& term: terms){
        if(term->kind==Kind::Add){
            flat.insert(flat.end(), term->args.begin(), term->args.end());
        }else{
            flat.push_back(term);
        }
    }
    std::map<std::string, std::pair<f64, Expr>> g;
    for(auto& term: flat){
        if(isNumber(term)){ c += term->value; continue; }
        auto [k, r] = split(term);
        auto key = str(r);
        auto entry = g.find(key);
        if(entry == g.end()){
            g.emplace(key, std::make_pair(k, r));
        }else{
            entry->second.first += k;
        }
    }
    std::vector<std::pair<std::string, Expr>> out;
    for(auto& [key, pair]: g){
        if(pair.first==0){ continue; }
        out.emplace_back(key, pair.first == 1 ? pair.second : make_mul({make_number(pair.first), pair.second}));
    }
    std::sort(out.begin(), out.end(), [](auto& a, auto& b){
        f64 x = degree(a.second);
        f64 y = degree(b.second);
        return x != y ?  x > y : a.first < b.first;
    });

    // real parst first, then terms containing i: 5 + 5i, x + i*y
    std::vector<Expr> myTerms;
    std::vector<Expr> myImaginary;
    for(auto& item: out){
        (contains(item.second, "i") ? myImaginary : myTerms).push_back(item.second);
    }
    if(c!=0){
        myTerms.push_back(make_number(c));
    }
    myTerms.insert(myTerms.end(), myImaginary.begin(), myImaginary.end());
    if(myTerms.empty()){ return make_number(0); }
    if(myTerms.size()==1){ return myTerms[0]; }
    return make(Kind::Add, 0, {}, myTerms);
}

// -
Expr make_mul(std::vector<Expr> factors){
    f64 c = 1;
    std::vector<Expr> flat;
    for(auto& factor: factors){
        if(factor->kind==Kind::Mul){
            flat.insert(flat.end(), factor->args.begin(), factor->args.end());
        }
        else{
            flat.push_back(factor);
        }
    }
    std::map<std::string, std::pair<Expr, f64>> g;
    for(auto& term: flat){
        if(isNumber(term)){
            c *= term->value;
            continue;
        }
        Expr base = term;
        f64 expo = 1;
        if(term->kind==Kind::Pow && isNumber(term->args[1])){
            base = term->args[0];
            expo = term->args[1]->value;
        }
        auto key = str(base);
        auto entry = g.find(key);
        if(entry == g.end()){
            g.emplace(key, std::make_pair(base, expo));
        }else{
            entry->second.second += expo;
        }
    }

    if(c == 0){ make_number(0); }
    std::vector<std::pair<std::string, Expr>> out;
    for(auto& [key, pair]: g){
        if(pair.second==0){ continue; }
        if(pair.first->kind==Kind::Sym && pair.first->name == "i" && isInteger(pair.second)){
            auto k = static_cast<i32>(((static_cast<i64>(pair.second) % 4) + 4) % 4); // i^2 = -i
            if(k >= 2){ c = -c; }
            if(k % 2){ out.emplace_back("i", pair.first); }
            continue;
        }
        Expr f = make_power(pair.first, make_number(pair.second));
        out.emplace_back(str(f), f);
    }
    std::sort(out.begin(), out.end(), [](auto& a, auto& b){
        return a.first < b.first;
    });
    std::vector<Expr> myTerms;
    if(c != 1 || out.empty()){ myTerms.push_back(make_number(c)); }
    for(auto& item: out){
        myTerms.push_back(item.second);
    }
    if(myTerms.size() == 1){ return myTerms[0]; }
    return make(Kind::Mul, 0, {}, myTerms);
}

// -
Expr make_power(const Expr& base, const Expr& expo){
    if(isNumber(expo, 0)){ return make_number(1); }
    if(isNumber(expo, 1)){ return base; }
    if(base->kind==Kind::Sym && base->name == "i" && isNumber(expo) && isInteger(expo->value)){
        auto k = static_cast<i32>(((static_cast<i64>(expo->value) % 4) + 4) % 4);
        return k == 0 ? make_number(1) : k == 1 ? base : k == 2 ? make_number(-1) : make_mul({make_number(-1), base});
    }
    if(isNumber(base, 1)){ return make_number(1); }
    if(base->kind==Kind::Sym && base->name == "i" && isNumber(expo) && isInteger(expo->value)){
        switch(((static_cast<i64>(expo->value)%4 + 4)%4)){
            case 0: return make_number(1);
            case 1: return base;
            case 2: return make_number(-1);
            default: return make_mul({make_number(-1), base});
        }
    }
    if(isNumber(base) && base->value < 0 && isNumber(expo, 0.5)){
        return make_mul({make_power(make_number(-base->value), expo), make_symbol("i")});
    }
    if(isNumber(base, 0) && isNumber(expo) && expo->value > 0){
        return make_number(0);
    }
    if(isNumber(base) && isNumber(expo)){
        auto r = std::pow(base->value, expo->value);
        if(std::isfinite(r) && (isInteger(expo->value) || isInteger(r))){
            return make_number(r);
        }
    }
    if(isNumber(expo) && isInteger(expo->value)){
        if(base->kind == Kind::Mul){
            std::vector<Expr> factors;
            for(auto& arg: base->args){
                factors.push_back(make_power(arg, expo));
            }
            return make_mul(factors);
        }
        if(base->kind == Kind::Pow){
            return make_power(base->args[0], make_mul({base->args[1], expo}));
        }
    }
    return make(Kind::Pow, 0, {}, {base, expo});
}

// -
std::optional<f64> evalFun(const std::string& name, f64 x){
    static const std::map<std::string, f64 (*)(f64)> funcTable = {
        {"sin", [](f64 x){ return std::sin(x); }},
        {"cos", [](f64 x){ return std::cos(x); }},
        {"tan", [](f64 x){ return std::tan(x); }},
        {"asin", [](f64 x){ return std::asin(x); }},
        {"acos", [](f64 x){ return std::acos(x); }},
        {"atan", [](f64 x){ return std::atan(x); }},
        {"sinh", [](f64 x){ return std::sinh(x); }},
        {"cosh", [](f64 x){ return std::cosh(x); }},
        {"tanh", [](f64 x){ return std::tanh(x); }},
        {"exp", [](f64 x){ return std::exp(x); }},
        {"ln", [](f64 x){ return std::log(x); }},
        {"abs", [](f64 x){ return std::abs(x); }},
    };
    auto entry = funcTable.find(name);
    if(entry == funcTable.end()){ return std::nullopt; }
    return entry->second(x);
}

// -
Expr make_fun(const std::string& name, const Expr& arg){
    if(auto x = numeric(arg)){
        // fold only when the result is an exact integer (cos(pi) = -1)
        if(auto r = evalFun(name, *x); r && std::isfinite(*r) && std::fabs(*r - std::round(*r)) < 1e-12){
            return make_number(std::round(*r));
        }
    }
    if(name == "exp" && arg->kind==Kind::Fun && arg->name=="ln"){
        return arg->args[0];
    }
    if(name == "ln" && arg->kind == Kind::Fun && arg->name == "exp"){
        return arg->args[0];
    }
    return make(Kind::Fun, 0, name, {arg});
}

bool contains(const Expr& expr, const std::string& var){
    if(expr->kind==Kind::Sym){ return expr->name == var; }
    for(auto& arg: expr->args){
        if(contains(arg, var)){
            return true;
        }
    }
    return false;
}

// -
std::optional<f64> numeric(const Expr& expr){
    switch(expr->kind){
        case Kind::Num: return expr->value;
        case Kind::Sym:
            if(expr->name == "pi" || expr->name == "\xCF\x80"){
                return 3.14159265358979323846;
            }
            if(expr->name == "e"){ return 2.718281845904523536; }
            return std::nullopt;
        case Kind::Fun:{
                auto x = numeric(expr->args[0]);
                return x ? evalFun(expr->name, *x) : std::nullopt;
            }// -
        case Kind::Pow: {
            auto a = numeric(expr->args[0]);
            auto b = numeric(expr->args[0]);
            if(!a || !b){ return std::nullopt; }
            return std::pow(*a, *b);
        }
        case Kind::Add:
        case Kind::Mul: {
                f64 r = expr->kind == Kind::Add ? 0 : 1;
                for(auto& arg: expr->args){
                    auto x = numeric(arg);
                    if(!x){ return std::nullopt; }
                    if(expr->kind == Kind::Add){ r += *x; }
                    else{ r *= *x; }
                }
                return r;
            }//
    }
    return std::nullopt;
}

// -
void setPrecision(i32 prec){
    g_precision = prec < 1 ? 1 : prec > 17 ? 17 : prec; 
}

// -
std::string str(const Expr& expr){
    switch(expr->kind){
        case Kind::Num: return numstr(expr->value);
        case Kind::Sym: return expr->name;
        case Kind::Fun: return expr->name + "(" + str(expr->args[0]) + ")";
        case Kind::Add: {
            std::string mystr;
            bool first = true;
            for(auto& term: expr->args){
                auto txt = str(term);
                if(txt=="0"){ continue; }
                if(first){ mystr = txt; }
                else if(!txt.empty() && txt[0] == '-'){
                    mystr += " - " + txt.substr(1);
                }else{
                    mystr += " + " + txt;
                }
                first = false;
            }
            return first ? "0" : mystr;
        } // -
        case Kind::Pow: {
            const Expr& base = expr->args[0];
            const Expr& expo = expr->args[1];
            if(isNumber(expo, 0.5)){ return "sqrt(" + str(base) + ")"; }
            if(isNumber(expo) && expo->value < 0){
                Expr deno = isNumber(expo, -1) ? base : make_power(base, make_number(-expo->value));
                auto sdeno = str(deno);
                return "1/" + (isAtom(deno) || deno->kind==Kind::Pow ? sdeno : "(" + sdeno + ")");
            }
            auto strBase = str(base);
            auto strExpo = str(expo);
            if(!isAtom(base)){ strBase = "(" + strBase + ")"; }
            if(!isAtom(expo)){ strExpo = "(" + strExpo + ")"; }
            return strBase + "^" + strExpo;
        }// -
        case Kind::Mul:{
            f64 c = 1;
            std::vector<Expr> numFactors, denoFactors;
            for(auto& arg: expr->args){
                if(isNumber(arg)){ c = arg->value; }
                else if(arg->kind==Kind::Pow && isNumber(arg->args[1]) && arg->args[1]->value < 0){
                    denoFactors.push_back(
                        isNumber(arg->args[1], -1) ?
                        arg->args[0] :
                        make_power(arg->args[0], make_number(-arg->args[1]->value))
                    );
                }else{
                    numFactors.push_back(arg);
                }
            }
            if(c != 0 && std::fabs(c) < g_displayTol){
                // below the display tolerance
                return "0";
            }
            f64 ac = std::fabs(c);
            std::vector<std::string> dens;
            std::string coef;
            if(auto f = frac(ac)){
                if(f->first != 1){ coef = std::to_string(f->first); }
                dens.push_back(std::to_string(f->second));
            }else if(ac != 1){ coef = numstr(ac); }
            std::string mystr;
            if(numFactors.empty()){
                mystr = coef.empty() ? "1": coef;
            }else{
                mystr = coef;
                const Expr& f0 = numFactors[0];
                bool implicit = (
                    coef.empty() || f0->kind==Kind::Sym || f0->kind==Kind::Fun ||
                    (f0->kind == Kind::Pow && f0->args[0]->kind == Kind::Sym && !isNumber(f0->args[1], 0.5))
                );
                if(!coef.empty() && !implicit){ mystr += "*"; }
                for(size_t i=0; i < numFactors.size(); ++i){
                    auto fs = str(numFactors[i]);
                    if(numFactors[i]->kind==Kind::Add){
                        fs = "(" + fs + ")";
                    }
                    mystr += (i ? "*" : "") + fs;
                }
            }
            for(auto& deno: denoFactors){
                auto ds = str(deno);
                if(deno->kind==Kind::Add || deno->kind==Kind::Mul){
                    ds = "(" + ds + ")";
                }
                dens.push_back(ds);
            }
            if(dens.size()==1){ mystr += "/" + dens[0]; }
            else if(dens.size() > 1) {
                mystr += "/(";
                for(size_t i=0; i < dens.size(); ++i){
                    mystr += (i ? "*": "") + dens[i];
                }
                mystr += ")";
            }
            return (c < 0 ? "-":"") + mystr;
        }//
    }
    return "?";
}

// -
Expr substitute(const Expr& expr, const std::string& var, const Expr& val){
    switch(expr->kind){
        case Kind::Num: return expr;
        case Kind::Sym: return expr->name == var ? val : expr;
        case Kind::Fun: return make_fun(expr->name, substitute(expr->args[0], var, val));
        case Kind::Pow:
            return make_power(
                substitute(expr->args[0], var, val),
                substitute(expr->args[1], var, val)
            );
        case Kind::Add:
        case Kind::Mul:{
            std::vector<Expr> args;
            for(auto& arg: expr->args){
                args.push_back(substitute(arg, var, val));
            }
            return expr->kind == Kind::Add ? make_add(args) : make_mul(args);
        }
    }
    return expr;
}

// -
Expr simplify(const Expr& expr){
    return substitute(substitute(expr, "", expr), "", expr);
}

static Expr mulExpr(const Expr& lhs, const Expr& rhs){
    if(lhs->kind==Kind::Add){
        std::vector<Expr> terms;
        for(auto& arg: lhs->args){
            terms.push_back(mulExpr(arg, rhs));
        }
        return make_add(terms);
    }
    if(rhs->kind == Kind::Add){
        std::vector<Expr> terms;
        for(auto& arg: rhs->args){
            terms.push_back(mulExpr(lhs, arg));
        }
        return make_add(terms);
    }
    return make_mul({lhs, rhs});
}

// -
Expr expand(const Expr& expr){
    switch(expr->kind){
        case Kind::Num:
        case Kind::Sym:
            return expr;
        case Kind::Fun:
            return make_fun(expr->name, expand(expr->args[0]));
        case Kind::Add:{
            std::vector<Expr> terms;
            for(auto& arg: expr->args){
                terms.push_back(expand(arg));
            }
            return make_add(terms);
        }
        case Kind::Mul:{
            Expr acc = make_number(1);
            for(auto& arg: expr->args){
                acc = mulExpr(acc, expand(arg));
            }
            return acc;
        }
        case Kind::Pow: {
            Expr base = expand(expr->args[0]);
            const Expr& expo = expr->args[1];
            if(isNumber(expo) && isInteger(expo->value) && expo->value >= 2 && base->kind==Kind::Add){
                Expr acc = base;
                for(int i=1; i < static_cast<int>(expo->value); ++i){
                    acc = mulExpr(acc, base);
                }
                return acc;
            }
            return make_power(base, expand(expo));
        }
    }
    return expr;
}

// -
Expr diff(const Expr& expr, const std::string& var){
    switch(expr->kind){
        case Kind::Num: return make_number(0);
        case Kind::Sym: return make_number(expr->name==var ? 1 : 0);
        case Kind::Add: {
            std::vector<Expr> terms;
            for(auto& arg: expr->args){
                terms.push_back(diff(arg, var));
            }
            return make_add(terms);
        }
        case Kind::Mul: {
            std::vector<Expr> terms;
            for(size_t i=0; i < expr->args.size(); ++i){
                std::vector<Expr> factors = expr->args;
                factors[i] = diff(expr->args[i], var);
                terms.push_back(make_mul(factors));
            }
            return make_add(terms);
        }
        case Kind::Pow: {
            const Expr& base = expr->args[0];
            const Expr& expo = expr->args[1];
            if(!contains(expo, var)){
                return make_mul({
                    expo,
                    make_power(base, make_add({expo, make_number(-1)})),
                    diff(base, var)
                });
            }
            return make_mul({
                expr,
                make_add({
                    make_mul({diff(expo, var), make_fun("ln", base)}),
                    make_mul({expo, diff(base, var), make_power(base, make_number(-1))})
                })
            });
        }
        case Kind::Fun: {
            const Expr& arg = expr->args[0];
            const std::string& name = expr->name;
            Expr deriv;
            if(name=="sin"){ deriv = make_fun("cos", arg); }
            else if(name=="cos"){ deriv = negate(make_fun("sin", arg)); }
            else if(name=="tan"){ deriv = make_power(make_fun("cos", arg), make_number(-2)); }
            else if(name=="exp"){ deriv = expr; }
            else if(name=="ln"){ deriv = make_power(arg, make_number(-1)); }
            else if(name=="sinh"){ deriv = make_fun("cosh", arg); }
            else if(name=="cosh"){ deriv = make_fun("sinh", arg); }
            else if(name=="tanh"){
                deriv = make_power(make_fun("cosh", arg), make_number(-2));
            }else if(name=="atan"){
                deriv = make_power(
                    make_add({make_number(1), make_power(arg, make_number(2))}),
                    make_number(-1)
                );
            }else if(name=="asin"){
                deriv = make_power(
                    make_add({make_number(1), negate(make_power(arg, make_number(2)))}),
                    make_number(-0.5)
                );
            }else if(name=="acos"){
                deriv = negate(make_power(
                    make_add({make_number(1), negate(make_power(arg, make_number(2)))}),
                    make_number(-0.5)
                ));
            }else{
                throw std::runtime_error("diff: unsupported function '" + name + "'");
            }
            return make_mul({deriv, diff(arg, var)});
        }
    }
    return make_number(0);
}

static Expr my_integral(const Expr& expr, const std::string& var){
    auto fail = [&]() -> Expr {
        throw std::runtime_error("integral: cannot integrate " + str(expr));
    };

    switch(expr->kind){
        case Kind::Add: {
            std::vector<Expr> terms;
            for(auto& arg: expr->args){
                terms.push_back(my_integral(arg, var));
            }
            return make_add(terms);
        }
        case Kind::Mul: {
            std::vector<Expr> k;
            std::vector<Expr> d;
            for(auto& arg: expr->args){
                (contains(arg, var) ? d : k).push_back(arg);
            }
            if(d.size()==1){
                k.push_back(my_integral(d[0], var));
                return make_mul(k);
            }
            Expr arg = expand(expr);
            if(arg->kind==Kind::Add){
                return my_integral(arg, var);
            }
            return fail();
        }
        case Kind::Sym:
            return make_mul({make_number(0.5), make_power(make_symbol(var), make_number(2))});
        case Kind::Pow: {
            const Expr& base = expr->args[0];
            const Expr& expo = expr->args[1];
            if(contains(expo, var)){ return fail(); }
            Expr deriv = diff(base, var);
            if(contains(deriv, var)){ return fail(); }
            if(isNumber(expo, -1)){
                return make_mul({make_fun("ln", base), make_power(deriv, make_number(-1))});
            }
            Expr n1 = make_add({expo, make_number(1)});
            return make_mul({
                make_power(base, n1),
                make_power(make_mul({n1, deriv}), make_number(-1))
            });
        }
        case Kind::Fun: {
            const Expr& arg = expr->args[0];
            Expr deriv = diff(arg, var);
            if(contains(deriv, var)){ return fail(); }
            const std::string& name = expr->name;
            Expr self;
            if(name=="sin"){ self = negate(make_fun("cos", arg));}
            else if(name=="cos"){ self = make_fun("sin", arg); }
            else if(name=="exp"){ self = expr; }
            else if(name=="sinh"){ self = make_fun("cosh", arg); }
            else if(name=="cosh"){ self = make_fun("sinh", arg); }
            else if(name=="tan"){ self = negate(make_fun("ln", make_fun("cos", arg))); }
            else if(name=="ln"){ self = make_add({make_mul({arg, expr}), negate(arg)}); }
            else {
                return fail();
            }
            return make_mul({self, make_power(deriv, make_number(-1))});
        }
        default:
            return fail();
    }
}

// -
Expr integral(const Expr& expr, const std::string& var){
    return my_integral(expand(expr), var);
}

// -
Expr taylor(const Expr& expr, const std::string& var, const Expr& center, i32 order){
    Expr cur = expr;
    std::vector<Expr> terms;
    f64 fact = 1;
    for(int k=0; k <= order; ++k){
        Expr coef = substitute(cur, var, center);
        terms.push_back(make_mul({
            coef,
            make_number(1/fact),
            make_power(make_add({make_symbol(var), negate(center)}), make_number(k))
        }));
        cur = diff(cur, var);
        fact *= k + 1;
    }
    return expand(make_add(terms));
}

// -
Expr imag(void){
    //! @todo
    return nullptr;
}

// -
Expr fromComplex(std::complex<double> z, f64 tol){
    //! @todo
    return nullptr;
}

// -
void freeSymbols(const Expr& expr, std::set<std::string>& out){
    //! @todo
}

// -
std::optional<f64> numericAt(const Expr& expr, const std::string& var, f64 x){
    //! @todo
    return std::nullopt;
}

// -
std::optional<Complex> numericC(const Expr& expr){
    //! @todo
    return std::nullopt;
}


// ----------------------------------------------------------------------------
}// end::namespace::creola
// ----------------------------------------------------------------------------