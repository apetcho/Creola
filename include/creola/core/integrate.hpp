#pragma once

#include "creola/core/visitors.hpp"

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola::core                               -*-
// -*----------------------------------------------------------------*-
namespace creola::core{
// -

class Integrator final: protected IntegralVisitor{
public:
    explicit Integrator(const Expr& expr, const Str& var);
    explicit Integrator(Expr&& expr, Str&& var);

    Expr integral(void);

private:
    Expr m_expr;
    Str m_var;

    Expr integral(const SymbolExpr& expr, const Str& var) override;
    Expr integral(const NumberExpr& expr, const Str& var) override;
    Expr integral(const NegExpr& expr, const Str& var) override;
    Expr integral(const AddExpr& expr, const Str& var) override;
    Expr integral(const MulExpr& expr, const Str& var) override;
    Expr integral(const PowExpr& expr, const Str& var) override;
    Expr integral(const CallExpr& expr, const Str& var) override;
    Expr integral(const EquationExpr& expr, const Str& var) override;
    Expr integral(const SystemExpr& expr, const Str& var) override;
};


// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola::core                                 -*-
// -*----------------------------------------------------------------*-