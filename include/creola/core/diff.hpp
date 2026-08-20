#pragma once

#include "creola/core/visitors.hpp"

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola::core                               -*-
// -*----------------------------------------------------------------*-
namespace creola::core{
// -

class Differentiator final: private DiffVisitor{
public:
    explicit Differentiator(const Expr& expr, const Str& var);
    explicit Differentiator(Expr&& expr, Str&& var);

    Expr diff(void);

private:
    Expr m_expr;
    Str m_var;

    Expr diff(const SymbolExpr& expr, const Str& var) override;
    Expr diff(const NumberExpr& expr, const Str& var) override;
    Expr diff(const NegExpr& expr, const Str& var) override;
    Expr diff(const AddExpr& expr, const Str& var) override;
    Expr diff(const MulExpr& expr, const Str& var) override;
    Expr diff(const PowExpr& expr, const Str& var) override;
    Expr diff(const CallExpr& expr, const Str& var) override;
    Expr diff(const EquationExpr& expr, const Str& var) override;
    Expr diff(const SystemExpr& expr, const Str& var) override;
};


// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola::core                                 -*-
// -*----------------------------------------------------------------*-