#pragma once

#include "creola/core/visitors.hpp"

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola::core                               -*-
// -*----------------------------------------------------------------*-
namespace creola::core{
// -
//! @todo: Add support for complex numbers

struct Lambda final{
    Vec<Str> params;
    Expr body;

    explicit Lambda(const Vec<Str>& params, const Expr& body);
    explicit Lambda(Vec<Str>&& params, Expr&& body);
    Lambda(const Lambda&) = default;
    Lambda(Lambda&&) = default;
    ~Lambda() = default;
    Lambda& operator=(const Lambda&) = default;
    Lambda& operator=(Lambda&&) = default;
};

// -*-
struct AstBase{
    virtual ~AstBase() = default;
};

struct ExprBase: public AstBase{
    virtual ~ExprBase() = default;
    virtual Expr eval(const EvalVisitor& visitor, Env& ctx) const = 0;
};

struct StmtBase: public AstBase {
    virtual ~StmtBase() = default;
    virtual void execute(const ExecuteVisitor& visitor, Env& ctx) = 0;
};

// -*-
class SymbolExpr final: public ExprBase {
public:
    explicit SymbolExpr(const Str& sym);
    SymbolExpr(const SymbolExpr&) = default;
    SymbolExpr(SymbolExpr&&) = default;
    SymbolExpr& operator=(const SymbolExpr&) const = default;
    SymbolExpr& operator=(SymbolExpr&&) = default;

    ~SymbolExpr() = default;

    Expr eval(const EvalVisitor& visitor, Env& ctx) const override;

    const Str& text(void) const { return this->m_text; }
    Str& text(void) { return this->m_text; }

private:
    Str m_text;
};

// -*-
class NumberExpr final: public ExprBase{
public:
    explicit NumberExpr();
    explicit NumberExpr(i64 val);
    explicit NumberExpr(f64 val);
    NumberExpr(const NumberExpr&) = default;
    NumberExpr(NumberExpr&&) = default;
    ~NumberExpr() = default;

    NumberExpr& operator=(const NumberExpr&) = default;
    NumberExpr& operator=(NumberExpr&&) = default;

    Expr eval(const EvalVisitor& visitor, Env& ctx) const override;

    i64 as_integer(void) const;
    f64 as_float(void) const;
    bool is_integer(void) const;

    Expr eval(const EvalVisitor& visitor, Env& ctx) const override;

private:
    using Data = std::variant<i64, f64>;
    Data m_data;
};

// -*-
class NegExpr final: public ExprBase{
public:
    explicit NegExpr(const Expr& expr);
    explicit NegExpr(Expr&& expr);
    NegExpr(const NegExpr&) = default;
    NegExpr(NegExpr&&) = default;

    ~NegExpr() = default;

    NegExpr& operator=(const NegExpr&) = default;
    NegExpr& operator=(NegExpr&&) = default;

    Expr eval(const EvalVisitor& visitor, Env& ctx) const override;

    const Expr& rhs(void) const{ return this->m_rhs; }
    Expr& rhs(void){ return this->m_rhs; }

private:
    Expr m_rhs;
};

// -*-
class AddExpr final: public ExprBase{
public:
    explicit AddExpr(const Expr& lhs, const Expr& rhs);
    explicit AddExpr(Expr&& lhs, Expr&& rhs);
    AddExpr(const AddExpr&) = default;
    AddExpr(AddExpr&&) = default;
    ~AddExpr() = default;
    AddExpr& operator=(const AddExpr&) = default;
    AddExpr& operator=(AddExpr&&) = default;

    Expr eval(const EvalVisitor& visitor, Env& ctx) const override;

    const Expr& lhs(void) const{ return this->m_lhs; }
    const Expr& rhs(void) const{ return this->m_rhs; }
    Expr& lhs(void){ return this->m_lhs; }
    Expr& rhs(void){ return this->m_rhs; }

private:
    Expr m_lhs;
    Expr m_rhs;
};

// -*-
class MulExpr final: public ExprBase{
public:
    explicit MulExpr(const Expr& lhs, const Expr& rhs);;
    explicit MulExpr(Expr&& lhs, Expr&& rhs);
    MulExpr(const MulExpr&) = default;
    MulExpr(MulExpr&&) = default;
    ~MulExpr() = default;
    MulExpr& operator=(const MulExpr&) = default;
    MulExpr& operator=(MulExpr&) = default;

    Expr eval(const EvalVisitor& visitor, Env& ctx) const override;

