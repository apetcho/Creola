#include "parser.hpp"
#include<cctype>
#include<stdexcept>


// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola                                     -*-
// -*----------------------------------------------------------------*-
namespace creola::lexer{
//
Tokenizer::Tokenizer(std::string&& src)
: m_src{std::move(src)}, m_pos{0}
{}

Token Tokenizer::next(void){
    this->skip_whitespace();
    if(this->m_pos == this->m_src.size()){
        return Token(TokenKind::End, "");
    }

    auto c = this->m_src[this->m_pos];
    if(std::isdigit(c)){
        auto start = this->m_pos;
        size_t& pos = this->m_pos;
        while(pos < this->m_src.size() && (std::isdigit(this->m_src[pos]) || this->m_src[pos]=='.')){
            ++pos;
        }
        auto lexme = this->m_src.substr(start, pos-start);
        auto num = std::stod(lexme);
        return Token(TokenKind::Number, lexme, num);
    }
    if(std::isalpha(c) || c == '_'){
        auto start = this->m_pos;
        size_t& pos = this->m_pos;
        while(pos < this->m_src.size() && (std::isalnum(this->m_src[pos]) || this->m_src[0]=='_')){
            ++pos;
        }
        auto lexeme = this->m_src.substr(start, pos-start);
        if(lexeme=="let"){
            return Token(TokenKind::KwLet, "let");
        }
        if(lexeme=="fun"){
            return Token(TokenKind::KwFun, "fun");
        }
        return Token(TokenKind::Ident, lexeme);
    }
    ++this->m_pos;
    switch(c){
    case '+': return Token(TokenKind::Plus, "+");
    case '-': return Token(TokenKind::Minus, "-");
    case '*': return Token(TokenKind::Star, "*");
    case '/': return Token(TokenKind::Slash, "/");
    case '^': return Token(TokenKind::Caret, "^");
    case '(': return Token(TokenKind::LParen, "(");
    case ')': return Token(TokenKind::RParen, ")");
    case ',': return Token(TokenKind::Comma, ",");
    case '=': return Token(TokenKind::Equal, "=");
    }

    return Token(TokenKind::End, "");
}

void Tokenizer::skip_whitespace(void){
    while(this->m_pos < this->m_src.size()){
        auto c = this->m_src[this->m_pos++];
        if(std::isspace(c)){ break; }
    }
}

// ---
Parser::Parser(std::string&& src)
: m_tokenizer{std::move(src)}{
    this->m_curTok = this->m_tokenizer.next();
}

core::Expr Parser::parse(void){
    //! @todo
}


void Parser::consume(TokenKind kind){
    if(this->m_curTok.kind != kind){
        throw std::runtime_error("unexpected token");
    }
    this->m_curTok = this->m_tokenizer.next();
}

core::Expr Parser::parse_primary(void){
    //! @todo
}
core::Expr Parser::parse_pow(void){
    //! @todo
}
core::Expr Parser::parse_term(void){
    //! @todo
}

/*
// -*-
class Parser{
public:
private:
Tokenizer m_tokenizer;
Token m_curTok;


};
*/

// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola                                       -*-
// -*----------------------------------------------------------------*-