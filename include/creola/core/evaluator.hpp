#pragma once

#include "creola/core/visitors.hpp"
#include "creola/core/diff.hpp"
#include "creola/core/expand.hpp"
#include "creola/core/factor.hpp"
#include "creola/core/integrate.hpp"
#include "creola/core/limit.hpp"
#include "creola/core/roots.hpp"
#include "creola/core/simplify.hpp"
#include "creola/core/taylor.hpp"
#include "creola/core/solver.hpp"

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola::core                               -*-
// -*----------------------------------------------------------------*-
namespace creola::core{
// -
class Evaluator final: protected EvalVisitor {
public:
    explicit Evaluator(Env& ctx);
    ~Evaluator() = default;

    Expr eval(const Expr& expr);
    Expr eval(Expr&& expr);

    // -*---------------------*-
    // -*-  C O M M A N D S  -*-
    // -*---------------------*-
    static Expr diff(const Expr& expr, const Str& var);
    static Expr simplify(const Expr& expr);
    static Expr expand(const Expr& expr);
    static Expr factor(const Expr& expr);
    static Expr integral(const Expr& expr, const Str& var);
    static Expr integrate(const Expr& expr, const Str& var, f64 vmin, f64 vmax);
    static Expr taylor(const Expr& expr, const Str& var, f64 center, u32 order);
    static Expr limit(const Expr& expr, const Str& var, f64 loc);
    static Expr roots(const Expr& expr, const Str& var);
    static Expr roots(const Expr& expr, const Str& var, f64 tolerance);
    static Expr solve(const EquationExpr& equation);
    static Expr solve(const EquationExpr& equation, f64 tolerance);
    static Expr solve(const SystemExpr& system);
    static Expr solve(const SystemExpr& system, f64 tolerance);
    static Expr fibonacci(u32 idx);
    static Expr factorial(u32 idx);
    static Expr modulo(const NumberExpr& lhs, const NumberExpr rhs);
    static Expr prime(u32 idx);
    static Expr gcd(i64 x, i64 y);
    static Expr lcm(i64 x, i64 y);
    static Expr help(const Str& cmd);
    static Expr show(const Str& query);
    static Expr plot(const Vec<Expr>& args);
    static Expr config(const Str& query, const Expr& expr);
    static Expr equation(const Expr& lhs, const Expr& rhs);
    static Expr system(const Vec<Expr>& lhs, const Vec<Expr>& rhs);
    // -*-

private:
    Env& m_ctx;
    // -----------------------------------------------------------------------
    // -*- diff, factor, expand, integrate, limit, roots, simplify, taylor -*-
    // -----------------------------------------------------------------------
    RootsFinder m_rootsFinder;
    LimitFinder m_limitFinder;
    Differentiator m_differentiator;
    Integrator m_integrator;
    Simplifier m_simplifier;
    Expander m_expander;
    Factorizer m_factorizer;
    TaylorExpander m_taylorExpander;
    Solver m_solver;

    Expr eval(const SymbolExpr& expr, Env& ctx) const override;
    Expr eval(const NumberExpr& expr, Env& ctx) const override;
    Expr eval(const NegExpr& expr, Env& ctx) const override;
    Expr eval(const AddExpr& expr, Env& ctx) const override;
    Expr eval(const MulExpr& expr, Env& ctx) const override;
    Expr eval(const PowExpr& expr, Env& ctx) const override;
    Expr eval(const CallExpr& expr, Env& ctx) const override;
    Expr eval(const EquationExpr& expr, Env& ctx) const override;
    Expr eval(const SystemExpr& expr, Env& ctx) const override;
};

// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola::core                                 -*-
// -*----------------------------------------------------------------*-