    const Expr& lhs(void) const{ return this->m_lhs; }
    const Expr& rhs(void) const{ return this->m_rhs; }
    Expr& lhs(void){ return this->m_lhs; }
    Expr& rhs(void){ return this->m_rhs; }

private:
    Expr m_lhs;
    Expr m_rhs;
};

// -*-
class PowExpr final: public ExprBase{
public:
    explicit PowExpr(const Expr& base, const Expr& expo);
    explicit PowExpr(Expr&& base, Expr&& expo);
    PowExpr(const PowExpr&) = default;
    PowExpr(PowExpr&) = default;
    ~PowExpr() = default;
    PowExpr& operator=(const PowExpr&) = default;
    PowExpr& operator=(PowExpr&&) = default;

    Expr eval(const EvalVisitor& visitor, Env& ctx) const override;

    const Expr& base(void) const { return this->m_base; }
    const Expr& exponent(void) const{ return this->m_exponent; }
    Expr& base(void) { return this->m_base; }
    Expr& exponent(void) { return this->m_exponent; }

private:
    Expr m_base;
    Expr m_exponent;
};

// -*-
class CallExpr final: public ExprBase{
public:
    explicit CallExpr(const Str& name, const Vec<Expr>& args);
    explicit CallExpr(Str&& name, Vec<Expr>&& args);
    CallExpr(const CallExpr&) = default;
    CallExpr(CallExpr&&) = default;
    ~CallExpr() = default;
    CallExpr& operator=(const CallExpr&) = default;
    CallExpr& operator=(CallExpr&&) = default;

    Expr eval(const EvalVisitor& visitor, Env& ctx) const override;

    const Str& name(void) const{ return this->m_name; }
    const Vec<Expr>& args(void) const{ return this->m_args; }
    Str& name(void){ return this->m_name; }
    Vec<Expr>& args(void){ return this->m_args; }

private:
    Str m_name;
    Vec<Expr> m_args;
};

// -*-
class EquationExpr final: public ExprBase{
public:
    explicit EquationExpr(const Expr& lhs, const Expr& rhs);
    explicit EquationExpr(Expr&& lhs, Expr&& rhs);
    EquationExpr(const EquationExpr&) = default;
    EquationExpr(EquationExpr&&) = default;
    ~EquationExpr() = default;
    EquationExpr& operator=(const EquationExpr&) = default;
    EquationExpr& operator=(EquationExpr&&) = default;

    Expr eval(const EvalVisitor& visitor, Env& ctx) const override;

    const Expr& lhs(void) const{ return this->m_lhs; }
    const Expr& rhs(void) const{ return this->m_rhs; }
    Expr& lhs(void){ return this->m_lhs; }
    Expr& rhs(void){ return this->m_rhs; }

private:
    Expr m_lhs;
    Expr m_rhs;
};

// -*-
class SystemExpr final: public ExprBase{
public:
    explicit SystemExpr(const Vec<EquationExpr>& equations);
    explicit SystemExpr(Vec<EquationExpr>&& equations);
    SystemExpr(const SystemExpr&) = default;
    SystemExpr(SystemExpr&&) = default;
    ~SystemExpr() = default;
    SystemExpr& operator=(const SystemExpr&) = default;
    SystemExpr& operator=(SystemExpr&&) = default;

    Expr eval(const EvalVisitor& visitor, Env& ctx) const override;

    const Vec<EquationExpr>& equations(void) const{ return this->m_equations; }
    Vec<EquationExpr>& equations(void){ return this->m_equations; }

private:
    Vec<EquationExpr> m_equations;
};

// -*-
class LetStmt final: public StmtBase{
public:
    explicit LetStmt(const Str& name, const Expr& expr);
    explicit LetStmt(Str&& name, Expr&& expr);
    LetStmt(const LetStmt&) = default;
    LetStmt(LetStmt&&) = default;
    ~LetStmt() = default;
    LetStmt& operator=(const LetStmt&) = default;
    LetStmt& operator=(LetStmt&&) = default;

    void execute(const ExecuteVisitor& visitor, Env& ctx) override;

    const Str& name(void) const{ return this->m_name; }
    const Expr& expr(void) const{ return this->m_expr; }
    Str& name(void){ return this->m_name; }
    Expr& expr(void){ return this->m_expr; }

private:
    Str m_name;
    Expr m_expr;
};

// -*-
class FunStmt final: public StmtBase{
public:
    explicit FunStmt(const Str& name, const Lambda& lambda);
    explicit FunStmt(Str&& name, Lambda&& lambda);
    FunStmt(const FunStmt&) = default;
    FunStmt(FunStmt&&) = default;
    ~FunStmt() = default;
    FunStmt& operator=(const FunStmt&) = default;
    FunStmt& operator=(FunStmt&&) = default;

