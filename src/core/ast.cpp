#include "creola/core/ast.hpp"

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola::core                               -*-
// -*----------------------------------------------------------------*-
namespace creola::core{
//

Lambda::Lambda(const Vec<Str>& params, const Expr& body)
: params{params}, body{body}
{}

Lambda::Lambda(Vec<Str>&& params, Expr&& body)
: params{std::move(params)}
, body{std::move(body)}
{}

// ------------------
// -*- SymbolExpr -*-
// ------------------
SymbolExpr::SymbolExpr(const Str& sym)
: m_text{sym}
{}

// -*-
Expr SymbolExpr::eval(const EvalVisitor& visitor, Env& ctx) const{
    return visitor.eval(*this, ctx);
}

// ------------------
// -*- NumberExpr -*-
// ------------------
NumberExpr::NumberExpr()
: m_data{i64{}}
{}

// -
NumberExpr::NumberExpr(i64 val)
: m_data{val}
{}

// -
NumberExpr::NumberExpr(f64 val)
: m_data{val}
{}

// -
Expr NumberExpr::eval(const EvalVisitor& visitor, Env& ctx) const{
    return visitor.eval(*this, ctx);
}

// -
i64 NumberExpr::as_integer(void) const{
    if(std::holds_alternative<i64>(this->m_data)){
        return std::get<i64>(this->m_data);
    }
    auto num = std::get<f64>(this->m_data);

    return static_cast<i64>(num);
}

// -
f64 NumberExpr::as_float(void) const{
    if(std::holds_alternative<f64>(this->m_data)){
        return std::get<f64>(this->m_data);
    }

    auto num = std::get<i64>(this->m_data);
    return static_cast<f64>(num);
}

// -
bool NumberExpr::is_integer(void) const{
    return std::holds_alternative<i64>(this->m_data);
}

// -
Expr NumberExpr::eval(const EvalVisitor& visitor, Env& ctx) const{
    return visitor.eval(*this, ctx);
}

// ---------------
// -*- NegExpr -*-
// ---------------
NegExpr::NegExpr(const Expr& expr)
: m_rhs{expr}
{}

// -
NegExpr::NegExpr(Expr&& expr)
: m_rhs{std::move(expr)}
{}

// -
Expr NegExpr::eval(const EvalVisitor& visitor, Env& ctx) const{
    return visitor.eval(*this, ctx);
}

// ---------------
// -*- AddExpr -*-
// ---------------
AddExpr::AddExpr(const Expr& lhs, const Expr& rhs)
: m_lhs{lhs}
, m_rhs{rhs}
{}

// -*-
AddExpr::AddExpr(Expr&& lhs, Expr&& rhs)
: m_lhs{std::move(lhs)}
, m_rhs{std::move(rhs)}
{}

// -
Expr AddExpr::eval(const EvalVisitor& visitor, Env& ctx) const{
    return visitor.eval(*this, ctx);
}

// ---------------
// -*- MulExpr -*-
// ---------------
MulExpr::MulExpr(const Expr& lhs, const Expr& rhs)
: m_lhs{lhs}
, m_rhs{rhs}
{}

// -
MulExpr::MulExpr(Expr&& lhs, Expr&& rhs)
: m_lhs{std::move(lhs)}
, m_rhs{std::move(rhs)}
{}

// -
Expr MulExpr::eval(const EvalVisitor& visitor, Env& ctx) const{
    return visitor.eval(*this, ctx);
}

// ---------------
// -*- PowExpr -*-
// ---------------
PowExpr::PowExpr(const Expr& base, const Expr& expo)
: m_base{base}
, m_exponent{expo}
{}

// -
PowExpr::PowExpr(Expr&& base, Expr&& expo)
: m_base{std::move(base)}
, m_exponent{std::move(expo)}
{}

// -
Expr PowExpr::eval(const EvalVisitor& visitor, Env& ctx) const{
    return visitor.eval(*this, ctx);
}

// ----------------
// -*- CallExpr -*-
// ----------------
CallExpr::CallExpr(const Str& name, const Vec<Expr>& args)
: m_name{name}
, m_args{args}
{}

// -
CallExpr::CallExpr(Str&& name, Vec<Expr>&& args)
: m_name{std::move(name)}
, m_args{std::move(args)}
{}

// -
Expr CallExpr::eval(const EvalVisitor& visitor, Env& ctx) const{
    return visitor.eval(*this, ctx);
}

// --------------------
// -*- EquationExpr -*-
// --------------------
EquationExpr::EquationExpr(const Expr& lhs, const Expr& rhs)
: m_lhs{lhs}
, m_rhs{rhs}
{}

// -
EquationExpr::EquationExpr(Expr&& lhs, Expr&& rhs)
: m_lhs{std::move(lhs)}
, m_rhs{std::move(rhs)}
{}

// -
Expr EquationExpr::eval(const EvalVisitor& visitor, Env& ctx) const{
    return visitor.eval(*this, ctx);
}

// ------------------
// -*- SystemExpr -*-
// ------------------
SystemExpr::SystemExpr(const Vec<EquationExpr>& equations)
: m_equations{equations}
{}

// -
SystemExpr::SystemExpr(Vec<EquationExpr>&& equations)
: m_equations{std::move(equations)}
{}

// -
Expr SystemExpr::eval(const EvalVisitor& visitor, Env& ctx) const{
    return visitor.eval(*this, ctx);
}

// ---------------
// -*- LetStmt -*-
// ---------------
LetStmt::LetStmt(const Str& name, const Expr& expr)
: m_name{name}
, m_expr{expr}
{}

// -
LetStmt::LetStmt(Str&& name, Expr&& expr)
: m_name{std::move(name)}
, m_expr{std::move(expr)}
{}

// -
void LetStmt::execute(const ExecuteVisitor& visitor, Env& ctx){
    visitor.execute(*this, ctx);
}

// ---------------
// -*- FunStmt -*-
// ---------------
FunStmt::FunStmt(const Str& name, const Lambda& lambda)
: m_name{name}
, m_lambda{lambda}
{}

// -
FunStmt::FunStmt(Str&& name, Lambda&& lambda)
: m_name{std::move(name)}
, m_lambda{std::move(lambda)}
{}

// -
void FunStmt::execute(const ExecuteVisitor& visitor, Env& ctx){
    visitor.execute(*this, ctx);
}

// -------------------------
// -*- Utility functions -*-
// -------------------------
//! @brief Create a SymbolExpr object shared pointer
Expr make_symbol_expr(const Str& name){
    return std::make_shared<SymbolExpr>(name);
}

//! @brief Create a NumberExpr object shared pointer
Expr make_number_expr(void){
    return std::make_shared<NumberExpr>();
}

// -
Expr make_number_expr(i64 num){
    return std::make_shared<NumberExpr>(num);
}

// -
Expr make_number_expr(f64 num){
    return std::make_shared<NumberExpr>(num);
}

//! @brief Create a NegExpr object shared pointer.
Expr make_neg_expr(const Expr& expr){
    return std::make_shared<NegExpr>(expr);
}

// -
Expr make_neg_expr(Expr&& expr){
    return std::make_shared<NegExpr>(expr);
}

//! @brief Create a AddExpr object shared pointer.
Expr make_add_expr(const Expr& lhs, const Expr& rhs){
    return std::make_shared<AddExpr>(lhs, rhs);
}

// -
Expr make_add_expr(Expr&& lhs, Expr&& rhs){
    return std::make_shared<AddExpr>(lhs, rhs);
}

//! @brief Create a MulExpr object shared pointer.
Expr make_mul_expr(const Expr& lhs, const Expr& rhs){
    return std::make_shared<MulExpr>(lhs, rhs);
}

// -
Expr make_mul_expr(Expr&& lhs, Expr&& rhs){
    return std::make_shared<MulExpr>(lhs, rhs);
}

//! @brief Create a PowExpr object shared pointer.
Expr make_pow_expr(const Expr& base, const Expr& exponent){
    return std::make_shared<PowExpr>(base, exponent);
}

// -
Expr make_pow_expr(Expr&& base, Expr&& exponent){
    return std::make_shared<PowExpr>(base, exponent);
}

//! @brief Create a CallExpr object shared pointer.
Expr make_call_expr(const Str& name, const Vec<Expr>& args){
    return std::make_shared<CallExpr>(name, args);
}

// -
Expr make_call_expr(Str&& name, Vec<Expr>&& args){
    return std::make_shared<CallExpr>(name, args);
}

// --------------------
// -*- EquationExpr -*-
// --------------------
Expr make_equation_expr(const Expr& lhs, const Expr& rhs){
    return std::make_shared<EquationExpr>(lhs, rhs);
}

// -
Expr make_equation_expr(Expr&& lhs, Expr&& rhs){
    return std::make_shared<EquationExpr>(lhs, rhs);
}

/*
// -
Expr make_system_expr(const Vec<EquationExpr>& equations){}
Expr make_system_expr(Vec<EquationExpr>&& equations){}

Stmt make_let_stmt(const Str& name, const Expr& expr){}
Stmt make_let_stmt(Str&& name, Expr&& expr){}
Stmt make_fun_stmt(const Str& name, const Lambda& lambda){}
Stmt make_fun_stmt(Str&& name, Lambda&& lambda){}

*/

// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola::core                                 -*-
// -*----------------------------------------------------------------*-