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
#include<cmath>
#include<vector>
#include<string>
#include<set>

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

struct Node;
using Expr = std::shared_ptr<const Node>;

struct Node {
    Kind kind{};
    double value{0.0};          // Num
    std::string name;           // Sym / Fun
    std::vector<Expr> args;     // Add, Mul, Pow(base, expr), Fun(args)
};

inline bool isInteger(double val){
    return std::isfinite(val) && val == std::floor(val);
}

/// Smart constructors: always return canonical form (flattened, folded, sorted,
/// loke terms collected).
Expr number(double val);
Expr symbol(const std::string& name);
Expr add(std::vector<Expr> terms);
Expr mul(std::vector<Expr> factors);
Expr power(const Expr& base, const Expr& exp);
Expr fun(const std::string& name, const Expr& args);

bool isNumber(const Expr& expr);
bool isNumber(const Expr& expr, double val);
bool contains(const Expr& expr, const std::string& var);

std::string str(const Expr& expr);
void setPrecision(int digits);

std::optional<double> numeric(const Expr& expr);
std::optional<double> evalFun(const std::string& name, double x);

std::optional<std::pair<long long, long long>> asFraction(double val);

extern double g_displayTol;

std::optional<double> numericAt(const Expr& expr, const std::string& var, double x);
std::optional<std::complex<double>> numericC(const Expr& expr);
Expr imag(void);
Expr fromComplex(std::complex<double> z, double tol=1e-12);
void freeSymbols(const Expr& expr, std::set<std::string>& out);

Expr subs(const Expr& expr, const std::string& var, const Expr& val);
Expr simplify(const Expr& expr);
Expr expand(const Expr& expr);
Expr diff(const Expr& expr, const std::string& var);
Expr integral(const Expr& expr, const std::string& var);
Expr taylor(const Expr& expr, const std::string& var, const Expr& center, int order);

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