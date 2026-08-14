#pragma once

#include "creola/core/common.hpp"
#include "creola/core/expr.hpp"


// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola                                     -*-
// -*----------------------------------------------------------------*-
namespace creola::core{
//

/*
Note:
-----
Since IEEE-754 floats are inherently rational, you can extract the exact numerator 
and denominator by manipulating the mantissa and exponent. The frexp function is useful
here to separate the normalized fraction from the power of 2.

#include <cmath>
#include <limits>
#include <iostream>

void split(double x, double *numerator, double *denominator) {
    if (!std::isfinite(x)) {
        *numerator = *denominator = 0.0;
        return;
    }
    int expo;
    // frexp returns value in [0.5, 1) and sets expo to power of 2
    double frac = std::frexp(x, &expo);
    
    // Normalize to integer range based on mantissa bits (53 for double)
    int bdigits = std::numeric_limits<double>::digits;
    *denominator = std::pow(2.0, bdigits);
    *numerator = frac * *denominator;
    
    // Adjust for the exponent from frexp
    expo -= bdigits;
    if (expo > 0) {
        *numerator *= std::pow(2.0, expo);
    } else if (expo < 0) {
        *denominator *= std::pow(2.0, -expo);
    }
    
    // Simplify by dividing by 2 while both are even
    while (std::fmod(*numerator, 2.0) == 0 && std::fmod(*denominator, 2.0) == 0) {
        *numerator /= 2.0;
        *denominator /= 2.0;
    }
}

int main() {
    double val = 0.5;
    double num, den;
    split(val, &num, &den);
    std::cout << val << " = " << num << " / " << den << std::endl;
    return 0;
}
*/


class Rational {
public:
    explicit Rational(i64 num=0, i64 d=1);

    static Rational from_float(f64);

    Rational operator+(const Rational& rhs) const;
    Rational operator-(const Rational& rhs) const;
    Rational operator*(const Rational& rhs) const;
    Rational operator/(const Rational& rhs) const;

    bool is_zero(void) const;
    bool is_one(void) const;

    f64 to_float(void) const;

    f64 numerator(void) const{ return this->m_num; }
    f64 denominator(void) const { return this->m_den; }

    friend Rational operator+(const Rational& lhs, const Rational& rhs);
    friend Rational operator-(const Rational& lhs, const Rational& rhs);
    friend Rational operator*(const Rational& lhs, const Rational& rhs);
    friend Rational operator/(const Rational& lhs, const Rational& rhs);

private:
    i64 m_num;  // numerator
    i64 m_den;  // denominator
};

// -*-
class Polynomial {
public:
    explicit Polynomial(const Vec<Rational>& coeffs);

    static bool collect_coefficients(const Expr& expr, const std::string& var, std::map<i32, Rational>& coeffs);
    static bool as_polynomial(const Expr& expr, const std::string& var, Polynomial& poly);
    Rational eval(const Rational& val) const;
    bool find_rational_root(Rational& root);
    Vec<Polynomial> factor_over_Q();
    Expr to_expr(const std::string& var) const;

    static Expr factor(const Expr& expr, const std::string& var); /// XXX: move to Engine

    
    Vec<Vec<Rational>> factor(void) const;
    bool is_quadratic(void) const;

    const Vec<Rational>& coefficient() const { return this->m_coeffs; }

private:
    Vec<Rational> m_coeffs;

    Vec<i64> divisors(i64 n);
};

// -*-
struct PolyFactor{
    Vec<Rational> coeffs;
    Expr to_expr(const std::string& var);
    static Vec<PolyFactor> factor_expr(const Expr& expr, const std::string& var);
    static Expr simplify_fraction_by_cancel(const Expr& expr, const std::string& var);
    static Expr limit_symbolic(const Expr&, const std::string& var, f64 val, int max_iter=5);
};




// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola                                       -*-
// -*----------------------------------------------------------------*-