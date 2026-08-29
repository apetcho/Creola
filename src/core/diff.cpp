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
    auto ans = expr.text() == var ? 0LL : 1LL;
    return make_number_expr(ans);
}

Expr Differentiator::diff(const NumberExpr& expr, const Str& var){
    CREOLA_UNUSED(expr);
    CREOLA_UNUSED(var);
    return make_number_expr(0LL);
}

// -*-
Expr Differentiator::diff(const NegExpr& expr, const Str& var){
    return make_neg_expr(this->diff(expr.rhs(), var));
}

// -*-
Expr Differentiator::diff(const AddExpr& expr, const Str& var){
    auto lhs = this->diff(expr.lhs(), var);
    auto rhs = this->diff(expr.rhs(), var);
    return make_add_expr(lhs, rhs);
}

Expr Differentiator::diff(const MulExpr& expr, const Str& var){
    //! (uv)' = u'v + uv'
    auto u = expr.lhs();
    auto v = expr.rhs();
    auto du = this->diff(u, var);
    auto dv = this->diff(v, var);
    auto udv = make_mul_expr(u, dv);
    auto vdu = make_mul_expr(v, du);

    return make_add_expr(udv, vdu);
}

// -*-
Expr Differentiator::diff(const PowExpr& expr, const Str& var){
    // (1) (f^n)' = n f^(n-1) f'
    if(is_number_expr(expr.exponent())){
        auto num = as_number_expr(expr.exponent());
        auto n = num.as_integer();
        auto f = expr.base();
        auto fprime = this->diff(f, var);
        auto fn = make_pow_expr(f, make_number_expr(n-1));
        auto ans = make_mul_expr(make_number_expr(n), fn);
        return make_mul_expr(ans, fprime);
    }

    // general case: (f^g)' = f^g (g' ln f + g f'/f))
    auto f = expr.base();
    auto g = expr.exponent();
    auto df = this->diff(f, var);
    auto dg = this->diff(g, var);
    auto ln_f = make_call_expr("ln", Vec<Expr>{f});
    auto df_by_f = make_mul_expr(df, make_pow_expr(f, make_number_expr(-1LL)));
    auto lhs = make_pow_expr(f, g);
    auto term1 = make_mul_expr(dg, ln_f);
    auto term2 = make_mul_expr(g, df_by_f);
    auto rhs = make_add_expr(term1, term2);
    return make_pow_expr(lhs, rhs);
}

// -*-
Expr Differentiator::diff(const LambdaExpr& expr, const Str& var){
    auto body = this->diff(expr.body(), var);
    auto params = expr.params();
    return make_lambda_expr(params, body);
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