#pragma once

#include<unordered_map>
#include<functional>
#include<stdexcept>
#include<cstdint>
#include<memory>
#include<vector>
#include<cctype>
#include<cmath>
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

class Result{
public:
    explicit Result(std::string& message, bool ok=true)
    : m_ok{ok}, m_value{message}
    {}

    bool is_ok(void)const { return this->m_ok; }
    std::string value(void) const {
        return this->m_okd ? this->m_value.ok : this->m_value.err;
    }

private:
    union Value{
        std::string ok;
        std::string err;
    };
    bool m_ok;
    Value m_value;
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

// -*-
class CreolaError: public std::runtime_error{
public:
    CreolaError(): std::runtime_error("unexpected error found."){}
    explicit CreolaError(const char* msg): std::runtime_error(msg){}
    explicit CreolaError(const std::string& msg): std::runtime_error(msg){}
};

// some utility functions
static inline bool almost_equal(f64 xnum, f64 ynum, f64 tol=1e-12){
    if(!std::isfinite(xnum) || !std::isfinite(ynum)){
        return false;
    }

    constexpr auto VMIN = std::numeric_limits<f64>::min();

    auto dx = std::abs(xnum-ynum);
    if(dx <= VMIN){ return true; }

    auto x = std::abs(xnum);
    auto y = std::abs(ynum);
    return (dx/std::max(x, y) <= tol);
}

static inline bool almost_zero(f64 num, f64 tol=1e-12){
    return almost_equal(num, 0.0, tol);
}


static inline Vec<i64> divisors(i64 num){
    //! @todo
    return {};
}

// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola::core                                 -*-
// -*----------------------------------------------------------------*-