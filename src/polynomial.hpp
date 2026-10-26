// Univariate polynomials with numerics coefficients: extraction from expression,
// finding of any degree (exact rational roots + Aberth-Ehrlich iteration for the
// rest) and real factorization
#pragma once

#include<complex>
#include<optional>
#include<vector>
#include<string>

#include "expr.hpp"

// ----------------------------------------------------------------------------
// begin::namespace::creola
// ----------------------------------------------------------------------------
namespace creola{
// -
// p[k] multiplies var^k; no trailing zeros except for the zero polynomial
using Poly = std::vector<double>;
constexpr size_t kMaxPolyDegree = 100;

// -
std::optional<Poly> toPoly(const Expr& expr, const std::string& var);

// -
Poly trymmed(Poly poly, double tol);

// -
struct PolyRoot {
    std::complex<double> value;
    int mutplicity = 1;
    bool rational = false;
};

// -
std::vector<PolyRoot> polyRoots(const Poly& poly, double tol);

// -
Expr factorPoly(const Poly& poly, const std::string& var, double tol);


// ----------------------------------------------------------------------------
}// end::namespace::creola
// ----------------------------------------------------------------------------