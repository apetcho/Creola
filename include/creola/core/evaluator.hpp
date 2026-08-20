#pragma once

#include "creola/core/visitors.hpp"

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola::core                               -*-
// -*----------------------------------------------------------------*-
namespace creola::core{
// -
class Evaluator final: protected EvalVisitor {
public:
    explicit Evaluator(Env& ctx);
    ~Evaluator() = default;

    Expr eval(const Expr& expr);
    Expr eval(Expr&& expr);

private:
    Env& m_ctx;

    Expr eval(const SymbolExpr& expr, Env& ctx) const override;
    Expr eval(const NumberExpr& expr, Env& ctx) const override;
    Expr eval(const NegExpr& expr, Env& ctx) const override;
    Expr eval(const AddExpr& expr, Env& ctx) const override;
    Expr eval(const MulExpr& expr, Env& ctx) const override;
    Expr eval(const PowExpr& expr, Env& ctx) const override;
    Expr eval(const CallExpr& expr, Env& ctx) const override;
    Expr eval(const EquationExpr& expr, Env& ctx) const override;
    Expr eval(const SystemExpr& expr, Env& ctx) const override;
};

// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola::core                                 -*-
// -*----------------------------------------------------------------*-