#pragma once

#include "creola/core/visitors.hpp"


// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola                                     -*-
// -*----------------------------------------------------------------*-
namespace creola::core{
//

class LimitFinder final: protected LimitVisitor{
public:
    explicit LimitFinder()
    : m_expr{nullptr}, m_var{}, m_loc{}
    {}

    Expr limit(const Expr& expr, const Str& var, f64 loc);

private:
    Expr m_expr;
    Str m_var;
    f64 m_loc;

    void setup(const Expr& expr, const Str& var, f64 loc){
        this->m_expr = expr;
        this->m_var = var;
        this->m_loc = loc;
    }

    Expr limit(const SymbolExpr& expr, const Str& var, f64 loc) override;
    Expr limit(const NumberExpr& expr, const Str& var, f64 loc) override;
    Expr limit(const NegExpr& expr, const Str& var, f64 loc) override;
    Expr limit(const AddExpr& expr, const Str& var, f64 loc) override;
    Expr limit(const MulExpr& expr, const Str& var, f64 loc) override;
    Expr limit(const PowExpr& expr, const Str& var, f64 loc) override;
    Expr limit(const LambdaExpr& expr, const Str& var, f64 loc) override;
    Expr limit(const CallExpr& expr, const Str& var, f64 loc) override;
    Expr limit(const EquationExpr& expr, const Str& var, f64 loc) override;
    Expr limit(const SystemExpr& expr, const Str& var, f64 loc) override;
    Expr limit(const SeqExpr& expr, const Str& var, f64 loc) override;
};


// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola                                       -*-
// -*----------------------------------------------------------------*-