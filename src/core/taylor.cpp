#include "creola/core/taylor.hpp"

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola::core                               -*-
// -*----------------------------------------------------------------*-
namespace creola::core{
//

// -*-
Expr TaylorExpander::taylor(const Expr& expr, const Str& var, f64 center, u32 order){
    //! @todo
    return nullptr;
}

Expr TaylorExpander::taylor(const SymbolExpr& expr, const Str& var, f64 center, u32 order){
    //! @todo
    return nullptr;
}

Expr TaylorExpander::taylor(const NumberExpr& expr, const Str& var, f64 center, u32 order){
    //! @todo
    return nullptr;
}

Expr TaylorExpander::taylor(const NegExpr& expr, const Str& var, f64 center, u32 order){
    //! @todo
    return nullptr;
}

Expr TaylorExpander::taylor(const AddExpr& expr, const Str& var, f64 center, u32 order){
    //! @todo
    return nullptr;
}

Expr TaylorExpander::taylor(const MulExpr& expr, const Str& var, f64 center, u32 order){
    //! @todo
    return nullptr;
}

Expr TaylorExpander::taylor(const PowExpr& expr, const Str& var, f64 center, u32 order){
    //! @todo
    return nullptr;
}

Expr TaylorExpander::taylor(const LambdaExpr& expr, const Str& var, f64 center, u32 order){
    //! @todo
    return nullptr;
}

Expr TaylorExpander::taylor(const CallExpr& expr, const Str& var, f64 center, u32 order){
    //! @todo
    return nullptr;
}

Expr TaylorExpander::taylor(const EquationExpr& expr, const Str& var, f64 center, u32 order){
    //! @todo
    return nullptr;
}

Expr TaylorExpander::taylor(const SystemExpr& expr, const Str& var, f64 center, u32 order){
    //! @todo
    return nullptr;
}

Expr TaylorExpander::taylor(const SeqExpr& expr, const Str& var, f64 center, u32 order){
    //! @todo
    return nullptr;
}

/*
class TaylorExpander final: protected TaylorVisitor{
public:

private:
    Expr m_expr;
    Str m_var;
    f64 m_center;
    u32 m_order;
};
*/

// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola::core                                 -*-
// -*----------------------------------------------------------------*-