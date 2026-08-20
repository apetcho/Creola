#pragma once

#include "creola/core/visitors.hpp"

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola                                     -*-
// -*----------------------------------------------------------------*-
namespace creola::core{
// -

class Solver final: SolveVisitor{
public:
    explicit Solver(const Expr& expr, f64 tol=1e-6);
    explicit Solver(Expr&& expr, f64 tol=1e-6);

    Vec<Expr> solve(void);

private:
    Expr m_expr;
    f64 m_tol;

    Vec<Expr> solve(const EquationExpr& expr, f64 tol=1e-6) override;
    Vec<Expr> solve(const SystemExpr& expr, f64 tol=1e-6) override;
};


// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola                                       -*-
// -*----------------------------------------------------------------*-