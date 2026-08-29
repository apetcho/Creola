#include "creola/core/integrate.hpp"

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola::core                               -*-
// -*----------------------------------------------------------------*-
namespace creola::core{
//

Expr Integrator::integral(const Expr& expr, const Str& var){
    //! @todo
    return nullptr;
}

Expr Integrator::integral(const SymbolExpr& expr, const Str& var){
    //! @todo
    return nullptr;
}

Expr Integrator::integral(const NumberExpr& expr, const Str& var){
    //! @todo
    return nullptr;
}

Expr Integrator::integral(const NegExpr& expr, const Str& var){
    //! @todo
    return nullptr;
}

Expr Integrator::integral(const AddExpr& expr, const Str& var){
    //! @todo
    return nullptr;
}

Expr Integrator::integral(const MulExpr& expr, const Str& var){
    //! @todo
    return nullptr;
}

Expr Integrator::integral(const PowExpr& expr, const Str& var){
    //! @todo
    return nullptr;
}

Expr Integrator::integral(const LambdaExpr& expr, const Str& var){
    //! @todo
    return nullptr;
}

Expr Integrator::integral(const CallExpr& expr, const Str& var){
    //! @todo
    return nullptr;
}

Expr Integrator::integral(const EquationExpr& expr, const Str& var){
    //! @todo
    return nullptr;
}

Expr Integrator::integral(const SystemExpr& expr, const Str& var){
    //! @todo
    return nullptr;
}

Expr Integrator::integral(const SeqExpr& expr, const Str& var){
    //! @todo
    return nullptr;
}

/*
class Integrator final: protected IntegralVisitor{
public:

private:
    Expr m_expr;
    Str m_var;

};
*/

// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola::core                                 -*-
// -*----------------------------------------------------------------*-