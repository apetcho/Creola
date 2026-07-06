#include "core-expr.hpp"

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola                                     -*-
// -*----------------------------------------------------------------*-
namespace creola{
//

Expr Number::simplify(void) const{
    return std::make_shared<Number>(*this);
}

// -*-
Expr Number::diff(const std::string& var) const{
    CREOLA_UNUSED(var);
    return std::make_shared<Number>(0.0);
}

Expr Number::integrate(const std::string& var) const{
    //! @todo
    return nullptr;
}
Expr Number::expand(void) const{
    //! @todo
    return nullptr;
}
Expr Number::factor(const std::string& var) const{
    //! @todo
    return nullptr;
}
Expr Number::groebner(const Vec<Expr>& exprs) const{
    //! @todo
    return nullptr;
}
Expr Number::taylor(const std::string& var, f64 val, int n) const{
    //! @todo
    return nullptr;
}
Vec<f64> Number::roots(const std::string& var, f64 vmin, f64 vmax, int samples) const{
    //! @todo
    return {};
}
f64 Number::limit(const std::string& var, f64 val, f64 eps=1e-6) const{
    //! @todo
    return 0.0;
}

f64 Number::eval(const std::string& var, f64 val) const{
    //! @todo
    return 0.0;
}

Box Number::to_box(void) const{
    //! @todo
    return Box();
}
Box Number::to_box_prec(int parent_prec) const{
    //! @todo
    return Box();
}
void Number::print_unicode(std::ostream& os, int prec) const{
    //! @todo
    return;
}

/*

// -*-
struct Number : public ExprBase{
    f64 value;
    explicit Number(f64 val)
    : ExprBase{ExprKind::Number}, value{val}{}


};

*/

// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola                                       -*-
// -*----------------------------------------------------------------*-