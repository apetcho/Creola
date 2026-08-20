#pragma once

#include "creola/core/common.hpp"
#include "creola/core/ast.hpp"
#include "creola/core/polynomial.hpp"

//! @note: Implement a simplified Gröbner factorization
/*
Algorithm
---------
We get a **usable core**:

- Leading monomial and coefficient
- Division of polynomials by a set (mutlivariate division).
- S-polynomial
- Buchberger loop to compute a Gröbner basis.

Once we have that, we can:

- Use Gröbner bases to detect factors and perform more robust mutlivariate
factorization (e.g. via splitting ideals).

*/

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola                                     -*-
// -*----------------------------------------------------------------*-
namespace creola::core{
//

struct Monomial{
    // e.g. x^2 y^1 -> {{"x", 2}, {"y", 1}}
    Dict<std::string, int> exps; // sorted by variable name

    bool operator<(const Monomial& rhs) const;

    bool divides(const Monomial& rhs) const;
    Monomial quotient(const Monomial& rhs) const;
    Monomial lcm(const Monomial& rhs) const;
};

class MultivariatePolynomial{
public:

    MultivariatePolynomial operator+(const MultivariatePolynomial& rhs) const;
    MultivariatePolynomial operator*(const MultivariatePolynomial& rhs) const;

    static bool from_expr(const Expr& expr, MultivariatePolynomial& poly);
    Expr to_expr(void) const;

    Monomial leading_monomial(void) const;
    Rational leading_coeff(void) const;

    static Vec<MultivariatePolynomial> groebner_basis(Vec<MultivariatePolynomial> mpoly_vec);

private:
    // sum c_m  * m
    Dict<Monomial, Rational> terms;     // monomial -> coeff

    MultivariatePolynomial mul_monomial(const Monomial& m, const Rational& ceoff) const;
    MultivariatePolynomial s_polynomial(const MultivariatePolynomial& rhs) const;
    MultivariatePolynomial reduce(const Vec<MultivariatePolynomial>& mpoly_vec) const;
};




// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola                                       -*-
// -*----------------------------------------------------------------*-