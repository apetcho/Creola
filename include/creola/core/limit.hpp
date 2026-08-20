#pragma once

#include "creola/core/visitors.hpp"


// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola                                     -*-
// -*----------------------------------------------------------------*-
namespace creola::core{
//

class LimitFinder final: protected LimitVisitor{
public:
    explicit LimitFinder(const Expr& expr, const Str& var, f64 loc);
    explicit LimitFinder(Expr&& expr, Str&& var, f64 loc);

    Expr limit(void);

private:
    Expr m_expr;
    Str m_var;
    f64 loc;

    Expr limit(const SymbolExpr& expr, const Str& var, f64 loc) override;
    Expr limit(const NumberExpr& expr, const Str& var, f64 loc) override;
    Expr limit(const NegExpr& expr, const Str& var, f64 loc) override;
    Expr limit(const AddExpr& expr, const Str& var, f64 loc) override;
    Expr limit(const MulExpr& expr, const Str& var, f64 loc) override;
    Expr limit(const PowExpr& expr, const Str& var, f64 loc) override;
    Expr limit(const CallExpr& expr, const Str& var, f64 loc) override;
    Expr limit(const EquationExpr& expr, const Str& var, f64 loc) override;
    Expr limit(const SystemExpr& expr, const Str& var, f64 loc) override;
};


// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola                                       -*-
// -*----------------------------------------------------------------*-