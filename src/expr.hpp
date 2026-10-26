/**
 * @file expr.hpp
 * @author Eyram K. Apetcho (you@domain.com)
 * @brief Symbolic expression core: immutatable canonicalising expression trees.
 * @version 0.1
 * @date 2026-10-23
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#pragma once

#include<memory>
#include<optional>
#include<complex>
#include<cstdint>
#include<cmath>
#include<vector>
#include<string>
#include<set>

#include "typedefs.hpp"

// ----------------------------------------------------------------------------
// begin::namespace::creola
// ----------------------------------------------------------------------------
namespace creola{
// -
enum class Kind {
    Num,
    Sym,
    Add,
    Mul,
    Pow,
    Fun,
};


struct Node {
    Kind kind{};
    f64 value{0.0};          // Num
    std::string name;           // Sym / Fun
    std::vector<Expr> args;     // Add, Mul, Pow(base, expr), Fun(args)
};

inline bool isInteger(f64 val){
    return std::isfinite(val) && val == std::floor(val);
}

/// Smart constructors: always return canonical form (flattened, folded, sorted,
/// loke terms collected).
Expr make_number(f64 val);
Expr make_symbol(const std::string& name);
Expr make_add(std::vector<Expr> terms);
Expr make_mul(std::vector<Expr> factors);
Expr make_power(const Expr& base, const Expr& exp);
Expr make_fun(const std::string& name, const Expr& args);

bool isNumber(const Expr& expr);
bool isNumber(const Expr& expr, f64 val);
bool contains(const Expr& expr, const std::string& var);

std::string str(const Expr& expr);
void setPrecision(i32 digits);

std::optional<f64> numeric(const Expr& expr);
std::optional<f64> evalFun(const std::string& name, f64 x);

/**
 * @brief Transform a floating-point number into a fractional number.
 * 
 * Return std::nullopt is the argument `val` is an integer or infinite, or if
 * the rounding noise does not fall under EPS [default: 1e-9] after MAX_ITERATIONS
 * [default: 1000]
 * 
 * @param val 
 * @return std::optional<std::pair<i64, i64>> 
 */
std::optional<std::pair<i64, i64>> asFraction(f64 val);

extern f64 g_displayTol;

std::optional<f64> numericAt(const Expr& expr, const std::string& var, f64 x);
std::optional<Complex> numericC(const Expr& expr);
Expr imag(void);
Expr fromComplex(Complex z, f64 tol=1e-12);
void freeSymbols(const Expr& expr, std::set<std::string>& out);

Expr substitute(const Expr& expr, const std::string& var, const Expr& val);
Expr simplify(const Expr& expr);
Expr expand(const Expr& expr);
Expr diff(const Expr& expr, const std::string& var);
Expr integral(const Expr& expr, const std::string& var);
Expr taylor(const Expr& expr, const std::string& var, const Expr& center, i32 order);

// ----------------------------------------------------------------------------
}// end::namespace::creola
// ----------------------------------------------------------------------------

/*
// ----------------------------------------------------------------------------
// begin::namespace::creola
// ----------------------------------------------------------------------------
namespace creola{
// -

// ----------------------------------------------------------------------------
}// end::namespace::creola
// ----------------------------------------------------------------------------
*/