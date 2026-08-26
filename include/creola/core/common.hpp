#pragma once

#include<unordered_map>
#include<functional>
#include<stdexcept>
#include<exception>
#include<iostream>
#include<sstream>
#include<optional>
#include<variant>
#include<cstdint>
#include<memory>
#include<vector>
#include<cctype>
#include<string>
#include<cmath>
#include<map>

#define CREOLA_UNUSED(arg)  (void)arg

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola::core                               -*-
// -*----------------------------------------------------------------*-
namespace creola::core {
// -
// -*- Forward declarations -*-
struct AstBase;
struct ExprBase;
struct StmtBase;

template<typename T>
using Vec = std::vector<T>;

template<typename T>
using Option = std::optional<T>;

template<typename T>
using Shared = std::shared_ptr<T>;

template<typename Key, typename Value>
using HashMap = std::unordered_map<Key, Value>;

template<typename Key, typename Value>
using Dict = std::map<Key, Value>;

// -*-
using Ast = Shared<AstBase>;
using Stmt = Shared<StmtBase>;
using Expr = Shared<ExprBase>;

using Str = std::string;

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

using UnaryFun = std::function<Expr(const Expr&)>;
using BinaryFun = std::function<Expr(const Expr&, const Expr&)>;
using MathFun = std::variant<UnaryFun, BinaryFun>;

// -*-
class CreolaError final {
public:
    CreolaError()
    : m_etype{"Error"}
    , m_msg{"unexpected error found."}
    {}

    explicit CreolaError(const char* msg)
    : m_etype{"Error"}
    , m_msg{msg}
    {}

    
    explicit CreolaError(const std::string& msg)
    : m_etype{"Error"}
    , m_msg{msg}
    {}

    explicit CreolaError(const char* etype, const char* msg)
    : m_etype{etype}
    , m_msg{msg}
    {}

    explicit CreolaError(const char* etype, const Str& msg)
    : m_etype{etype}
    , m_msg{msg}
    {}

    explicit CreolaError(const Str& etype, const char* msg)
    : CreolaError(msg){
        this->m_etype = etype;
    }

    explicit CreolaError(const Str& etype, const std::string& msg)
    : CreolaError(msg){
        this->m_etype = etype;
    }

    Str describe(void) const {
        std::ostringstream oss;
        oss << this->m_etype << ": " << this->m_msg;
        return oss.str();
    }

private:
    Str m_etype;
    Str m_msg;
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
    std::cerr << "In 'ltrim()'" << std::endl;
    auto ptr = text.begin();
    while(ptr != text.end()){
        if(!std::isspace(*ptr)){ break; }
        ptr++;
    }
    return std::string(ptr, text.end());
}

static inline std::string rtrim(const std::string& text){
    auto len = text.length();
    while(len > 0 && std::isspace(text[--len])){ continue; }
    return text.substr(0, len+1);
}

static inline std::string trim(const std::string& text){
    std::cerr << "In 'trim()'" << std::endl;
    return creola::core::ltrim(rtrim(text));
}


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
    return creola::core::almost_equal(num, 0.0, tol);
}


static inline Vec<i64> divisors(i64 num){
    //! @todo
    return {};
}

// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola::core                                 -*-
// -*----------------------------------------------------------------*-