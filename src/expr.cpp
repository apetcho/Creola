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

    bool atomic(const Expr& expr){
        //! @todo
        return false;
    }

    Expr negate(const Expr& expr){
        //! @todo
        return nullptr;
    }
} // namespace

// -
Expr number(f64 val){
    //! @todo
    return nullptr;
}

// -
Expr symbol(const std::string& name){
    //! @todo
    return nullptr;
}

// -
bool isNumber(const Expr& expr){
    //! @todo
    return false;
}

// -
bool isNumber(const Expr& expr, f64 val){
    //! @todo
    return false;
}

// -
Expr add(std::vector<Expr> terms){
    //! @todo
    return nullptr;
}

// -
Expr mul(std::vector<Expr> factors){
    //! @todo
    return nullptr;
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