#pragma once

#include "creola/core/visitors.hpp"

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola::core                               -*-
// -*----------------------------------------------------------------*-
namespace creola::core{
// -
class Factorizer final: protected FactorVisitor{
public:
    explicit Factorizer()
    : m_expr{nullptr}, m_var{}
    {}

    void setup(const Expr& expr, const Str& var){
        this->m_expr = expr;
        this->m_var = var;
    }

    Expr factor(void);

private:
    Expr m_expr;
    Str m_var;

    Expr factor(const SymbolExpr& expr, const Str& var) override;
    Expr factor(const NumberExpr& expr, const Str& var) override;
    Expr factor(const NegExpr& expr, const Str& var) override;
    Expr factor(const AddExpr& expr, const Str& var) override;
    Expr factor(const MulExpr& expr, const Str& var) override;
    Expr factor(const PowExpr& expr, const Str& var) override;
    Expr factor(const LambdaExpr& expr, const Str& var) override;
    Expr factor(const CallExpr& expr, const Str& var) override;
    Expr factor(const EquationExpr& expr, const Str& var) override;
    Expr factor(const SystemExpr& expr, const Str& var) override;
    Expr factor(const SeqExpr& expr, const Str& var) override;
};


// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola::core                                 -*-
// -*----------------------------------------------------------------*-