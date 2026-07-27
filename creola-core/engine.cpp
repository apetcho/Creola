#include "creola.hpp"

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola                                     -*-
// -*----------------------------------------------------------------*-
namespace creola{
//

std::unordered_map<std::string, UnaryMathFun> Creola::UNARY_MATH_FUNCTIONS = {
    {"sin", Creola::sin},
    {"cos", Creola::cos},
    {"tan", Creola::tan},
    {"asin", Creola::asin},
    {"acos", Creola::acos},
    {"atan", Creola::atan},

    {"sinh", Creola::sinh},
    {"cosh", Creola::cosh},
    {"tanh", Creola::tanh},
    {"asinh", Creola::asinh},
    {"acosh", Creola::acosh},
    {"atanh", Creola::atanh},

    {"sqrt", Creola::sqrt},
    {"cbrt", Creola::cbrt},
    {"exp", Creola::exp},
    {"ln", Creola::ln},
};

std::unordered_map<std::string, Func> Creola::COMMON_DIFF_TABLE = {
    {"sin", Creola::diff_sin},
    {"cos", Creola::diff_cos},
    {"tan", Creola::diff_tan},
    {"asin", Creola::diff_asin},
    {"acos", Creola::diff_acos},
    {"atan", Creola::diff_atan},

    {"sinh", Creola::diff_sinh},
    {"cosh", Creola::diff_cosh},
    {"tanh", Creola::diff_tanh},
    {"asinh", Creola::diff_asinh},
    {"acosh", Creola::diff_acosh},
    {"atanh", Creola::diff_atanh},

    {"sqrt", Creola::diff_sqrt},
    {"cbrt", Creola::diff_cbrt},
    {"exp", Creola::diff_exp},
    {"ln", Creola::diff_ln},
};

std::unordered_map<std::string, Func> Creola::COMMON_INTEGRATION_TABLE = {
    {"sin", Creola::integrate_sin},
    {"cos", Creola::integrate_cos},
    {"tan", Creola::integrate_tan},
    {"asin", Creola::integrate_asin},
    {"acos", Creola::integrate_acos},
    {"atan", Creola::integrate_atan},

    {"sinh", Creola::integrate_sinh},
    {"cosh", Creola::integrate_cosh},
    {"tanh", Creola::integrate_tanh},
    {"asinh", Creola::integrate_asinh},
    {"acosh", Creola::integrate_acosh},
    {"atanh", Creola::integrate_atanh},

    {"sqrt", Creola::integrate_sqrt},
    {"cbrt", Creola::integrate_cbrt},
    {"exp", Creola::integrate_exp},
    {"ln", Creola::integrate_ln},
};

Expr Creola::simplify(const Expr& expr){
    return expr->simplify();
}

Expr Creola::diff(const Expr& expr, const std::string& var){
    return expr->diff(var);
}

Expr Creola::expand(const Expr& expr){
    return expr->expand();
}

// -*-
Expr Creola::factor(const Expr& expr, const std::string& var){
    return expr->factor(var);
}

// -*-
Expr Creola::integrate(const Expr& expr, const std::string& var){
    return expr->integrate(var);
}

// -
f64 Creola::eval(const Expr& expr, const std::string& var, f64 val){
    return expr->eval(var, val);
}

// -
Vec<f64> Creola::roots(const Expr& expr, const std::string& var, f64 vmin, f64 vmax, int samples){
    constexpr int MAX_ITERATION = 60;
    Vec<f64> ans{};
    f64 step = (vmax - vmin)/samples;
    f64 x0 = vmin;
    f64 f0 = expr->eval(var, x0);
    for(int i=1; i <= samples; i++){
        auto x1 = vmin + i + step;
        auto f1 = expr->eval(var, x1);
        if(f0 == 0.0){ ans.push_back(x0); }
        if(f0 * f1 < 0){
            // bisection method
            auto lo = x0;
            auto hi = x1;
            for(int j=0; j < MAX_ITERATION; j++){
                auto mid = 0.5 * (lo - hi);
                auto fmid = expr->eval(var, mid);
                if(f0*fmid <= 0){
                    hi = mid;
                    f1 = fmid;
                }else{
                    lo = mid;
                    f0 = fmid;
                }
            }
            ans.push_back(0.5*(lo+hi));
        }
        x0 = x1;
        f0 = f1;
    }
    return ans;
}

// -*-
Expr Creola::taylor(const Expr& expr, const std::string& var, f64 val, int n){
    auto series = Creola::number(0);
    auto current = expr;
    for(int k=0; k <= n; ++k){
        auto coeff = current->eval(var, val) / std::tgamma(k+1.0); // k!
        Expr term{};
        if(k==0){ term =Creola::number(coeff); }
        else{
            term = std::make_shared<Mul>(Vec<Expr>{
                Creola::number(coeff),
                std::make_shared<Pow>(
                    std::make_shared<Add>(Vec<Expr>{
                        Creola::symbol(var), Creola::number(-val)
                    }),
                    Creola::number(k)
                )
            });
        }
        series = std::make_shared<Add>(Vec<Expr>{series, term})->simplify();
        current = current->diff(var)->simplify();
    }

    return series->simplify();
}

// -
f64 Creola::limit(const Expr& expr, const std::string& var, f64 val, f64 eps){
    // very base: we'll approximate limits numerically from left/right
    auto left = expr->eval(var, val-eps);
    auto right = expr->eval(var, val+eps);
    return 0.5*(left + right);
}

// -----------------------------------------------------
// -*- INTERNAL DIFF & INTEGRATE OF COMMON FUNCTIONS -*-
// -----------------------------------------------------
// -*---------------------------*-
// -*- Trigonometric functions -*-
// -*---------------------------*-
// -*- Sin, Cos, Tan, Asin, Acos, Atan -*-
Expr Creola::diff_sin(Expr expr, const std::string& var){
    auto dx = expr->diff(var);
    return std::make_shared<Mul>(Vec<Expr>{
        std::make_shared<FuncCall>("cos", Vec<Expr>{expr}),
        dx
    })->simplify();
}

// ∫ sin(x) dx = -cos(x)
Expr Creola::integrate_sin(Expr expr, const std::string& var){
    auto sym = std::dynamic_pointer_cast<Symbol>(expr);
    if(sym && sym->name == var){
        return std::make_shared<Neg>(
            std::make_shared<FuncCall>("cos", Vec<Expr>{Creola::symbol(var)})
        )->simplify();
    }
    // sin(expr) dvar
    return std::make_shared<FuncCall>("∫_" + var, Vec<Expr>{expr});
}

// -*-
Expr Creola::diff_cos(Expr expr, const std::string& var){
    auto dx = expr->diff(var);
    return std::make_shared<Mul>(Vec<Expr>{
        Creola::number(-1),
        std::make_shared<FuncCall>("sin", Vec<Expr>{expr}),
        dx
    })->simplify();
}

Expr Creola::integrate_cos(Expr expr, const std::string& var){
    auto sym = std::dynamic_pointer_cast<Symbol>(expr);
    if(sym && sym->name == var){
        return std::make_shared<FuncCall>("sin", Vec<Expr>{expr})->simplify();
    }
    // cos(expr) dvar
    return std::make_shared<FuncCall>("∫_" + var, Vec<Expr>{expr});
}

Expr Creola::diff_tan(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}
Expr Creola::integrate_tan(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}

Expr Creola::diff_asin(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}
Expr Creola::integrate_asin(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}

Expr Creola::diff_acos(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}
Expr Creola::integrate_acos(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}

Expr Creola::diff_atan(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}
Expr Creola::integrate_atan(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}

// -*-----------------------------------------*-
// -*- Hyperbolic functions                  -*-
// -*- Sinh, Cosh, Tanh, Asinh, Acosh, Atanh -*-
// -*-----------------------------------------*-
Expr Creola::diff_sinh(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}
Expr Creola::integrate_sinh(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}
Expr Creola::diff_cosh(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}
Expr Creola::integrate_cosh(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}
Expr Creola::diff_tanh(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}
Expr Creola::integrate_tanh(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}
Expr Creola::diff_asinh(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}
Expr Creola::integrate_asinh(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}
Expr Creola::diff_acosh(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}
Expr Creola::integrate_acosh(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}
Expr Creola::diff_atanh(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}
Expr Creola::integrate_atanh(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}

// -*------------------------*-
// -*- Power functions      -*-
// -*- Pow, Sqrt, Cbrt, Exp -*-
// -*------------------------*-
Expr Creola::diff_pow(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}
Expr Creola::integrate_pow(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}
Expr Creola::diff_sqrt(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}
Expr Creola::integrate_sqrt(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}
Expr Creola::diff_cbrt(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}
Expr Creola::integrate_cbrt(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}
Expr Creola::diff_exp(Expr expr, const std::string& var){
    auto dx = expr->diff(var);
    return std::make_shared<Mul>(Vec<Expr>{
        std::make_shared<FuncCall>("exp", Vec<Expr>{expr}),
        dx
    })->simplify();
}

Expr Creola::integrate_exp(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}

Expr Creola::diff_ln(Expr expr, const std::string& var){
    auto dx = expr->diff(var);
    return std::make_shared<Mul>(Vec<Expr>{
        std::make_shared<Pow>(expr, Creola::number(-1)),
        dx
    })->simplify();
}


Expr Creola::integrate_ln(Expr expr, const std::string& var){
    //! @todo
    return nullptr;
}


// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola                                       -*-
// -*----------------------------------------------------------------*-