#pragma once

#include<string>
#include<vector>


// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola                                     -*-
// -*----------------------------------------------------------------*-
namespace creola{
// -

enum class TokenKind {
    Num, Ident, Let, Fun,
    Eq, Plus, Minus, Star, Slash, Caret,
    LParen, RParen, LBrace, RBrace, Comma,
    End
};

// -
struct Token final {
    TokenKind kind;
    std::string lexeme;
};

// -
class Tokenizer final {
public:
    Tokenizer(const std::string& source);
    std::vector<Token> tokenize(void);

private:
    std::string m_src;
    std::size_t m_pos;

    char peek(void) const;
    void advance(void);
    void skipWhitespace(void);
    Token scanNumber(void);
    Token scanIdent(void);
    bool is_at_end(void) const;
    bool is_eof(char c) const;
    char next_char(void) const;
};

// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola                                       -*-
// -*----------------------------------------------------------------*-