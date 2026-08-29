#pragma once

#include "creola/core/visitors.hpp"

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola                                     -*-
// -*----------------------------------------------------------------*-
namespace creola::core{
//

class RootsFinder final: protected RootsVisitor{
public:
    explicit RootsFinder()
    : m_expr{nullptr}, m_var{}, m_tol{1e-6}
    {}

    Expr roots(const Expr& expr, const Str& var, f64 tol=1e-6);

private:
    Expr m_expr;
    Str m_var;
    f64 m_tol;

    void setup(const Expr& expr, const Str& var, f64 tol=1e-6){
        this->m_expr = expr;
        this->m_var = var;
        this->m_tol = tol;
    }

    Expr roots(const SymbolExpr& expr, const Str& var, f64 tol=1e-6) override;
    Expr roots(const NumberExpr& expr, const Str& var, f64 tol=1e-6) override;
    Expr roots(const NegExpr& expr, const Str& var, f64 tol=1e-6) override;
    Expr roots(const AddExpr& expr, const Str& var, f64 tol=1e-6) override;
    Expr roots(const MulExpr& expr, const Str& var, f64 tol=1e-6) override;
    Expr roots(const PowExpr& expr, const Str& var, f64 tol=1e-6) override;
    Expr roots(const LambdaExpr& expr, const Str& var, f64 tol=1e-6) override;
    Expr roots(const CallExpr& expr, const Str& var, f64 tol=1e-6) override;
    Expr roots(const EquationExpr& expr, const Str& var, f64 tol=1e-6) override;
    Expr roots(const SystemExpr& expr, const Str& var, f64 tol=1e-6) override;
    Expr roots(const SeqExpr& expr, const Str& var, f64 tol=1e-6) override;
};


// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola                                       -*-
// -*----------------------------------------------------------------*-