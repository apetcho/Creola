#pragma once

#include "creola/core/visitors.hpp"

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola::core                               -*-
// -*----------------------------------------------------------------*-
namespace creola::core{
// -

class Simplifier final: protected SimplifyVisitor{
public:
    explicit Simplifier(const Expr& expr);
    explicit Simplifier(Expr&& expr);
    ~Simplifier() = default;

    Expr simplify(void);

private:
    Expr m_expr;

    Expr simplify(const SymbolExpr& expr) override;
    Expr simplify(const NumberExpr& expr) override;
    Expr simplify(const NegExpr& expr) override;
    Expr simplify(const AddExpr& expr) override;
    Expr simplify(const MulExpr& expr) override;
    Expr simplify(const PowExpr& expr) override;
    Expr simplify(const CallExpr& expr) override;
    Expr simplify(const EquationExpr& expr) override;
    Expr simplify(const SystemExpr& expr) override;
};


// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola::core                                 -*-
// -*----------------------------------------------------------------*-