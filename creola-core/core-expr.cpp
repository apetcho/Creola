#include "core-expr.hpp"

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola::core                               -*-
// -*----------------------------------------------------------------*-
namespace creola::core{
//
// -------------------------
// --- Number Expression ---
// -------------------------
Expr Number::simplify(void) const{
    return std::make_shared<Number>(*this);
}

// -*-
Expr Number::diff(const std::string& var) const{
    CREOLA_UNUSED(var);
    return std::make_shared<Number>(0.0);
}

// -*-
Expr Number::integrate(const std::string& var) const{
    //! @todo
    return nullptr;
}

// -*-
Expr Number::expand(void) const{
    return std::make_shared<Number>(*this);
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

// -*-
void Number::print_unicode(std::ostream& os, int prec) const{
    CREOLA_UNUSED(prec);
    os << this->value;
    return;
}

// -------------------------
// --- Symbol Expression ---
// -------------------------
Expr Symbol::simplify(void) const{
    return std::make_shared<Symbol>(*this);
}

//
Expr Symbol::diff(const std::string& var) const{
    return std::make_shared<Number>(this->name==var ? 1.0 : 0.0);
}

Expr Symbol::integrate(const std::string& var) const{
    //! @todo
    return nullptr;
}

// -*-
Expr Symbol::expand(void) const{
    return std::make_shared<Symbol>(*this);
}


Expr Symbol::factor(const std::string& var) const{
    //! @todo
    return nullptr;
}
Expr Symbol::groebner(const Vec<Expr>& exprs) const{
    //! @todo
    return nullptr;
}
Expr Symbol::taylor(const std::string& var, f64 val, int n) const{
    //! @todo
    return nullptr;
}
Vec<f64> Symbol::roots(const std::string& var, f64 vmin, f64 vmax, int samples) const{
    //! @todo
    return {};
}
f64 Symbol::limit(const std::string& var, f64 val, f64 eps) const{
    //! @todo
    return 0.0;
}

f64 Symbol::eval(const std::string& var, f64 val) const{
    //! @todo
    return 0.0;
}

Box Symbol::to_box(void) const{
    //! @todo
    return Box();
}

Box Symbol::to_box_prec(int parent_prec) const{
    //! @todo
    return Box();
}
void Symbol::print_unicode(std::ostream& os, int prec) const{
    CREOLA_UNUSED(prec);
    os << this->name;
    return;
}

// ---------------------------
// --- Neg[ate] Expression ---
// ---------------------------
Expr Neg::simplify(void) const{
    auto expr = this->arg->simplify();
    if (auto num = std::dynamic_pointer_cast<Number>(expr)){
        return number(-num->value);
    }
    return std::make_shared<Neg>(expr);
}

// -*-
Expr Neg::diff(const std::string& var) const{
    return std::make_shared<Neg>(this->arg->diff(var));
}

Expr Neg::integrate(const std::string& var) const{
    //! @todo
    return nullptr;
}
Expr Neg::expand(void) const{
    //! @todo
    return nullptr;
}
Expr Neg::factor(const std::string& var) const{
    //! @todo
    return nullptr;
}
Expr Neg::groebner(const Vec<Expr>& exprs) const{
    //! @todo
    return nullptr;
}
Expr Neg::taylor(const std::string& var, f64 val, int n) const{
    //! @todo
    return nullptr;
}
Vec<f64> Neg::roots(const std::string& var, f64 vmin, f64 vmax, int samples) const{
    //! @todo
    return {};
}
f64 Neg::limit(const std::string& var, f64 val, f64 eps=1e-6) const{
    //! @todo
    return 0.0;
}

f64 Neg::eval(const std::string& var, f64 val) const{
    //! @todo
    return 0.0;
}

Box Neg::to_box(void) const{
    //! @todo
    return Box();
}
Box Neg::to_box_prec(int parent_prec) const{
    //! @todo
    return Box();
}
void Neg::print_unicode(std::ostream& os, int prec=0) const{
    //! @todo
    return;
}

// ----------------------
// --- Add Expression ---
// ----------------------

/*

Expr Add::simplify(void) const{
    //! @todo
    return;
}
Expr Add::diff(const std::string& var) const{
    //! @todo
    return;
}
Expr Add::integrate(const std::string& var) const{
    //! @todo
    return;
}
Expr Add::expand(void) const{
    //! @todo
    return;
}
Expr Add::factor(const std::string& var) const{
    //! @todo
    return;
}
Expr Add::groebner(const Vec<Expr>& exprs) const{
    //! @todo
    return;
}
Expr Add::taylor(const std::string& var, f64 val, int n) const{
    //! @todo
    return;
}
Vec<f64> Add::roots(const std::string& var, f64 vmin=, f64 vmax, int samples) const{
    //! @todo
    return;
}
f64 Add::limit(const std::string& var, f64 val, f64 eps) const{
    //! @todo
    return;
}

f64 Add::eval(const std::string& var, f64 val) const{
    //! @todo
    return;
}

Box Add::to_box(void) const{
    //! @todo
    return;
}
Box Add::to_box_prec(int parent_prec) const{
    //! @todo
    return;
}
void Add::print_unicode(std::ostream& os, int prec=0) const{
    //! @todo
    return;
}

*/


// ----------------------
// --- Mul Expression ---
// ----------------------

/*

Expr Mul::simplify(void) const{
    //! @todo
    return;
}
Expr Mul::diff(const std::string& var) const{
    //! @todo
    return;
}
Expr Mul::integrate(const std::string& var) const{
    //! @todo
    return;
}
Expr Mul::expand(void) const{
    //! @todo
    return;
}
Expr Mul::factor(const std::string& var) const{
    //! @todo
    return;
}
Expr Mul::groebner(const Vec<Expr>& exprs) const{
    //! @todo
    return;
}
Expr Mul::taylor(const std::string& var, f64 val, int n) const{
    //! @todo
    return;
}
Vec<f64> Mul::roots(const std::string& var, f64 vmin=, f64 vmax, int samples) const{
    //! @todo
    return;
}
f64 Mul::limit(const std::string& var, f64 val, f64 eps) const{
    //! @todo
    return;
}

f64 Mul::eval(const std::string& var, f64 val) const{
    //! @todo
    return;
}

Box Mul::to_box(void) const{
    //! @todo
    return;
}
Box Mul::to_box_prec(int parent_prec) const{
    //! @todo
    return;
}
void Mul::print_unicode(std::ostream& os, int prec=0) const{
    //! @todo
    return;
}

*/

// ----------------------
// --- Pow Expression ---
// ----------------------

/*

Expr Pow::simplify(void) const{
    //! @todo
    return;
}
Expr Pow::diff(const std::string& var) const{
    //! @todo
    return;
}
Expr Pow::integrate(const std::string& var) const{
    //! @todo
    return;
}
Expr Pow::expand(void) const{
    //! @todo
    return;
}
Expr Pow::factor(const std::string& var) const{
    //! @todo
    return;
}
Expr Pow::groebner(const Vec<Expr>& exprs) const{
    //! @todo
    return;
}
Expr Pow::taylor(const std::string& var, f64 val, int n) const{
    //! @todo
    return;
}
Vec<f64> Pow::roots(const std::string& var, f64 vmin=, f64 vmax, int samples) const{
    //! @todo
    return;
}
f64 Pow::limit(const std::string& var, f64 val, f64 eps) const{
    //! @todo
    return;
}

f64 Pow::eval(const std::string& var, f64 val) const{
    //! @todo
    return;
}

Box Pow::to_box(void) const{
    //! @todo
    return;
}
Box Pow::to_box_prec(int parent_prec) const{
    //! @todo
    return;
}
void Pow::print_unicode(std::ostream& os, int prec=0) const{
    //! @todo
    return;
}

*/

// ---------------------------
// --- FuncCall Expression ---
// ---------------------------

/*

Expr FuncCall::simplify(void) const{
    //! @todo
    return;
}
Expr FuncCall::diff(const std::string& var) const{
    //! @todo
    return;
}
Expr FuncCall::integrate(const std::string& var) const{
    //! @todo
    return;
}
Expr FuncCall::expand(void) const{
    //! @todo
    return;
}
Expr FuncCall::factor(const std::string& var) const{
    //! @todo
    return;
}
Expr FuncCall::groebner(const Vec<Expr>& exprs) const{
    //! @todo
    return;
}
Expr FuncCall::taylor(const std::string& var, f64 val, int n) const{
    //! @todo
    return;
}
Vec<f64> FuncCall::roots(const std::string& var, f64 vmin=, f64 vmax, int samples) const{
    //! @todo
    return;
}
f64 FuncCall::limit(const std::string& var, f64 val, f64 eps) const{
    //! @todo
    return;
}

f64 FuncCall::eval(const std::string& var, f64 val) const{
    //! @todo
    return;
}

Box FuncCall::to_box(void) const{
    //! @todo
    return;
}
Box FuncCall::to_box_prec(int parent_prec) const{
    //! @todo
    return;
}
void FuncCall::print_unicode(std::ostream& os, int prec=0) const{
    //! @todo
    return;
}

*/

// ---- Helper functions ----
/*


// -*-

Expr simplify(const Expr& expr){}
Expr diff(const Expr& expr);
Expr expand(const Expr& expr);
Expr factor(const Expr& expr, const std::string& var);
Expr integrate(const Expr& expr, const std::string& var);
f64 eval(const Expr& expr, const std::string& var, f64 val);
Vec<f64> roots(const Expr& expr, const std::string& var, f64 vmin=-10, f64 vmax=10, int samples=200);
Expr taylor(const Expr& expr, const std::string& var, f64 val, int n);
f64 limit(const Expr& expr, const std::string& var, f64 val, int max_iter=5);

*/

// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola::core                                 -*-
// -*----------------------------------------------------------------*-