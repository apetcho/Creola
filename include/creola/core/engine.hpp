#pragma once

#include "creola/core/expr.hpp"
#include "creola/core/pprint.hpp"

#include<iostream>
#include<iomanip>
#include<mutex>
#include<set>

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola::core                               -*-
// -*----------------------------------------------------------------*-
namespace creola::core {
//

// -*-
class Creola final : public PrintVisitor {
public:
    // - handle_line()
    Result run(const std::string& src);
    // - parse_expression_only()
    Expr parse(const std::string& src);

    // -
    static std::string ps1;
    static std::string ps2;
    static std::set<std::string> keywords;
    static std::set<std::string> builtin_commands;
    static void define_keywords(void);
    static void define_builtin_commands(void);
    // -
    static Shared<Creola> app;
    static std::mutex app_mtx;

    static void display(std::ostream& os, const Number& expr, [[maybe_unused]] int prec=0);
    static void display(std::ostream& os, const Symbol& expr, [[maybe_unused]] int prec=0);
    static void display(std::ostream& os, const Add& expr, [[maybe_unused]] int prec=0);
    static void display(std::ostream& os, const Mul& expr, [[maybe_unused]] int prec=0);
    static void display(std::ostream& os, const Pow& expr, [[maybe_unused]] int prec=0);
    static void display(std::ostream& os, const Neg& expr, [[maybe_unused]] int prec=0);
    static void display(std::ostream& os, const FuncCall& expr, [[maybe_unused]] int prec=0);

    static void display(std::ostream& os, const Expr& expr, [[maybe_unused]] int prec=0);

    // -
    static void print(std::ostream& os){}
    
    static void print(std::ostream& os, const Expr& expr){
        Creola::display(os, expr);
    }
    
    template <typename T, typename... Args>
    static void print(std::ostream& os, T first, Args... rest) {
        os << first << " ";
        Creola::print(os, first);
        Creola::print(os, rest...);
    }
    
    static void println(){}
    static void println(std::ostream& os){
        os << std::endl;
    }
    
    static void println(std::ostream& os, const Expr& expr){
        Creola::display(os, expr);
        os << std::endl;
    }
    
    template <typename T, typename... Args>
    static void println(std::ostream& os, T first, Args... rest) {
        Creola::print(os, first);
        Creola::println(os, rest...);
        //os << std::endl;
    }

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

    void validate_var(const std::string& var){
        if(this->m_vars.find(var)==this->m_vars.end()){
            std::stringstream ess;
            ess << "undefined variable " << std::quote(var);
            throw CreolaError(ess.str());
        }
    }

    void validate_func(const std::string& func){
        if(this->m_funcs.find(func)==this->m_funcs.end()){
            std::stringstream ess;
            ess << "undefined function named " << std::quote(var);
            throw CreolaError(ess.str());
        }
    }

    Expr handle_expr(const Expr& expr){
        if(Creola::is_symbol_expr(expr)){// we have a symbol that represent either a variable or a function name.
            Symbol sym{""};
            Creola::as(expr, sym);
            if(sym.name().find('(')!=std::string::npos){
                // sym.name() is the name of a function;
                this->m_validate_func(sym.name());
                return this->m_funcs[sym.name()];
            }
            // otherwise sym.name() is the name of a variable
            this->m_validate_var(sym.name());
            return this->m_vars[sym.name()];
        }
        // expr is just a freshly constructed expression for raw string.
        return expr;
    }

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

    //! @todo Define default symbols including `→`, `-∞`, `+∞`, `ℝ`, etc.

    static bool is_fraction(const Expr& expr, Expr& num, Expr& den){
        //! @todo
        return false;
    }

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

    // ----------------------
    // -*- HELPER METHODS -*-
    // ----------------------
    /**
     * @brief Remove a command `cmd' from the beginning of src.
     * 
     * @code[.cpp]
     * Creola creola;
     * std::string src = "let x = 10";
     * auto out = creola.trim_command(src);
     * std::cout << out << std::endl; // out: x = 10
     * @endcode 
     * 
     * @param src 
     * @param cmd 
     * @return std::string 
     */
    std::string trim_command(const std::string& src, const char* cmd);

    /**
     * @brief Process on of the builtin commands.
     * 
     * For the set of all builtin command, @see{Creola::builtin_commands}
     * 
     * @param src 
     */
    Expr process_command(const std::string& src, Vec<Expr>& vecResult);

    /**
     * @brief Parse and evaluate a statement starting with one of the builtin keywords
     * 
     * The builtin keyword include "let" and "fun".
     * 
     * @param src 
     */
    Vec<Expr> process_keyword(const std::string& src);

    /**
     * @brief Parse let statement
     * 
     * Syntax:
     * -------
     *      let var = value
     * 
     * @param src 
     */
    Vec<Expr> handle_keyword_let(const std::string& src);

