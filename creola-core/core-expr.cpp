#include "core-expr.hpp"

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola                                     -*-
// -*----------------------------------------------------------------*-
namespace creola{
//

Expr Number::simplify(void) const{
    return std::make_shared<Number>(*this);
}


/*

// -*-
struct Number : public ExprBase{
    f64 value;
    explicit Number(f64 val)
    : ExprBase{ExprKind::Number}, value{val}{}

Expr Number::simplify(void) const override;
Expr Number::diff(const std::string& var) const override;
Expr Number::integrate(const std::string& var) const override;
Expr Number::expand(void) const override;
Expr Number::factor(const std::string& var) const override;
Expr Number::groebner(const Vec<Expr>& exprs) const override;
Expr Number::taylor(const std::string& var, f64 val, int n) const override;
Vec<f64> Number::roots(const std::string& var, f64 vmin=-10, f64 vmax=10, int samples=100) const override;
f64 Number::limit(const std::string& var, f64 val, f64 eps=1e-6) const override;

f64 eval(const std::string& var, f64 val) const override;

Box Number::to_box(void) const override;
Box Number::to_box_prec(int parent_prec) const override;
void Number::print_unicode(std::ostream& os, int prec=0) const override;
};

*/

// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola                                       -*-
// -*----------------------------------------------------------------*-