#include "creola/core/evaluator.hpp"
#include "creola/core/ast.hpp"
#include "creola/core/env.hpp"

#include<cmath>
#include<numeric>

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

// -*-
static i64 factorial_helper(u32 n){
    static Vec<i64> memo = {
        1LL,
        1LL,                       2LL,
        6LL,                       24LL,
        120LL,                     720LL,
        5'040LL,                   40'320LL,
        362'880LL,                 3'628'800LL,
        39'916'800LL,              479'001'600LL,
        6'227'020'800LL,           87'178'291'200LL,
        1'307'674'368'000LL,       20'922'789'888'000LL,
        355'687'428'096'000LL,     6'402'373'705'728'000LL,
        121'645'100'408'832'000LL, 2'432'902'008'176'640'000LL,
    };

    // If the value is already computed, return it immediately
    if(n < memo.size()) {
        return memo[n];
    }

    // Compute and store new values up to n
    // Start from the last known value to avoid redundant multiplication
    auto result = memo.back();
    for(int i = memo.size(); i <= n; ++i) {
        result *= i;
        memo.push_back(result);
    }

    return result;
}

Expr Evaluator::factorial(Evaluator& evaluator, u32 n){
    // i64::MAX = 9223372036854775807;
    if(n > 20){
        throw CreolaError("factorial(" + std::to_string(n) + " is out of range.");
    }
    auto num = factorial_helper(n);
    return make_number_expr(num);
}

//
Expr Evaluator::modulo(Evaluator& evaluator, const NumberExpr& lhs, const NumberExpr& rhs){
    if(lhs.is_integer() && rhs.is_integer()){
        auto xnum = lhs.as_integer();
        auto ynum = rhs.as_integer();
        auto num = xnum % ynum;
        return make_number_expr(num);
    }
    auto xnum = lhs.as_float();
    auto ynum = rhs.as_float();
    auto num = std::fmod(xnum, ynum);
    return make_number_expr(num);
}

// -*-
class MemoizedPrimeFinder{
private:
    Vec<u32> m_primes;

    bool is_prime(u32 n){
        if(n <= 1){ return false; }
        if(n <= 3){ return true; }
        if(n % 2 == 0 || n % 3 == 0){
            return false;
        }

        // Check divisibility
        for(const auto& p: this->m_primes){
            if(p * p > n){ break; }
            if(n % p == 0){ return false; }
        }

        return true;
    }

public:
    MemoizedPrimeFinder(){
        this->m_primes = {
            2,   3,   5,   7,   11,  13,  17,  19,  23,  29,  31,  37,  41,
            43,  47,  53,  59,  61,  67,  71,  73,  79,  83,  89,  97,  101,
            103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167,
            173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239,
            241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313,
            317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397,
            401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 467,
            479, 487, 491, 499, 503, 509, 521, 523, 541, 547, 557, 563, 569,
            571, 577, 587, 593, 599, 601, 607, 613, 617, 619, 631, 641, 643,
            647, 653, 659, 661, 673, 677, 683, 691, 701, 709, 719
        };
    }


    i64 nth_prime(u32 nth){
        if(nth < this->m_primes.size()){
            return this->m_primes[nth];
        }

        auto n = this->m_primes[this->m_primes.size()-1];
        while(this->m_primes.size() < nth){
            n += 2;
            if(this->is_prime(n)){
                this->m_primes.push_back(n);
            }
        }
        auto result = this->m_primes[this->m_primes.size()-1];

        return result;
    }
};

// -*-
Expr Evaluator::prime(Evaluator& evaluator, u32 idx){
    MemoizedPrimeFinder primeFinder;
    auto num = primeFinder.nth_prime(idx);
    return make_number_expr(num);
}

// -*-
Expr Evaluator::gcd(Evaluator& evaluator, i64 x, i64 y){
    auto num = std::gcd(x, y);
    return make_number_expr(num);
}

Expr Evaluator::lcm(Evaluator& evaluator, i64 x, i64 y){
    auto num = std::lcm(x, y);
    return make_number_expr(num);
}

Expr Evaluator::help(Evaluator& evaluator, const Str& cmd){
    //! @todo
    throw CreolaError("`help()`: not implemented yet");
}

Expr Evaluator::show(Evaluator& evaluator, const Str& query){
    //! @todo
    throw CreolaError("`help()`: not implemented yet");
}


Expr Evaluator::plot(Evaluator& evaluator, const Vec<Expr>& args){
    //! @todo
    throw CreolaError("`plot()`: not implemented yet");
}

Expr Evaluator::config(Evaluator& evaluator, const Str& query, const Expr& expr){
    //! @todo
    throw CreolaError("`config()`: not implemented yet");
}

Expr Evaluator::equation(Evaluator& evaluator, const Expr& lhs, const Expr& rhs, const Str& var){
    //! @todo
    throw CreolaError("`equation()`: not implemented yet");
}

Expr Evaluator::system(Evaluator& evaluator, const Vec<Expr>& lhs, const Vec<Expr>& rhs, const Vec<Str>& vars){
    //! @todo
    throw CreolaError("`system()`: not implemented yet");
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