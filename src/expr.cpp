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
        return mul({number(-1), expr});
    }
} // namespace

// -
Expr number(f64 val){
    return make(Kind::Num, val==0 ? 0.0 : val, {}, {});
}

// -
Expr symbol(const std::string& name){
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
Expr add(std::vector<Expr> terms){
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
        out.emplace_back(key, pair.first == 1 ? pair.second : mul({number(pair.first), pair.second}));
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
        myTerms.push_back(number(c));
    }
    myTerms.insert(myTerms.end(), myImaginary.begin(), myImaginary.end());
    if(myTerms.empty()){ return number(0); }
    if(myTerms.size()==1){ return myTerms[0]; }
    return make(Kind::Add, 0, {}, myTerms);
}

// -
Expr mul(std::vector<Expr> factors){
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

    if(c == 0){ number(0); }
    std::vector<std::pair<std::string, Expr>> out;
    for(auto& [key, pair]: g){
        if(pair.second==0){ continue; }
        if(pair.first->kind==Kind::Sym && pair.first->name == "i" && isInteger(pair.second)){
            auto k = static_cast<i32>(((static_cast<i64>(pair.second) % 4) + 4) % 4); // i^2 = -i
            if(k >= 2){ c = -c; }
            if(k % 2){ out.emplace_back("i", pair.first); }
            continue;
        }
        Expr f = power(pair.first, number(pair.second));
        out.emplace_back(str(f), f);
    }
    std::sort(out.begin(), out.end(), [](auto& a, auto& b){
        return a.first < b.first;
    });
    std::vector<Expr> myTerms;
    if(c != 1 || out.empty()){ myTerms.push_back(number(c)); }
    for(auto& item: out){
        myTerms.push_back(item.second);
    }
    if(myTerms.size() == 1){ return myTerms[0]; }
    return make(Kind::Mul, 0, {}, myTerms);
}

// -
Expr power(const Expr& base, const Expr& exp){
    //! @todo
    return nullptr;
}

// -
std::optional<f64> evalFun(const std::string& name, f64 x){
    //! @todo
    return std::nullopt;
}

// -
Expr fun(const std::string& name, const Expr& args){
    //! @todo
    return nullptr;
}

bool contains(const Expr& expr, const std::string& var){
    //! @todo
    return false;
}

// -
std::optional<f64> numeric(const Expr& expr){
    //! @todo
    return std::nullopt;
}

// -
void setPrecision(i32 digits){
    //! @todo
}

// -
std::string str(const Expr& expr){
    //! @todo
    return "";
}

// -
Expr subs(const Expr& expr, const std::string& var, const Expr& val){
    //! @todo
    return nullptr;
}

// -
Expr simplify(const Expr& expr){
    //! @todo
    return nullptr;
}

static Expr mulExpr(const Expr& lhs, const Expr& rhs){
    //! @todo
    return nullptr;
}

// -
Expr expand(const Expr& expr){
    //! @todo
    return nullptr;
}

// -
Expr diff(const Expr& expr, const std::string& var){
    //! @todo
    return nullptr;
}

static Expr my_integral(const Expr& expr, const std::string& var){
    //! @todo
    return nullptr;
}

// -
Expr integral(const Expr& expr, const std::string& var){
    //! @todo
    return nullptr;
}

// -
Expr taylor(const Expr& expr, const std::string& var, const Expr& center, i32 order){
    //! @todo
    return nullptr;
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