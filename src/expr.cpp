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

    std::pair<double, Expr> split(const Expr& expr){
        //! @todo
        return {};
    }
} // namespace

// -
double g_displayTol = 1e-12;

// -
std::optional<std::pair<i64, i64>> asFraction(f64 val){
    //! @todo
    return std::nullopt;
}

namespace {
    i32 g_precision = 10;

    // -
    std::optional<std::pair<i64, i64>> frac(f64 val){
        //! @todo
        return std::nullopt;
    }

    // -
    std::string numstr(f64 v){
        //! @todo
        return "";
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