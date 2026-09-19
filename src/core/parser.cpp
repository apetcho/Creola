#include "ast.hpp"
#include "parser.hpp"

#include<functional>
#include<stdexcept>
#include<sstream>
#include<iomanip>
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

// -*-
[[maybe_unused]] Token Parser::consume(TokenKind kind, const std::string& msg){
    if(this->current().kind==TokenKind::End){
        throw std::runtime_error("Unexpected end of input");
    }
    if(!this->match(kind)){
        std::stringstream stream;
        stream << msg << " got " << std::quoted(this->current().lexeme);
        throw std::runtime_error(stream.str());
    }
    auto token = this->current();
    this->advance();
    return token;
}

// -*-
Stmt Parser::parse(void){
    return this->parseStatement();
}

// -*-
Stmt Parser::parseStatement(void){
    if(this->match(TokenKind::Let)){
        return this->parseLet();
    }
    if(this->match(TokenKind::Fun)){
        return this->parseFun();
    }

    throw std::runtime_error("Expected 'let' or 'fun'.");
}

// -*-
Stmt Parser::parseLet(void){
    this->consume(TokenKind::Let, "Expected 'let'");
    auto name = this->consume(TokenKind::Ident, "Exepected variable name").lexeme;
    if(name=="let"){
        std::stringstream stream;
        stream << "'let' is a reserved word. Variable name cannot be 'let'";
        throw std::runtime_error(stream.str());
    }
    if(name=="fun"){
        std::stringstream stream;
        stream << "'fun' is a reserved word. Variable name cannot be 'fun'";
        throw std::runtime_error(stream.str());
    }

    auto expr = this->parseExpression();
    return makeLet(name, std::move(expr));
}

/*
class Parser final {
public:

private:
    std::vector<Token> m_tokens;
    std::size_t m_cur;

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