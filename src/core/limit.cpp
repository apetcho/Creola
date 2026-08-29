#include "creola/core/limit.hpp"

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola::core                               -*-
// -*----------------------------------------------------------------*-
namespace creola::core{
//
// -*-
Expr LimitFinder::limit(const Expr& expr, const Str& var, f64 loc){
    //! @todo
    return nullptr;
}

Expr LimitFinder::limit(const SymbolExpr& expr, const Str& var, f64 loc){
    //! @todo
    return nullptr;
}

Expr LimitFinder::limit(const NumberExpr& expr, const Str& var, f64 loc){
    //! @todo
    return nullptr;
}

Expr LimitFinder::limit(const NegExpr& expr, const Str& var, f64 loc){
    //! @todo
    return nullptr;
}

Expr LimitFinder::limit(const AddExpr& expr, const Str& var, f64 loc){
    //! @todo
    return nullptr;
}

Expr LimitFinder::limit(const MulExpr& expr, const Str& var, f64 loc){
    //! @todo
    return nullptr;
}

Expr LimitFinder::limit(const PowExpr& expr, const Str& var, f64 loc){
    //! @todo
    return nullptr;
}

Expr LimitFinder::limit(const LambdaExpr& expr, const Str& var, f64 loc){
    //! @todo
    return nullptr;
}

Expr LimitFinder::limit(const CallExpr& expr, const Str& var, f64 loc){
    //! @todo
    return nullptr;
}

Expr LimitFinder::limit(const EquationExpr& expr, const Str& var, f64 loc){
    //! @todo
    return nullptr;
}

Expr LimitFinder::limit(const SystemExpr& expr, const Str& var, f64 loc){
    //! @todo
    return nullptr;
}

Expr LimitFinder::limit(const SeqExpr& expr, const Str& var, f64 loc){
    //! @todo
    return nullptr;
}

/*
class LimitFinder final: protected LimitVisitor{
public:

private:
    Expr m_expr;
    Str m_var;
    f64 m_loc;

};
*/

// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola::core                                 -*-
// -*----------------------------------------------------------------*-