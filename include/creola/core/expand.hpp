#pragma once

#include "creola/core/visitors.hpp"

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola::core                               -*-
// -*----------------------------------------------------------------*-
namespace creola::core{
// -

class Expander final: protected ExpandVisitor{
public:
    explicit Expander(const Expr& expr);
    explicit Expander(Expr&& expr);

    Expr expand(void);

private:
    Expr m_expr;

    // -*-
    Expr expand(const SymbolExpr& expr) override;
    Expr expand(const NumberExpr& expr) override;
    Expr expand(const NegExpr& expr) override;
    Expr expand(const AddExpr& expr) override;
    Expr expand(const MulExpr& expr) override;
    Expr expand(const PowExpr& expr) override;
    Expr expand(const CallExpr& expr) override;
    Expr expand(const EquationExpr& expr) override;
    Expr expand(const SystemExpr& expr) override;
};

// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola::core                                 -*-
// -*----------------------------------------------------------------*-