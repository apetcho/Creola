#pragma once

#include "creola/core/visitors.hpp"
#include "creola/core/diff.hpp"
#include "creola/core/simplify.hpp"
#include "creola/core/integrate.hpp"
#include "creola/core/expand.hpp"
#include "creola/core/factor.hpp"
#include "creola/core/taylor.hpp"
#include "creola/core/roots.hpp"
#include "creola/core/limit.hpp"
#include "creola/core/solver.hpp"


// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola::core                               -*-
// -*----------------------------------------------------------------*-
namespace creola::core{
// -

struct Factorial{
    static Dict<u32, u64> memo;
    u64 operator()(u32 n);
};

struct Fibonacci{
    static Dict<u32, u64> memo;
    u64 operator()(u32 n);
};

struct Prime{
    static Dict<u32, u64> memo;
    u64 operator()(u32 n);
};

// --
class Symbolic final{
public:
    Symbolic() = default;
    explicit Symbolic(const Simplifier& simplifier);
    explicit Symbolic(const Differentiator& differentiator);
    explicit Symbolic(const Integrator& integrator);
    explicit Symbolic(const Expander& expander);
    explicit Symbolic(const Factorizer& factorizer);
    explicit Symbolic(const TaylorExpander& taylorExpander);
    explicit Symbolic(const RootsFinder& rootsFinder);
    explicit Symbolic(const LimitFinder& limitFinder);
    explicit Symbolic(const Solver& solver);

    bool is_simplifier(void) const;
    bool is_differentiator(void) const;
    bool is_integrator(void) const;
    bool is_expander(void) const;
    bool is_factorizer(void) const;
    bool is_taylor(void) const;
    bool is_roots_finder(void) const;
    bool is_limit_finder(void) const;
    bool is_solver(void) const;

    Simplifier as_simplifier(void) const;
    Differentiator as_differentiator(void) const;
    Integrator as_integrator(void) const;
    Expander as_expander(void) const;
    Factorizer as_factorizer(void) const;
    TaylorExpander as_taylor_expander(void) const;
    RootsFinder as_root_finder(void) const;
    LimitFinder as_limit_finder(void) const;
    Solver as_solver(void) const;

    Expr simplify(void) const;
    Expr expand(void) const;
    Expr factor(void) const;
    Expr diff(void) const;
    Expr integral(void) const;
    Expr taylor(void) const;
    Expr limit(void) const;
    Vec<Expr> roots(void) const;
    Vec<Expr> solve(void) const;
    //bool is_(void) const;

private:
    using Type = std::variant<
        std::monostate,
        Simplifier,
        Differentiator,
        Integrator,
        Expander,
        Factorizer,
        TaylorExpander,
        RootsFinder,
        LimitFinder,
        Solver>;
    
    // -
    Type m_type;
};

// -*-
class Commands final: public ExecuteVisitor{
public:
    explicit Commands(const Symbolic& symbolic);

    void execute(const LetStmt& stmt, Env& ctx) const override;
    void execute(const FunStmt& stmt, Env& ctx) const override;

    Expr equation(const Expr&);
    Expr equation(Expr&&);
    Expr system(const Expr&);
    Expr system(Expr&&);

    Expr simplify(const Expr&);
    Expr simplify(Expr&&);

    Expr expand(const Expr&);
    Expr expand(Expr&&);

    Expr factor(const Expr&);
    Expr factor(Expr&&);

    Expr diff(const Expr&);
    Expr diff(Expr&&);

    Expr integral(const Expr&);
    Expr integral(Expr&&);

    Expr integrate(const Expr&);
    Expr integrate(Expr&&);

    Expr taylor(const Expr&);
    Expr taylor(Expr&&);

    Expr limit(const Expr&);
    Expr limit(Expr&&);

    Expr roots(const Expr&);
    Expr roots(Expr&&);

    Expr solve(const Expr&);
    Expr solve(Expr&&);

    Expr fibonacci(const Expr&);
    Expr fibonacci(Expr&&);

    Expr factorial(const Expr&);
    Expr factorial(Expr&&);

    Expr modulo(const Expr&);
    Expr modulo(Expr&&);

    Expr prime(const Expr&);
    Expr prime(Expr&&);

    Expr help(const Expr&);
    Expr help(Expr&&);

    Expr show(const Expr&);
    Expr show(Expr&&);

    Expr plot(const Expr&);
    Expr plot(Expr&&);

    Expr config(const Expr&);
    Expr config(Expr&&);

private:
    Symbolic m_symbolic;
};


// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola::core                                 -*-
// -*----------------------------------------------------------------*-