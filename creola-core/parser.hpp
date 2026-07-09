#pragma once

#include<string>
#include "core-expr.hpp"

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola                                     -*-
// -*----------------------------------------------------------------*-
namespace creola::lexer{
//

enum class TokenKind{
    End, Number, Ident,
    Plus, Minus, Star, Slash, Caret,
    LParen, RParen, Comma, Equal,
    KwLet, KwFun,
};

// -*-
struct Token{
    TokenKind kind;
    std::string text;
    long double num;
};

// -*-
class Tokenizer final {
public:
    explicit Tokenizer(std::string&& src);

    ~Tokenizer() = default;
    Token next(void);
private:
    std::string m_src;
    size_t m_pos;

    void skip_whitespace(void);
};

// -*-
class Parser{
public:
    explicit Parser(std::string&& src);
    core::Expr parse(void); // parse_expr
    void consume(TokenKind kind);

private:
    Tokenizer m_tokenizer;
    Token m_curTok;
    
    core::Expr parse_primary(void);
    core::Expr parse_pow(void);
    core::Expr parse_term(void);
};


// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola                                       -*-
// -*----------------------------------------------------------------*-