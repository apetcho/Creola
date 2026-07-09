#include "parser.hpp"
#include<cctype>

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola                                     -*-
// -*----------------------------------------------------------------*-
namespace creola::lexer{
//
Tokenizer::Tokenizer(std::string&& src)
: m_src{std::move(src)}, m_pos{0}
{}

Token Tokenizer::next(void){
    //! @todo
}

void Tokenizer::skip_whitespace(void){
    while(this->m_pos < this->m_src.size()){
        auto c = this->m_src[this->m_pos++];
        if(std::isspace(c)){ break; }
    }
}

// ---
Parser::Parser(std::string&& src)
: m_tokenizer{std::move(src)}
{
    //! @todo
}
core::Expr Parser::parse(void){
    //! @todo
}
void Parser::consume(TokenKind kind){
    //! @todo
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
class Tokenizer final {
public:
~Tokenizer() = default;

private:
    std::string m_src;
    size_t m_pos;
};

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