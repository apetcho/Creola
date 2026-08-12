#pragma once

#include "creola/core/expr.hpp"
#include "creola/core/pprint.hpp"
#include<mutex>

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola::core                               -*-
// -*----------------------------------------------------------------*-
namespace creola::core {
//

// -*-
class Creola final : public PrintVisitor {
public:

    // - handle_line()
    void run(const std::string& src);
    // - parse_expression_only()
    Expr parse(const std::string& src);

    // -
    static Shared<Creola> app;
    static std::mutex app_mtx;

    static void display(std::ostream& os, const ExprBase& expr, [[maybe_unused]] int prec=0);

    void visit(std::ostream& os, const Number& expr, int prec=0) const override;
    void visit(std::ostream& os, const Symbol& expr, int prec=0) const override;
    void visit(std::ostream& os, const Neg& expr, int prec=0) const override;
    void visit(std::ostream& os, const Add& expr, int prec=0) const override;
    void visit(std::ostream& os, const Mul& expr, int prec=0) const override;
    void visit(std::ostream& os, const Pow& expr, int prec=0) const override;
    void visit(std::ostream& os, const FuncCall& expr, int prec=0) const override;

private:
    // user-defined variables & functions
    HashMap<std::string, Expr> m_vars;
    HashMap<std::string, FunctionDef> m_funcs;

    // -
    Expr substitute(const Expr& expr, const std::string& var, const Expr& val);
    // apply_user_func ==> apply 
    Expr apply(const std::string& name, const Vec<Expr>& args);

    // -*-
    static bool is_number_expr(const Expr& expr);
    static bool is_symbol_expr(const Expr& expr);
    static bool is_neg_expr(const Expr& expr);
    static bool is_add_expr(const Expr& expr);
    static bool is_mul_expr(const Expr& expr);
    static bool is_pow_expr(const Expr& expr);
    static bool is_call_expr(const Expr& expr);

    // -
    static void as(Expr expr, Number& num);
    static void as(Expr expr, Symbol& num);
    static void as(Expr expr, Neg& num);
    static void as(Expr expr, Add& num);
    static void as(Expr expr, Mul& num);
    static void as(Expr expr, Pow& num);
    static void as(Expr expr, FuncCall& num);

public:
    static std::unordered_map<std::string, UnaryMathFun> UNARY_MATH_FUNCTIONS;
    static std::unordered_map<std::string, Func> COMMON_DIFF_TABLE;
    static std::unordered_map<std::string, Func> COMMON_INTEGRATION_TABLE;

    static Expr number(f64 val);
    static Expr symbol(const std::string& var);
    static bool is_zero(const Expr& expr);
    static bool is_one(const Expr& expr);

    // -----------------------------------
    // -*- High level helper functions -*-
    // -----------------------------------
    static Expr simplify(const Expr& expr);
    static Expr diff(const Expr& expr, const std::string& var);
    static Expr expand(const Expr& expr);
    static Expr factor(const Expr& expr, const std::string& var);
    static Expr integrate(const Expr& expr, const std::string& var);
    static f64 eval(const Expr& expr, const std::string& var, f64 val);
    static Vec<f64> roots(const Expr& expr, const std::string& var, f64 vmin=-10, f64 vmax=10, int samples=200);
    static Expr taylor(const Expr& expr, const std::string& var, f64 val, int n);
    static f64 limit(const Expr& expr, const std::string& var, f64 val, f64 eps=1e-6);


private:
    static inline Expr sin(f64 x){ return number(std::sin(x)); }
    static inline Expr cos(f64 x){ return number(std::cos(x)); }
    static inline Expr tan(f64 x){ return number(std::tan(x)); }
    static inline Expr asin(f64 x){ return number(std::asin(x)); }
    static inline Expr acos(f64 x){ return number(std::acos(x)); }
    static inline Expr atan(f64 x){ return number(std::atan(x)); }

    static inline Expr sinh(f64 x){ return number(std::sinh(x)); }
    static inline Expr cosh(f64 x){ return number(std::cosh(x)); }
    static inline Expr tanh(f64 x){ return number(std::tanh(x)); }
    static inline Expr asinh(f64 x){ return number(std::asinh(x)); }
    static inline Expr acosh(f64 x){ return number(std::acosh(x)); }
    static inline Expr atanh(f64 x){ return number(std::atanh(x)); }

    static inline Expr sqrt(f64 x){ return number(std::sqrt(x)); }
    static inline Expr cbrt(f64 x){ return number(std::cbrt(x)); }
    static inline Expr exp(f64 x){ return number(std::exp(x)); }
    static inline Expr ln(f64 x){ return number(std::log(x)); }

    // -*---------------------------*-
    // -*- Trigonometric functions -*-
    // -*---------------------------*-
    // -*- Sin, Cos, Tan, Asin, Acos, Atan -*-
    static Expr diff_sin(Expr expr, const std::string& var);
    static Expr integrate_sin(Expr expr, const std::string& var);

    static Expr diff_cos(Expr expr, const std::string& var);
    static Expr integrate_cos(Expr expr, const std::string& var);

    static Expr diff_tan(Expr expr, const std::string& var);
    static Expr integrate_tan(Expr expr, const std::string& var);

    static Expr diff_asin(Expr expr, const std::string& var);
    static Expr integrate_asin(Expr expr, const std::string& var);

    static Expr diff_acos(Expr expr, const std::string& var);
    static Expr integrate_acos(Expr expr, const std::string& var);

    static Expr diff_atan(Expr expr, const std::string& var);
    static Expr integrate_atan(Expr expr, const std::string& var);
    
    // Hyperbolic functions
    // -*- Sinh, Cosh, Tanh, Asinh, Acosh, Atanh -*-
    static Expr diff_sinh(Expr expr, const std::string& var);
    static Expr integrate_sinh(Expr expr, const std::string& var);
    static Expr diff_cosh(Expr expr, const std::string& var);
    static Expr integrate_cosh(Expr expr, const std::string& var);
    static Expr diff_tanh(Expr expr, const std::string& var);
    static Expr integrate_tanh(Expr expr, const std::string& var);
    static Expr diff_asinh(Expr expr, const std::string& var);
    static Expr integrate_asinh(Expr expr, const std::string& var);
    static Expr diff_acosh(Expr expr, const std::string& var);
    static Expr integrate_acosh(Expr expr, const std::string& var);
    static Expr diff_atanh(Expr expr, const std::string& var);
    static Expr integrate_atanh(Expr expr, const std::string& var);


    // Power functions
    // -*- Pow, Sqrt, Cbrt, Exp -*-
    static Expr diff_pow(Expr expr, const std::string& var);
    static Expr integrate_pow(Expr expr, const std::string& var);
    static Expr diff_sqrt(Expr expr, const std::string& var);
    static Expr integrate_sqrt(Expr expr, const std::string& var);
    static Expr diff_cbrt(Expr expr, const std::string& var);
    static Expr integrate_cbrt(Expr expr, const std::string& var);
    static Expr diff_exp(Expr expr, const std::string& var);
    static Expr integrate_exp(Expr expr, const std::string& var);
    static Expr diff_ln(Expr expr, const std::string& var);
    static Expr integrate_ln(Expr expr, const std::string& var);
};

// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola::core                                  -*-
// -*----------------------------------------------------------------*-