    /**
     * @brief Parse the `fun` statement.
     * 
     * Syntax:
     * -------
     *      fun name(params) = expr
     * 
     * Example:
     * -------
     *      creola> fun f(x) = x^2 + 2x + 1
     *      ======> f(x) = x^2 + 2x + 1
     * 
     * @param src 
     */
    Vec<Expr> handle_keyword_fun(const std::string& src);

    /**
     * @brief Parse the `simplify` command expression.
     * 
     * Syntax:
     * -------
     *      simplify(expr)
     * 
     * Example:
     * --------
     *      creola> simplify(x - 1 + 2)
     *      ======> x + 1
     *      creola> simplify(sin^2(x) + cos^2(x))
     *      ======> 1
     * 
     * @param src 
     */
    Expr handle_command_simplify(const std::string& src, Vec<Expr>& vecResult);

    /**
     * @brief Parse the `diff` command expression.
     * 
     * Syntax:
     * -------
     *      diff(expr, var)
     * 
     * Example:
     * --------
     *      creola> diff(x^2 + 1, x)
     *      ======> 2x
     *      creola> diff(cos(x), x)
     *      ======> -sin(x)
     * 
     * @param src 
     */
    Expr handle_command_diff(const std::string& src, Vec<Expr>& vecResult);

    /**
     * @brief Parse the `integrate` command expression.
     * 
     * Syntax:
     * -------
     *      (1) integrate(expr, var)
     *      (2) integrate(expr, var, vmin, vmax)
     * 
     * Example:
     * --------
     *      creola> integrate("x", x)
     *      ======> 1/2*x^2
     *      creola> integrate("2*x + y", x)
     *      ======> x^2 + y
     * 
     * @param src 
     */
    Expr handle_command_integrate(const std::string& src, Vec<Expr>& vecResult);

    /**
     * @brief Parse the `taylor` command expression.
     * 
     * Syntax:
     * ------
     *      taylor(expr, var, center, order)
     * 
     * Example:
     * -------
     *      creola> taylor("exp(x)", x, 0, 3)
     *      ======> 1 + x + x^2/2 + x^3/6
     * 
     * @param src 
     */
    Expr handle_command_taylor(const std::string& src, Vec<Expr>& vecResult);

    /**
     * @brief Parse the `expand` command expression.
     * 
     * Syntax:
     * ------
     *      expand(expression)
     * 
     * Example:
     * --------
     *      creola> expand("(x-1)*(x+1)")
     *      ======> x^2 - 1
     * 
     * @param src 
     * @param vecResult 
     * @return Expr 
     */
    Expr handle_command_expand(const std::string& src, Vec<Expr>& vecResult);

    /**
     * @brief Parser the command `factor` expression.
     * 
     * Syntax:
     * ------
     *      factor(expr, var)
     * 
     * Example:
     * --------
     *      creola> factor("x^2 - 1", x)
     *      ======> (x-1)(x+1)
     *      creola> factor("x^2 - 1", y)
     *      ======> x^2 - 1
     * 
     * @param src 
     * @param vecResult 
     * @return Expr 
     */
    Expr handle_command_factor(const std::string& src, Vec<Expr>& vecResult);

    /**
     * @brief Parse the command `limit` expression.
     * 
     * Syntax:
     * -------
     *      limit(expr, var, val)
     * 
     * Example:
     * -------
     *      creola> limit("ln(x)/x", x, 0)
     *      ======> +∞
     * 
     * @param src 
     * @param vecResult 
     * @return Expr 
     */
    Expr handle_command_limit(const std::string& src, Vec<Expr>& vecResult);

    //! @brief implement the helper method `handle_groebner()`
    Expr handle_command_groebner(const std::string& src, Vec<Expr>& vecResult);

    //! @brief implement the helper method `handle_rewrite()`
    Expr handle_command_rewrite(const std::string& src, Vec<Expr>& vecResult);

    //! @brief implement the helper method `handle_roots()`
    Expr handle_command_roots(const std::string& src, Vec<Expr>& vecResult);

    //! @brief implement the helper method `handle_solve()`
    Expr handle_command_solve(const std::string& src, Vec<Expr>& vecResult);

    //! @brief implement the helper method `handle_solve_system()`
    Expr handle_command_solve_system(const std::string& src, Vec<Expr>& vecResult);

    //! @brief implement the helper method `handle_parfrac()`
    Expr handle_command_partfrac(const std::string& src, Vec<Expr>& vecResult);
    Expr handle_command_equation(const std::string& src, Vec<Expr>& vecResult);
    Expr handle_command_system(const std::string& src, Vec<Expr>& vecResult);
    Expr handle_command_factorial(const std::string& src, Vec<Expr>& vecResult);
    Expr handle_command_fibonacci(const std::string& src, Vec<Expr>& vecResult);
    Expr handle_command_help(const std::string& src, Vec<Expr>& vecResult);
    Expr handle_command_print(const std::string& src, Vec<Expr>& vecResult);
    Expr handle_command_config(const std::string& src, Vec<Expr>& vecResult);
    Expr handle_command_show(const std::string& src, Vec<Expr>& vecResult);
};

// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola::core                                  -*-
// -*----------------------------------------------------------------*-