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
struct Helper;

// -*-
class Evaluator final: protected EvalVisitor {
public:
    explicit Evaluator(Env& ctx);
    ~Evaluator() = default;

    Expr eval(const Expr& expr);
    Expr eval(Expr&& expr);

    // -*---------------------*-
    // -*-  C O M M A N D S  -*-
    // -*---------------------*-
    static Expr diff(Evaluator& evaluator, const Expr& expr, const Str& var);
    static Expr simplify(Evaluator& evaluator, const Expr& expr);
    static Expr expand(Evaluator& evaluator, const Expr& expr);
    static Expr factor(Evaluator& evaluator, const Expr& expr);
    static Expr integral(Evaluator& evaluator, const Expr& expr, const Str& var);
    static Expr integrate(Evaluator& evaluator, const Expr& expr, const Str& var, f64 vmin, f64 vmax);
    static Expr taylor(Evaluator& evaluator, const Expr& expr, const Str& var, f64 center, u32 order);
    static Expr limit(Evaluator& evaluator, const Expr& expr, const Str& var, f64 loc);
    static Expr roots(Evaluator& evaluator, const Expr& expr, const Str& var);
    static Expr roots(Evaluator& evaluator, const Expr& expr, const Str& var, f64 tolerance);
    static Expr solve(Evaluator& evaluator, const EquationExpr& equation);
    static Expr solve(Evaluator& evaluator, const EquationExpr& equation, f64 tolerance);
    static Expr solve(Evaluator& evaluator, const SystemExpr& system);
    static Expr solve(Evaluator& evaluator, const SystemExpr& system, f64 tolerance);
    static Expr fibonacci(Evaluator& evaluator, u32 idx);
    static Expr factorial(Evaluator& evaluator, u32 idx);
    static Expr modulo(Evaluator& evaluator, const NumberExpr& lhs, const NumberExpr& rhs);
    static Expr prime(Evaluator& evaluator, u32 idx);
    static Expr gcd(Evaluator& evaluator, i64 x, i64 y);
    static Expr lcm(Evaluator& evaluator, i64 x, i64 y);
    static Expr help(Evaluator& evaluator, const Str& cmd);
    static Expr show(Evaluator& evaluator, const Str& query);
    static Expr plot(Evaluator& evaluator, const Vec<Expr>& args);
    static Expr config(Evaluator& evaluator, const Str& query, const Expr& expr);
    static Expr equation(Evaluator& evaluator, const Expr& lhs, const Expr& rhs, const Str& var);
    static Expr system(Evaluator& evaluator, const Vec<Expr>& lhs, const Vec<Expr>& rhs, const Vec<Str>& vars);
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

    Helper m_helper;

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

struct Helper{
    Str help(const Str& name){
        using HelpFn = Str (*)(void);
        HashMap<Str, HelpFn> helpers = {
            { "diff", Helper::help_diff },
            { "simplify", Helper::help_simplify },
            { "expand", Helper::help_expand },
            { "factor", Helper::help_factor },
            { "integral", Helper::help_integral },
            { "integrate", Helper::help_integrate },
            { "taylor", Helper::help_taylor },
            { "limit", Helper::help_limit },
            { "roots", Helper::help_roots },
            { "solve", Helper::help_solve },
            { "fibonacci", Helper::help_fibonacci },
            { "factorial", Helper::help_factorial },
            { "modulo", Helper::help_modulo },
            { "prime", Helper::help_prime },
            { "gcd", Helper::help_gcd },
            { "lcm", Helper::help_lcm },
            { "help", Helper::help_help },
            { "show", Helper::help_show },
            { "plot", Helper::help_plot },
            { "config", Helper::help_config },
            { "equation", Helper::help_equation },
            { "system", Helper::help_system },
        };

        auto entry = helpers.find(name);
        if(entry == helpers.end()){ return ""; }
        return entry->second();
    }

private:
    static Str help_diff(void);
    static Str help_simplify(void);
    static Str help_expand(void);
    static Str help_factor(void);
    static Str help_integral(void);
    static Str help_integrate(void);
    static Str help_taylor(void);
    static Str help_limit(void);
    static Str help_roots(void);
    static Str help_solve(void);
    static Str help_fibonacci(void);
    static Str help_factorial(void);
    static Str help_modulo(void);
    static Str help_prime(void);
    static Str help_gcd(void);
    static Str help_lcm(void);
    static Str help_help(void);
    static Str help_show(void);
    static Str help_plot(void);
    static Str help_config(void);
    static Str help_equation(void);
    static Str help_system(void);
};


// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola::core                                 -*-
// -*----------------------------------------------------------------*-