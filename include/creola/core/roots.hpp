#pragma once

#include "creola/core/visitors.hpp"

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola                                     -*-
// -*----------------------------------------------------------------*-
namespace creola::core{
//

class RootsFinder final: protected RootsVisitor{
public:
    explicit RootsFinder(const Expr& expr, const Str& var, f64 tol=1e-6);
    explicit RootsFinder(Expr&& expr, Str&& var, f64 tol=1e-6);

    Vec<Expr> roots(void);

private:
    Expr m_expr;
    Str m_var;
    f64 m_tol;

    Vec<Expr> roots(const SymbolExpr& expr, const Str& var, f64 tol=1e-6) override;
    Vec<Expr> roots(const NumberExpr& expr, const Str& var, f64 tol=1e-6) override;
    Vec<Expr> roots(const NegExpr& expr, const Str& var, f64 tol=1e-6) override;
    Vec<Expr> roots(const AddExpr& expr, const Str& var, f64 tol=1e-6) override;
    Vec<Expr> roots(const MulExpr& expr, const Str& var, f64 tol=1e-6) override;
    Vec<Expr> roots(const PowExpr& expr, const Str& var, f64 tol=1e-6) override;
    Vec<Expr> roots(const CallExpr& expr, const Str& var, f64 tol=1e-6) override;
    Vec<Expr> roots(const EquationExpr& expr, const Str& var, f64 tol=1e-6) override;
    Vec<Expr> roots(const SystemExpr& expr, const Str& var, f64 tol=1e-6) override;
};


// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola                                       -*-
// -*----------------------------------------------------------------*-