    void execute(const ExecuteVisitor& visitor, Env& ctx) override;

    const Str& name(void) const{ return this->m_name; }
    const Lambda& lambda(void) const{ return this->m_lambda; }
    Str& name(void){ return this->m_name; }
    Lambda& lambda(void){ return this->m_lambda; }

private:
    Str m_name;
    Lambda m_lambda;
};

Expr make_unary_expr(const Expr& expr);
Expr make_unary_expr(Expr&& expr);
Expr make_binary_expr(const Expr& lhs, const Expr& rhs);
Expr make_binary_expr(Expr&& lhs, Expr&& rhs);

Expr make_symbol_expr(const Str& name);
Expr make_number_expr(void);
Expr make_number_expr(i64 num);
Expr make_number_expr(f64 num);
Expr make_neg_expr(const Expr& expr);
Expr make_neg_expr(Expr&& expr);
Expr make_add_expr(const Expr& lhs, const Expr& rhs);
Expr make_add_expr(Expr&& lhs, Expr&& rhs);
Expr make_mul_expr(const Expr& lhs, const Expr& rhs);
Expr make_mul_expr(Expr&& lhs, Expr&& rhs);
Expr make_pow_expr(const Expr& base, const Expr& exponent);
Expr make_pow_expr(Expr&& base, Expr&& exponent);
Expr make_call_expr(const Str& name, const Vec<Expr>& args);
Expr make_call_expr(Str&& name, Vec<Expr>&& args);
Expr make_equation_expr(const Expr& lhs, const Expr& rhs);
Expr make_equation_expr(Expr&& lhs, Expr&& rhs);
Expr make_system_expr(const Vec<EquationExpr>& equations);
Expr make_system_expr(Vec<EquationExpr>&& equations);

Stmt make_let_stmt(const Str& name, const Expr& expr);
Stmt make_let_stmt(Str&& name, Expr&& expr);
Stmt make_fun_stmt(const Str& name, const Lambda& lambda);
Stmt make_fun_stmt(Str&& name, Lambda&& lambda);

/**
 * @brief Returns true if `expr` is a symbol-expression.
 * 
 * @param expr 
 * @return true 
 * @return false 
 */
static inline bool is_symbol_expr(const Expr& expr){
    return (std::dynamic_pointer_cast<SymbolExpr>(expr) ? true : false);
}

/**
 * @brief Returns true if `expr` is a number-expression.
 * 
 * @param expr 
 * @return true 
 * @return false 
 */
static inline bool is_number_expr(const Expr& expr){
    return (std::dynamic_pointer_cast<NumberExpr>(expr) ? true : false);
}

/**
 * @brief Return true if `expr` is neg[ate]-expression.
 * 
 * @param expr 
 * @return true 
 * @return false 
 */
static inline bool is_neg_expr(const Expr& expr){
    return (std::dynamic_pointer_cast<NegExpr>(expr) ? true : false);
}

/**
 * @brief Return true if `expr` is Add-expression type.
 * 
 * @param expr 
 * @return true 
 * @return false 
 */
static inline bool is_add_expr(const Expr& expr){
    return (std::dynamic_pointer_cast<AddExpr>(expr) ? true : false);
}

/**
 * @brief Returns true if `expr` is MulExpr type.
 * 
 * @param expr 
 * @return true 
 * @return false 
 */
static inline bool is_mul_expr(const Expr& expr){
    return (std::dynamic_pointer_cast<MulExpr>(expr) ? true : false);
}

static inline bool is_pow_expr(const Expr& expr);
static inline bool is_call_expr(const Expr& expr);
static inline bool is_equation_expr(const Expr& expr);
static inline bool is_system_expr(const Expr& expr);

static inline SymbolExpr as_symbol_expr(const Expr& expr);
static inline NumberExpr as_number_expr(const Expr& expr);
static inline NegExpr as_neg_expr(const Expr& expr);
static inline AddExpr as_add_expr(const Expr& expr);
static inline MulExpr as_mul_expr(const Expr& expr);
static inline PowExpr as_pow_expr(const Expr& expr);
static inline CallExpr as_call_expr(const Expr& expr);
static inline EquationExpr as_equation_expr(const Expr& expr);
static inline SystemExpr as_system_expr(const Expr& expr);


// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola::core                                 -*-
// -*----------------------------------------------------------------*-