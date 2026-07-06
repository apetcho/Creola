#pragma once

#include "common.hpp"
#include "expr.hpp"



// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola                                     -*-
// -*----------------------------------------------------------------*-
namespace creola{
//

class Rational {
public:
    explicit Rational(long long num=0, long long d=1);

    static Rational from(long double);
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

    Rational operator+(const Rational& rhs) const;
    Rational operator-(const Rational& rhs) const;
    Rational operator*(const Rational& rhs) const;
    Rational operator/(const Rational& rhs) const;

    bool is_zero(void) const;
    bool is_one(void) const;

    long double to_float(void) const;


private:
    long long m_num{0LL};   // numerator
    long long m_den{1LL};   // denominator
};

// -*-
class UnivariatePolynomial{
public:
    explicit UnivariatePolynomial(const Vec<Rational>& coeffs);

    // - as_polynomial(...)
    static bool from_expr(const Expr& expr, const std::string& var, UnivariatePolynomial& poly);
    Expr to_expr(const std::string& var) const;

    Rational eval(const Rational& val) const;
    bool find_root(Rational& root) const;

    Vec<Vec<Rational>> factor(void) const;
    bool is_quadratic(void) const;

private:
    std::vector<Rational> m_coeffs;

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