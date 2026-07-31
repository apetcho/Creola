#pragma once
#include<memory>
#include<vector>
#include<cstdint>
#include<functional>
#include<unordered_map>
#include<map>

#define CREOLA_USE_READLINE
#ifdef CREOLA_USE_READLINE
#include<readline/readline.h>
#include<readline/history.h>
#endif

#define CREOLA_UNUSED(arg)  (void)arg

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola                                     -*-
// -*----------------------------------------------------------------*-
namespace creola {
// -

template<typename T>
using Vec = std::vector<T>;

template<typename T>
using Shared = std::shared_ptr<T>;

template<typename Key, typename Value>
using HashMap = std::unordered_map<Key, Value>;

template<typename Key, typename Value>
using Dict = std::map<Key, Value>;

// -*-
struct ExprBase;
using Expr = Shared<ExprBase>;

using f64 = double;
using i64 = std::int64_t;
using usize = size_t;

// -*- Common Math functions mapping
enum class MathFunc{
    // Trigonometric functions
    Sin, Cos, Tan, Asin, Acos, Atan,
    // Hyperbolic functions
    Sinh, Cosh, Tanh, Asinh, Acosh, Atanh,
    // Power functions
    Pow, Sqrt, Cbrt, Exp,
    // ...
};
// -
using UnaryMathFun = std::function<Expr(f64)>;
using Func = std::function<Expr(Expr, const std::string&)>;

// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola                                       -*-
// -*----------------------------------------------------------------*-