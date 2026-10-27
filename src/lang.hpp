/**
 * @file lang.hpp
 * @author Eyram K. Apetcho (you@domain.com)
 * @brief Lexer + parser for Creola language. Produces an AST
 * @version 0.1
 * @date 2026-10-23
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#pragma once

#include<functional>
#include<memory>
#include<vector>
#include<string>


// ----------------------------------------------------------------------------
// begin::namespace::creola
// ----------------------------------------------------------------------------
namespace creola{
// -
struct Ast;
using AstPtr = std::shared_ptr<Ast>;

struct Ast{
    enum Kind {
        Num, Str, Ident, BinOp, Neg,
        Call, List, Index, Slice,
        Named, Let, Fun,
    };

    Kind kind{};
    double num{0.0};
    std::string text;                   // identifier / operator / string / callee
    std::vector<AstPtr> children;
    std::vector<std::string> params;    // Fun
};

std::vector<AstPtr> parse(const std::string& src, const std::function<bool(const std::string&)>& isCallable);

// ----------------------------------------------------------------------------
}// end::namespace::creola
// ----------------------------------------------------------------------------