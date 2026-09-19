#include "parser.hpp"

#include<functional>
#include<stdexcept>
#include<memory>
#include<cmath>


// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola                                     -*-
// -*----------------------------------------------------------------*-
namespace creola{
// -

// -*-
Parser::Parser(const std::string& src)
: m_tokens(Tokenizer(src).tokenize())
, m_cur{0}
{}

// -*-
bool Parser::match(TokenKind kind) const{
    return this->current().kind==kind;
}

// -*-
Token Parser::current(void) const{
    if(this->m_cur >= this->m_tokens.size()){
        return Token{TokenKind::End, ""};
    }
    return this->m_tokens[this->m_cur];
}

void Parser::advance(void){
    this->m_cur += 1;
}

/*
class Parser final {
public:


Stmt Parser::parse(void){}

private:
    std::vector<Token> m_tokens;
    std::size_t m_cur;


Token Parser::consume(TokenKind kind, const std::string& msg){}

Stmt Parser::parseStatement(void){}
Stmt Parser::parseLet(void){}
Stmt Parser::parseFun(void){}

Expr Parser::parseExpression(void){}
Expr Parser::parseAddSub(void){}
Expr Parser::parseMulDiv(void){}
Expr Parser::parsePow(void){}
Expr Parser::parseUnary(void){}
Expr Parser::parsePrimary(void){}
Expr Parser::parseCall(void){}
Expr Parser::parseLambda(void){}
};
*/

// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola                                       -*-
// -*----------------------------------------------------------------*-