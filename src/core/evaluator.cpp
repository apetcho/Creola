#include "creola/core/evaluator.hpp"
#include "creola/core/ast.hpp"
#include "creola/core/env.hpp"

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola::core                               -*-
// -*----------------------------------------------------------------*-
namespace creola::core{
// -

Evaluator::Evaluator(Env& ctx)
: m_ctx{ctx}
, m_rootsFinder{}
, m_limitFinder{}
, m_differentiator{}
, m_integrator{}
, m_simplifier{}
, m_expander{}
, m_factorizer{}
, m_taylorExpander{}
, m_solver{}
, m_helper{}
{}

Expr Evaluator::eval(const Expr& expr){
    //! @todo
    return nullptr;
}


Expr Evaluator::eval(Expr&& expr){
    //! @todo
    return nullptr;
}

// -------------------------
// -*-  V I S I T O R S  -*-
// -------------------------
Expr Evaluator::eval(const SymbolExpr& expr, Env& ctx) const{
    //! @todo
    return nullptr;
}


Expr Evaluator::eval(const NumberExpr& expr, Env& ctx) const{
    //! @todo
    return nullptr;
}

Expr Evaluator::eval(const NegExpr& expr, Env& ctx) const {
    //! @todo
    return nullptr;
}

Expr Evaluator::eval(const AddExpr& expr, Env& ctx) const{
    //! @todo
    return nullptr;
}

Expr Evaluator::eval(const MulExpr& expr, Env& ctx) const{
    //! @todo
    return nullptr;
}

Expr Evaluator::eval(const PowExpr& expr, Env& ctx) const{
    //! @todo
    return nullptr;
}

Expr Evaluator::eval(const CallExpr& expr, Env& ctx) const{
    //! @todo
    return nullptr;
}


Expr Evaluator::eval(const EquationExpr& expr, Env& ctx) const {
    //! @todo
    return nullptr;
}

Expr Evaluator::eval(const SystemExpr& expr, Env& ctx) const {
    //! @todo
    return nullptr;
}

// -----------------------------------------------------------------------
// -*-                   C   O  M   M   A   N   D   S                  -*-
// -*- diff, factor, expand, integral, integrate, limit, roots, solve  -*-
// -*- simplify, taylor, equation, system, moduloa, config, show, help,-*-
// -*- show, fibonacci, factorial, prime, gcd, lcm, plot,              -*-
// -----------------------------------------------------------------------
Expr Evaluator::diff(Evaluator& evaluator, const Expr& expr, const Str& var){
    return evaluator.m_differentiator.diff(expr, var);
}


Expr Evaluator::simplify(Evaluator& evaluator, const Expr& expr){
    return evaluator.m_simplifier.simplify(expr);
}

Expr Evaluator::expand(Evaluator& evaluator, const Expr& expr){
    return evaluator.m_expander.expand(expr);
}


Expr Evaluator::factor(Evaluator& evaluator, const Expr& expr, const Str& var){
    return evaluator.m_factorizer.factor(expr, var);
}

Expr Evaluator::integral(Evaluator& evaluator, const Expr& expr, const Str& var){
    return evaluator.m_integrator.integral(expr, var);
}

Expr Evaluator::integrate(Evaluator& evaluator, const Expr& expr, const Str& var, f64 vmin, f64 vmax){
    auto integral = evaluator.m_integrator.integral(expr, var);
    Env& ctx = evaluator.m_ctx;
    Str name{"__f("};
    name += var + ")__";
    auto lambda = make_lambda_expr(Vec<Str>{var}, integral);
    Env env(&evaluator.m_ctx);
    env.define(name, lambda);
    auto caller = make_call_expr(name, Vec<Expr>{make_number_expr(vmin)});
    evaluator.m_ctx = env;
    auto f1 = evaluator.eval(caller);
    caller = make_call_expr(name, Vec<Expr>{make_number_expr(vmax)});
    auto f2 = evaluator.eval(caller);
    auto result =  make_add_expr(f2, make_neg_expr(f1));
    result = Evaluator::simplify(evaluator, result);
    evaluator.m_ctx = ctx;
    return result;
}


Expr Evaluator::taylor(Evaluator& evaluator, const Expr& expr, const Str& var, f64 center, u32 order){
    return evaluator.m_taylorExpander.taylor(expr, var, center, order);
}


Expr Evaluator::limit(Evaluator& evaluator, const Expr& expr, const Str& var, f64 loc){
    return evaluator.m_limitFinder.limit(expr, var, loc);
}


Expr Evaluator::roots(Evaluator& evaluator, const Expr& expr, const Str& var){
    return evaluator.m_rootsFinder.roots(expr, var);
}


Expr Evaluator::roots(Evaluator& evaluator, const Expr& expr, const Str& var, f64 tolerance){
    return evaluator.m_rootsFinder.roots(expr, var, tolerance);
}


Expr Evaluator::solve(Evaluator& evaluator, const EquationExpr& equation){
    return evaluator.m_solver.solve(equation);
}


Expr Evaluator::solve(Evaluator& evaluator, const EquationExpr& equation, f64 tolerance){
    return evaluator.m_solver.solve(equation, tolerance);
}


Expr Evaluator::solve(Evaluator& evaluator, const SystemExpr& system){
    return evaluator.m_solver.solve(system);
}

Expr Evaluator::solve(Evaluator& evaluator, const SystemExpr& system, f64 tolerance){
    return evaluator.m_solver.solve(system, tolerance);
}

// -*-
static Dict<u32, i64> fibMemo = {
    {0,  0},      {1,  1},      {2,  1},      {3,  2},      {4,  3},
    {5,  5},      {6, 8},       {7,  13},     {8,  21},     {9,  34},
    {10, 55},     {11, 89},     {12, 144},    {13, 233},    {14, 377},
    {15, 610},    {15, 987},    {17, 1597},   {18, 2584},   {19, 4181},
    {20, 6765},   {21, 10946},  {22, 17711},  {23, 28657},  {24, 46368},
    {25, 75025},  {26, 121393}, {27, 196418}, {28, 317811}, {29, 514229},
    {30, 832040}, {31, 1346269}, {32, 2178309},
};

// -*-
static i64 fibonacci_helper(u32 idx){
    static Vec<i64> memo = {
        0,             1,             1,             2,
        3,             5,             8,             13,
        21,            34,            55,            89,
        144,           233,           377,           610,
        987,           1597,          2584,          4181,
        6765,          10946,         17711,         28657,
        46368,         75025,         121393,        196418,
        317811,        514229,        832040,        1346269,
        2178309,       3524578,       5702887,       9227465,
        14930352,      24157817,      39088169,      63245986,
        102334155,     165580141,     267914296,     433494437,
        701408733,     1134903170,    1836311903,    2971215073,
        4807526976,    7778742049,    12586269025,   20365011074,
        32951280099,   53316291173,   86267571272,   139583862445,
        225851433717,  365435296162,  591286729879,  956722026041,
        1548008755920, 2504730781961, 4052739537881, 6557470319842
    };

    if (idx < memo.size()) {
        return memo[idx];
    }

    // Compute missing values up to n
    for (size_t i = memo.size(); i <= idx; ++i) {
        memo.push_back(memo[i - 1] + memo[i - 2]);
    }

    return memo[idx];
}


// -*-
Expr Evaluator::fibonacci(Evaluator& evaluator, u32 idx){
    auto num = fibonacci_helper(idx);
    return make_number_expr(num);
}

Expr Evaluator::factorial(Evaluator& evaluator, u32 idx){
    //! @todo
    return nullptr;
}

Expr Evaluator::modulo(Evaluator& evaluator, const NumberExpr& lhs, const NumberExpr& rhs){
    //! @todo
    return nullptr;
}

Expr Evaluator::prime(Evaluator& evaluator, u32 idx){
    //! @todo
    return nullptr;
}

Expr Evaluator::gcd(Evaluator& evaluator, i64 x, i64 y){
    //! @todo
    return nullptr;
}

Expr Evaluator::lcm(Evaluator& evaluator, i64 x, i64 y){
    //! @todo
    return nullptr;
}

Expr Evaluator::help(Evaluator& evaluator, const Str& cmd){
    //! @todo
    return nullptr;
}

Expr Evaluator::show(Evaluator& evaluator, const Str& query){
    //! @todo
    return nullptr;
}


Expr Evaluator::plot(Evaluator& evaluator, const Vec<Expr>& args){
    //! @todo
    return nullptr;
}

Expr Evaluator::config(Evaluator& evaluator, const Str& query, const Expr& expr){
    //! @todo
    return nullptr;
}

Expr Evaluator::equation(Evaluator& evaluator, const Expr& lhs, const Expr& rhs, const Str& var){
    //! @todo
    return nullptr;
}

Expr Evaluator::system(Evaluator& evaluator, const Vec<Expr>& lhs, const Vec<Expr>& rhs, const Vec<Str>& vars){
    //! @todo
    return nullptr;
}

// -----------------------------------
// -*- H E L P   C A L L B A C K S -*-
// -----------------------------------
Str Helper::help_diff(void){
    //! @todo
    return "";
}

Str Helper::help_simplify(void){
    //! @todo
    return "";
}

Str Helper::help_expand(void){
    //! @todo
    return "";
}

Str Helper::help_factor(void){
    //! @todo
    return "";
}

Str Helper::help_integral(void){
    //! @todo
    return "";
}

Str Helper::help_integrate(void){
    //! @todo
    return "";
}

Str Helper::help_taylor(void){
    //! @todo
    return "";
}

Str Helper::help_limit(void){
    //! @todo
    return "";
}

Str Helper::help_roots(void){
    //! @todo
    return "";
}

Str Helper::help_solve(void){
    //! @todo
    return "";
}

Str Helper::help_fibonacci(void){
    //! @todo
    return "";
}

Str Helper::help_factorial(void){
    //! @todo
    return "";
}

Str Helper::help_modulo(void){
    //! @todo
    return "";
}

Str Helper::help_prime(void){
    //! @todo
    return "";
}

Str Helper::help_gcd(void){
    //! @todo
    return "";
}

Str Helper::help_lcm(void){
    //! @todo
    return "";
}

Str Helper::help_help(void){
    //! @todo
    return "";
}

Str Helper::help_show(void){
    //! @todo
    return "";
}

Str Helper::help_plot(void){
    //! @todo
    return "";
}

Str Helper::help_config(void){
    //! @todo
    return "";
}

Str Helper::help_equation(void){
    //! @todo
    return "";
}

Str Helper::help_system(void){
    //! @todo
    return "";
}

/*
// -*-
class Evaluator final: protected EvalVisitor {
public:


// -*-

private:
    Env& m_ctx;

};

struct Helper{
private:



};

*/

// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola::core                                 -*-
// -*----------------------------------------------------------------*-