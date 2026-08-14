#pragma once

#include<unordered_map>
#include<functional>
#include<cstdint>
#include<memory>
#include<vector>
#include<cctype>
#include<map>

#define CREOLA_UNUSED(arg)  (void)arg

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola::core                               -*-
// -*----------------------------------------------------------------*-
namespace creola::core {
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
using u32 = std::uint32_t;
using i32 = std::int32_t;
using u64 = std::uint32_t;
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

// -*-
struct FunctionDef {
    std::string param; // univariate function
    Expr body;
};

// -*-
enum class ExprKind {
    NUM, SYM, ADD, MUL, POW, NEG, CALL,
};


// -*-
static inline bool starts_with(const std::string& text, const std::string& prefix){
    if(text.length() < prefix.length()){ return false; }
    if(prefix == text.substr(0, prefix.length())){
        return true;
    }
    return false;
}

static inline bool ends_with(const std::string& text, const std::string& suffix){
    if(suffix.length() > text.length()){ return false; }
    auto start = text.length() - suffix.length();
    if(text.substr(start, std::string::npos)==suffix){
        return true;
    }
    return false;
}

static inline std::string ltrim(const std::string& text){
    auto ptr = text.begin();
    while(ptr != text.end()){
        if(!std::isspace(*ptr)){ break; }
        ptr++;
    }
    return std::string(ptr, text.end());
}

static inline std::string rtrim(const std::string& text){
    auto ptr = text.rbegin();
    while(ptr != text.rend()){
        if(!std::isspace(*ptr)){ break; }
        ptr++;
    }
    return std::string(text.rend(), ptr);
}

static inline std::string trim(const std::string& text){
    return ltrim(rtrim(text));
}

// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola::core                                 -*-
// -*----------------------------------------------------------------*-