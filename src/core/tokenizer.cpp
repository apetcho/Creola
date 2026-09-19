#include "tokenizer.hpp"

#include<cctype>
#include<sstream>

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

// -
void Tokenizer::advance(void){
    this->m_pos += 1;
}

// -*-
void Tokenizer::skipWhitespace(void){
    while(!this->is_at_end() && std::isspace(this->peek())){
        this->advance();
    }
}

// -
bool Tokenizer::is_at_end(void) const{
    return this->peek()==EOF ? true : false;
}

// -
bool Tokenizer::is_eof(char c) const{
    return (c==EOF) ? true : false;
}

// -
char Tokenizer::next_char(void) const{
    auto pos = this->m_pos + 1;
    if(pos >= this->m_src.size()){
        return EOF;
    }

    return this->m_src[pos];
}

// -*-
Token Tokenizer::scanNumber(void){
    auto pos = this->m_pos;
    while(!this->is_at_end() && std::isdigit(this->peek())){
        this->advance();
    }
    if(this->peek()=='.' && std::isdigit(this->next_char())){
        this->advance(); // consume '.'
        while(!this->is_at_end() && std::isdigit(this->peek())){
            this->advance();
        }
    }
    auto len = this->m_pos - pos;
    return Token{TokenKind::Num, this->m_src.substr(pos, len)};
}

// -*-
Token Tokenizer::scanIdent(void){
    auto pos = this->m_pos;
    while(!this->is_at_end() && (std::isalnum(this->peek()) || this->peek()=='_')){
        this->advance();
    }
    auto len = this->m_pos - pos;
    auto lexeme = this->m_src.substr(pos, len);
    if(lexeme=="let"){
        return Token{TokenKind::Let, lexeme};
    }
    if(lexeme=="fun"){
        return Token{TokenKind::Fun, lexeme};
    }

    return Token{TokenKind::Ident, lexeme};
}

// -*-
std::vector<Token> Tokenizer::tokenize(void){
    std::vector<Token> result{};
    while(!this->is_at_end()){
        this->skipWhitespace();
        auto c = this->peek();

        // Numbers
        if(std::isdigit(c)){
            result.push_back(this->scanNumber());
            continue;
        }
        if(std::isalpha(c) || c=='_'){
            result.push_back(this->scanIdent());
            continue;
        }

        // Operators
        if(c=='+'){
            result.push_back(Token{TokenKind::Plus, "+"});
            continue;
        }
        if(c=='-'){
            result.push_back(Token{TokenKind::Minus, "-"});
            continue;
        }
        if(c=='*'){
            result.push_back(Token{TokenKind::Star, "*"});
            continue;
        }
        if(c=='/'){
            result.push_back(Token{TokenKind::Slash, "/"});
            continue;
        }
        if(c=='^'){
            result.push_back(Token{TokenKind::Caret, "^"});
            continue;
        }
        if(c=='('){
            result.push_back(Token{TokenKind::LParen, "("});
            continue;
        }
        if(c==')'){
            result.push_back(Token{TokenKind::RParen, ")"});
            continue;
        }
        if(c=='{'){
            result.push_back(Token{TokenKind::LBrace, "{"});
            continue;
        }
        if(c=='}'){
            result.push_back(Token{TokenKind::RBrace, "}"});
            continue;
        }
        if(c==','){
            result.push_back(Token{TokenKind::Comma, ","});
            continue;
        }
        if(c=='='){
            result.push_back(Token{TokenKind::Equal, "="});
            continue;
        }

        // All acceptable characters or token have been scanned. If we're here,
        // then we probably encountered an invalid character

        std::stringstream stream;
        stream << "Invalid character '" << c << "' found in the input text";
        throw std::runtime_error(stream.str());
    }
    result.push_back(Token{TokenKind::End, ""});
    return result;
}

// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola                                       -*-
// -*----------------------------------------------------------------*-