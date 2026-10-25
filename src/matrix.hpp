/**
 * @file matrix.hpp
 * @author your name (you@domain.com)
 * @brief Dense complex matrices and numerical linear algebra.
 * @version 0.1
 * @date 2026-10-23
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#pragma once

#include<complex>
#include<vector>


// ----------------------------------------------------------------------------
// begin::namespace::creola
// ----------------------------------------------------------------------------
namespace creola{
// -
using Complex = std::complex<double>;


struct Matrix {
    int r{0};
    int c{0};

    std::vector<Complex> store;

    Matrix() = default;
    Matrix(int rows, int cols);
    Complex& at(int i, int j);
    const Complex& at(int i, int j) const;

    Matrix operator+(const Matrix& rhs) const;
    Matrix operator-(const Matrix& rhs) const;
    Matrix operator*(const Matrix& rhs) const;
    Matrix operator*(const Complex& factor) const;  // scale
    Matrix transpose(void) const;
    Matrix operator^(unsigned long n) const;        // power
    Matrix inverse(void) const;
    Complex determinant(void) const;
    Complex trace(void) const;
    int rank(double tol=1e-10) const;

    static Matrix identity(unsigned int n);
};

// Matrix operator+(const Matrix& lhs, const Matrix& rhs);
// Matrix operator-(const Matrix& lhs, const Matrix& rhs);
// Matrix operator*(const Matrix& lhs, const Matrix& rhs);
// Matrix scale(const Matrix& mat, Complex& factor);
// Matrix transpose(const Matrix&);
// Matrix identity(int n);
// Matrix power(const Matrix& mat, long long n); // n >= 0
// Matrix inverse(const Matrix& mat);
// Complex determinant(const Matrix& mat);
// Complex trace(const Matrix& mat);

/// @brief SVD decompositon
struct SVD {
    Matrix U;
    std::vector<double> S;
    Matrix V;

    // -
    void operator()(const Matrix& mat);
};

// SVD svd(const Matrix& mat);
// int rank(const Matrix& mat);

struct Eigen {
    std::vector<Complex> values;
    Matrix vectors;

    void operator()(const Matrix& mat, double tol=1e-9);
};

//Eigen eigen(const Matrix& mat, double tol=1e-9);

// ----------------------------------------------------------------------------
}// end::namespace::creola
// ----------------------------------------------------------------------------
