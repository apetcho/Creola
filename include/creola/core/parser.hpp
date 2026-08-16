#pragma once

#include<string>
#include "creola/core/expr.hpp"

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola::core                               -*-
// -*----------------------------------------------------------------*-
namespace creola::core{
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
    f64 num;

    Token(): kind(TokenKind::End), text{""}, num{std::numeric_limits<f64>::max()}{}

    Token(TokenKind kind, char lexme, f64 val=std::numeric_limits<f64>::max())
    : kind{kind}, text{lexme}, num{val}
    {}

    Token(TokenKind kind, const std::string& lexme, f64 val=std::numeric_limits<f64>::max())
    : kind{kind}, text{lexme}, num{val}
    {}
};

// -*-
class Tokenizer final {
public:
    explicit Tokenizer(const std::string& src);

    ~Tokenizer() = default;
    Token next(void);
private:
    std::string m_src;
    size_t m_pos;

    void skip_whitespace(void);

    void advance(int pos=1){
        this->m_pos += pos;
    }
    
    char peek(int pos=0){
        if(this->m_pos + pos >= this->m_src.length()){ return EOF; }
        auto idx = this->m_pos + pos;
        return this->m_src[idx];
    }

    Token read_symbol(void);
    Token read_number(void);
    Token match_symbol(const std::string& text);
};

// -*-
class Parser{
public:
    Parser(): m_tokenizer{""}, m_curTok{Token(TokenKind::End, "")}
    {}
    explicit Parser(const std::string& src);
    Expr parse(void); // parse_expr
    const Token& current(void) const { return this->m_curTok; }

    bool match(TokenKind kind){
        return this->m_curTok.kind==kind;
    }

    void consume(TokenKind kind);
    void consume(TokenKind kind, const char* msg){
        this->expect(kind, msg);
        this->consume(kind);
    }
    void expect(TokenKind kind, const char* msg);

private:
    Tokenizer m_tokenizer;
    Token m_curTok;
    
    Expr parse_primary(void);
    Expr parse_pow(void);
    Expr parse_term(void);
};


// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola::core                                 -*-
// -*----------------------------------------------------------------*-