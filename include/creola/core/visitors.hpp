#pragma once

#include "creola/core/common.hpp"


// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola::core                               -*-
// -*----------------------------------------------------------------*-
namespace creola::core{
// -
// -*- Foreward declarations
class Env;
class SymbolExpr;
class NumberExpr;
class NegExpr;
class AddExpr;
class MulExpr;
class PowExpr;
class CallExpr;
class EquationExpr;
class SystemExpr;

class LetStmt;
class FunStmt;

class Box;

// --------------------
// -*- AST Visitors -*-
// --------------------
struct EvalVisitor{
    virtual ~EvalVisitor() = default;
    virtual Expr eval(const SymbolExpr& expr, Env& ctx) const = 0;
    virtual Expr eval(const NumberExpr& expr, Env& ctx) const = 0;
    virtual Expr eval(const NegExpr& expr, Env& ctx) const = 0;
    virtual Expr eval(const AddExpr& expr, Env& ctx) const = 0;
    virtual Expr eval(const MulExpr& expr, Env& ctx) const = 0;
    virtual Expr eval(const PowExpr& expr, Env& ctx) const = 0;
    virtual Expr eval(const CallExpr& expr, Env& ctx) const = 0;
    virtual Expr eval(const EquationExpr& expr, Env& ctx) const = 0;
    virtual Expr eval(const SystemExpr& expr, Env& ctx) const = 0;
};

// -
struct ExecuteVisitor{
    virtual ~ExecuteVisitor() = default;
    virtual void execute(const LetStmt& stmt, Env& ctx) const = 0;
    virtual void execute(const FunStmt& stmt, Env& ctx) const = 0;
};

// ----------------------------------
// -*- Symbolic Calculus Visitors -*-
// ----------------------------------
struct SimplifyVisitor{
    virtual ~SimplifyVisitor() = default;
    virtual Expr simplify(const SymbolExpr& expr) = 0;
    virtual Expr simplify(const NumberExpr& expr) = 0;
    virtual Expr simplify(const NegExpr& expr) = 0;
    virtual Expr simplify(const AddExpr& expr) = 0;
    virtual Expr simplify(const MulExpr& expr) = 0;
    virtual Expr simplify(const PowExpr& expr) = 0;
    virtual Expr simplify(const CallExpr& expr) = 0;
    virtual Expr simplify(const EquationExpr& expr) = 0;
    virtual Expr simplify(const SystemExpr& expr) = 0;
};

struct ExpandVisitor{
    virtual ~ExpandVisitor() = default;
    virtual Expr expand(const SymbolExpr& expr) = 0;
    virtual Expr expand(const NumberExpr& expr) = 0;
    virtual Expr expand(const NegExpr& expr) = 0;
    virtual Expr expand(const AddExpr& expr) = 0;
    virtual Expr expand(const MulExpr& expr) = 0;
    virtual Expr expand(const PowExpr& expr) = 0;
    virtual Expr expand(const CallExpr& expr) = 0;
    virtual Expr expand(const EquationExpr& expr) = 0;
    virtual Expr expand(const SystemExpr& expr) = 0;
};

struct FactorVisitor{
    virtual ~FactorVisitor() = default;
    virtual Expr factor(const SymbolExpr& expr, const Str& var) = 0;
    virtual Expr factor(const NumberExpr& expr, const Str& var) = 0;
    virtual Expr factor(const NegExpr& expr, const Str& var) = 0;
    virtual Expr factor(const AddExpr& expr, const Str& var) = 0;
    virtual Expr factor(const MulExpr& expr, const Str& var) = 0;
    virtual Expr factor(const PowExpr& expr, const Str& var) = 0;
    virtual Expr factor(const CallExpr& expr, const Str& var) = 0;
    virtual Expr factor(const EquationExpr& expr, const Str& var) = 0;
    virtual Expr factor(const SystemExpr& expr, const Str& var) = 0;
};

// -
struct DiffVisitor{
    virtual ~DiffVisitor() = default;
    virtual Expr diff(const SymbolExpr& expr, const Str& var) = 0;
    virtual Expr diff(const NumberExpr& expr, const Str& var) = 0;
    virtual Expr diff(const NegExpr& expr, const Str& var) = 0;
    virtual Expr diff(const AddExpr& expr, const Str& var) = 0;
    virtual Expr diff(const MulExpr& expr, const Str& var) = 0;
    virtual Expr diff(const PowExpr& expr, const Str& var) = 0;
    virtual Expr diff(const CallExpr& expr, const Str& var) = 0;
    virtual Expr diff(const EquationExpr& expr, const Str& var) = 0;
    virtual Expr diff(const SystemExpr& expr, const Str& var) = 0;
};


struct IntegralVisitor{
    virtual ~IntegralVisitor() = default;
    virtual Expr integral(const SymbolExpr& expr, const Str& var) = 0;
    virtual Expr integral(const NumberExpr& expr, const Str& var) = 0;
    virtual Expr integral(const NegExpr& expr, const Str& var) = 0;
    virtual Expr integral(const AddExpr& expr, const Str& var) = 0;
    virtual Expr integral(const MulExpr& expr, const Str& var) = 0;
    virtual Expr integral(const PowExpr& expr, const Str& var) = 0;
    virtual Expr integral(const CallExpr& expr, const Str& var) = 0;
    virtual Expr integral(const EquationExpr& expr, const Str& var) = 0;
    virtual Expr integral(const SystemExpr& expr, const Str& var) = 0;
};


struct TaylorVisitor{
    virtual ~TaylorVisitor() = default;
    virtual Expr taylor(const SymbolExpr& expr, const Str& var, f64 center, u32 order) = 0;
    virtual Expr taylor(const NumberExpr& expr, const Str& var, f64 center, u32 order) = 0;
    virtual Expr taylor(const NegExpr& expr, const Str& var, f64 center, u32 order) = 0;
    virtual Expr taylor(const AddExpr& expr, const Str& var, f64 center, u32 order) = 0;
    virtual Expr taylor(const MulExpr& expr, const Str& var, f64 center, u32 order) = 0;
    virtual Expr taylor(const PowExpr& expr, const Str& var, f64 center, u32 order) = 0;
    virtual Expr taylor(const CallExpr& expr, const Str& var, f64 center, u32 order) = 0;
    virtual Expr taylor(const EquationExpr& expr, const Str& var, f64 center, u32 order) = 0;
    virtual Expr taylor(const SystemExpr& expr, const Str& var, f64 center, u32 order) = 0;
};

struct LimitVisitor{
    virtual ~LimitVisitor() = default;
    virtual Expr limit(const SymbolExpr& expr, const Str& var, f64 loc) = 0;
    virtual Expr limit(const NumberExpr& expr, const Str& var, f64 loc) = 0;
    virtual Expr limit(const NegExpr& expr, const Str& var, f64 loc) = 0;
    virtual Expr limit(const AddExpr& expr, const Str& var, f64 loc) = 0;
    virtual Expr limit(const MulExpr& expr, const Str& var, f64 loc) = 0;
    virtual Expr limit(const PowExpr& expr, const Str& var, f64 loc) = 0;
    virtual Expr limit(const CallExpr& expr, const Str& var, f64 loc) = 0;
    virtual Expr limit(const EquationExpr& expr, const Str& var, f64 loc) = 0;
    virtual Expr limit(const SystemExpr& expr, const Str& var, f64 loc) = 0;
};

// -
struct RootsVisitor{
    virtual ~RootsVisitor() = default;
    virtual Vec<Expr> roots(const SymbolExpr& expr, const Str& var, f64 tol=1e-6) = 0;
    virtual Vec<Expr> roots(const NumberExpr& expr, const Str& var, f64 tol=1e-6) = 0;
    virtual Vec<Expr> roots(const NegExpr& expr, const Str& var, f64 tol=1e-6) = 0;
    virtual Vec<Expr> roots(const AddExpr& expr, const Str& var, f64 tol=1e-6) = 0;
    virtual Vec<Expr> roots(const MulExpr& expr, const Str& var, f64 tol=1e-6) = 0;
    virtual Vec<Expr> roots(const PowExpr& expr, const Str& var, f64 tol=1e-6) = 0;
    virtual Vec<Expr> roots(const CallExpr& expr, const Str& var, f64 tol=1e-6) = 0;
    virtual Vec<Expr> roots(const EquationExpr& expr, const Str& var, f64 tol=1e-6) = 0;
    virtual Vec<Expr> roots(const SystemExpr& expr, const Str& var, f64 tol=1e-6) = 0;
};

// -*-
struct SolveVisitor{
    virtual ~SolveVisitor() = default;
    virtual Vec<Expr> solve(const EquationExpr& expr, f64 tol=1e-6) = 0;
    virtual Vec<Expr> solve(const SystemExpr& expr, f64 tol=1e-6) = 0;
};


// -------------------------------
// -*- Pretty Printer Visitors -*-
// -------------------------------
struct PrintVisitor{
    virtual ~PrintVisitor() = default;
    virtual void print(std::ostream& os, const SymbolExpr& expr) = 0;
    virtual void print(std::ostream& os, const NumberExpr& expr) = 0;
    virtual void print(std::ostream& os, const NegExpr& expr) = 0;
    virtual void print(std::ostream& os, const AddExpr& expr) = 0;
    virtual void print(std::ostream& os, const MulExpr& expr) = 0;
    virtual void print(std::ostream& os, const PowExpr& expr) = 0;
    virtual void print(std::ostream& os, const CallExpr& expr) = 0;
    virtual void print(std::ostream& os, const EquationExpr& expr) = 0;
    virtual void print(std::ostream& os, const SystemExpr& expr) = 0;
};

struct ToBoxVisitor{
    virtual ~ToBoxVisitor() = default;
    virtual void to_box(const SymbolExpr& expr, Box& box) const = 0;
    virtual void to_box(const NumberExpr& expr, Box& box) const = 0;
    virtual void to_box(const NegExpr& expr, Box& box) const = 0;
    virtual void to_box(const AddExpr& expr, Box& box) const = 0;
    virtual void to_box(const MulExpr& expr, Box& box) const = 0;
    virtual void to_box(const PowExpr& expr, Box& box) const = 0;
    virtual void to_box(const CallExpr& expr, Box& box) const = 0;
    virtual void to_box(const EquationExpr& expr, Box& box) const = 0;
    virtual void to_box(const SystemExpr& expr, Box& box) const = 0;
};



// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola::core                                 -*-
// -*----------------------------------------------------------------*-