#include "tokenizer.hpp"

#include<cctype>


// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola                                     -*-
// -*----------------------------------------------------------------*-
namespace creola{
// -

// -*-
Tokenizer::Tokenizer(const std::string& source)
: m_src{source}
, m_pos{0}
{}

/*
class Tokenizer final {
public:


std::vector<Token> Tokenizer::tokenize(void){}

private:
    std::string m_src;
    std::size_t m_pos;

char Tokenizer::peek(void) const{}
char Tokenizer::advance(void){}
void Tokenizer::skipWhitespace(void){}
Token Tokenizer::scanNumber(void){}
Token Tokenizer::scanIdent(void){}

};

*/

// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola                                       -*-
// -*----------------------------------------------------------------*-