#pragma once

#include "creola/core/visitors.hpp"

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola::core                               -*-
// -*----------------------------------------------------------------*-
namespace creola::core{
// -

class TaylorExpander final: protected TaylorVisitor{
public:
    explicit TaylorExpander()
    : m_expr{nullptr}
    , m_var{}
    , m_center{}
    , m_order{}
    {}

    void setup(Expr&& expr, Str&& var, f64 center, u32 order){
        this->m_expr = expr;
        this->m_var = var;
        this->m_center = center;
        this->m_order = order;
    }

    Expr taylor(void);

private:
    Expr m_expr;
    Str m_var;
    f64 m_center;
    u32 m_order;

    Expr taylor(const SymbolExpr& expr, const Str& var, f64 center, u32 order) override;
    Expr taylor(const NumberExpr& expr, const Str& var, f64 center, u32 order) override;
    Expr taylor(const NegExpr& expr, const Str& var, f64 center, u32 order) override;
    Expr taylor(const AddExpr& expr, const Str& var, f64 center, u32 order) override;
    Expr taylor(const MulExpr& expr, const Str& var, f64 center, u32 order) override;
    Expr taylor(const PowExpr& expr, const Str& var, f64 center, u32 order) override;
    Expr taylor(const LambdaExpr& expr, const Str& var, f64 center, u32 order) override;
    Expr taylor(const CallExpr& expr, const Str& var, f64 center, u32 order) override;
    Expr taylor(const EquationExpr& expr, const Str& var, f64 center, u32 order) override;
    Expr taylor(const SystemExpr& expr, const Str& var, f64 center, u32 order) override;
    Expr taylor(const SeqExpr& expr, const Str& var, f64 center, u32 order) override;
};


// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola::core                                 -*-
// -*----------------------------------------------------------------*-