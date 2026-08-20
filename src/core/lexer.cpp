#include "creola/core/lexer.hpp"

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola::core                               -*-
// -*----------------------------------------------------------------*-
namespace creola::core{
//

// -*-
Lexer::Lexer(const Str& src) : m_src{}
{}

// -*-
Token Lexer::next(void){
    this->skip_ws();
    if(this->is_eos()){ return Token(); }

    auto c = this->peek();
    // read number 
    if(std::isdigit(c) || c == '.'){ return this->read_number(); }

    // read string := "text" | 'text'
    if(c=='"' || c == '\''){ return this->read_string(); }

    // read identifier
    if(std::isalpha(c) || c == '_'){ return this->read_identifier(); }

    // -*-
    this->advance();
    switch(c){
        case '+': return Token(TokenKind::Plot, "+");
        case '-': return Token(TokenKind::Minus, "-");
        case '*': return Token(TokenKind::Star, "*");
        case '/': return Token(TokenKind::Slash, "/");
        case '^': return Token(TokenKind::Caret, "^");
        case '(': return Token(TokenKind::LParen, "(");
        case ')': return Token(TokenKind::RParen, ")");
        case ',': return Token(TokenKind::Comma, ",");
        case '=': return Token(TokenKind::Equal, "=");
        case ';': return Token(TokenKind::Semi, ";");
    }

    return Token();
}

// -*-
void Lexer::skip_ws(void){
    while(!this->is_eos() && std::isspace(this->peek())){
        this->advance();
    }
}

/*
// -*-
class Lexer{
public:

char Lexer::peek(void){}
Token Lexer::read_number(void){}
Token Lexer::read_string(void){}
Token Lexer::read_identifier(void){}

private:
    Str m_src;
    usize m_pos = 0;


};

*/

// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola::core                                 -*-
// -*----------------------------------------------------------------*-