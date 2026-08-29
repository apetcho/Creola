#include "creola/core/diff.hpp"
#include "creola/core/ast.hpp"

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola::core                               -*-
// -*----------------------------------------------------------------*-
namespace creola::core{
//

Expr Differentiator::diff(const Expr& expr, const Str& var){
    if(auto self = std::dynamic_pointer_cast<NumberExpr>(expr)){
        return this->diff(*self, var);
    }
    if(auto self = std::dynamic_pointer_cast<SymbolExpr>(expr)){
        return this->diff(*self, var);
    }
    if(auto self = std::dynamic_pointer_cast<NegExpr>(expr)){
        return this->diff(*self, var);
    }
    if(auto self = std::dynamic_pointer_cast<AddExpr>(expr)){
        return this->diff(*self, var);
    }

    if(auto self = std::dynamic_pointer_cast<MulExpr>(expr)){
        return this->diff(*self, var);
    }

    if(auto self = std::dynamic_pointer_cast<PowExpr>(expr)){
        return this->diff(*self, var);
    }

    if(auto self = std::dynamic_pointer_cast<LambdaExpr>(expr)){
        return this->diff(*self, var);
    }

    if(auto self = std::dynamic_pointer_cast<CallExpr>(expr)){
        return this->diff(*self, var);
    }

    if(auto self = std::dynamic_pointer_cast<EquationExpr>(expr)){
        return this->diff(*self, var);
    }

    if(auto self = std::dynamic_pointer_cast<SymbolExpr>(expr)){
        return this->diff(*self, var);
    }

    auto self = std::dynamic_pointer_cast<SeqExpr>(expr);
    return this->diff(*self, var);
}

Expr Differentiator::diff(const SymbolExpr& expr, const Str& var){
    //! @todo
    return nullptr;
}

Expr Differentiator::diff(const NumberExpr& expr, const Str& var){
    CREOLA_UNUSED(expr);
    CREOLA_UNUSED(var);
    return make_number_expr(0LL);
}

Expr Differentiator::diff(const NegExpr& expr, const Str& var){
    //! @todo
    return nullptr;
}

Expr Differentiator::diff(const AddExpr& expr, const Str& var){
    //! @todo
    return nullptr;
}

Expr Differentiator::diff(const MulExpr& expr, const Str& var){
    //! @todo
    return nullptr;
}

Expr Differentiator::diff(const PowExpr& expr, const Str& var){
    //! @todo
    return nullptr;
}

Expr Differentiator::diff(const LambdaExpr& expr, const Str& var){
    //! @todo
    return nullptr;
}

Expr Differentiator::diff(const CallExpr& expr, const Str& var){
    //! @todo
    return nullptr;
}

Expr Differentiator::diff(const EquationExpr& expr, const Str& var){
    //! @todo
    return nullptr;
}

Expr Differentiator::diff(const SystemExpr& expr, const Str& var){
    //! @todo
    return nullptr;
}

Expr Differentiator::diff(const SeqExpr& expr, const Str& var){
    //! @todo
    return nullptr;
}

/*
class Differentiator final: private DiffVisitor{
public:
    

private:
    Expr m_expr;
    Str m_var;

};
*/

// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola::core                                 -*-
// -*----------------------------------------------------------------*-