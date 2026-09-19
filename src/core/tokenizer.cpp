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

// -*-
char Tokenizer::peek(void) const{
    if(this->m_pos >= this->m_src.size()){
        return EOF;
    }
    return this->m_src[this->m_pos];
}


/*
class Tokenizer final {
public:

// -*-
std::vector<Token> Tokenizer::tokenize(void){
    std::vector<Token> result{};

    std::size_t i = 0;
    while(i < this->m_src.size()){
        //! @todo
    }
}

private:
    std::string m_src;
    std::size_t m_pos;


char Tokenizer::advance(void){}
void Tokenizer::skipWhitespace(void){}
Token Tokenizer::scanNumber(void){}
Token Tokenizer::scanIdent(void){}
bool Tokenizer::is_at_end(void) const{}
bool Tokenizer::is_eof(char c) const{}
char Tokenizer::next_char(void) const{}

};

*/

// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola                                       -*-
// -*----------------------------------------------------------------*-