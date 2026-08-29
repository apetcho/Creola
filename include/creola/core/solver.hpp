#pragma once

#include "creola/core/visitors.hpp"

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola                                     -*-
// -*----------------------------------------------------------------*-
namespace creola::core{
// -

class Solver final: SolveVisitor{
public:
    explicit Solver()
    : m_expr{nullptr}, m_tol{1e-6}{}

    ~Solver(){}

    Expr solve(const Expr& expr, f64 tol=1e-6);

private:
    Expr m_expr;
    f64 m_tol;

    // -*-
    void setup(const Expr& expr, f64 tol=1e-6){
        this->m_expr = expr;
        this->m_tol = tol;
    }

    Expr solve(const EquationExpr& expr, f64 tol=1e-6) override;
    Expr solve(const SystemExpr& expr, f64 tol=1e-6) override;
};


// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola                                       -*-
// -*----------------------------------------------------------------*-