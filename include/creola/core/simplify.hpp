#pragma once

#include "creola/core/visitors.hpp"

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola::core                               -*-
// -*----------------------------------------------------------------*-
namespace creola::core{
// -

class Simplifier final: protected SimplifyVisitor{
public:
    explicit Simplifier(): m_expr{nullptr} {}
    ~Simplifier() = default;

    Expr simplify(const Expr& expr);

private:
    Expr m_expr;

    void setup(const Expr& expr){
        this->m_expr = expr;
    }

    Expr simplify(const SymbolExpr& expr) override;
    Expr simplify(const NumberExpr& expr) override;
    Expr simplify(const NegExpr& expr) override;
    Expr simplify(const AddExpr& expr) override;
    Expr simplify(const MulExpr& expr) override;
    Expr simplify(const PowExpr& expr) override;
    Expr simplify(const LambdaExpr& expr) override;
    Expr simplify(const CallExpr& expr) override;
    Expr simplify(const EquationExpr& expr) override;
    Expr simplify(const SystemExpr& expr) override;
    Expr simplify(const SeqExpr& expr) override;
};


// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola::core                                 -*-
// -*----------------------------------------------------------------*-