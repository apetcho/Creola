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

    Token(): kind(TokenKind::End), text{""}, num{std::numeric_limits<f64>::max()}{}

    Token(TokenKind kind, const std::string& lexme, f64 val=std::numeric_limits<f64>::max())
    : kind{kind}, text{lexme}, num{val}
    {}
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
    Parser(): m_tokenizer{""}, m_curTok{Token(TokenKind::End, "")}
    {}
    explicit Parser(std::string&& src);
    Expr parse(void); // parse_expr

private:
    Tokenizer m_tokenizer;
    Token m_curTok;
    
    void consume(TokenKind kind);
    void expect(TokenKind kind);
    bool match(TokenKind kind){
        return this->m_curTok.kind==kind;
    }
    Expr parse_primary(void);
    Expr parse_pow(void);
    Expr parse_term(void);
};


// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola                                       -*-
// -*----------------------------------------------------------------*-