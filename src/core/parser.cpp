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

// -*-
Stmt Parser::parseFun(void){
    this->consume(TokenKind::Fun, "Expected 'fun'");
    auto name = this->consume(TokenKind::Ident, "Expected function name").lexeme;
    if(name=="let"){
        std::stringstream stream;
        stream << "'let' is a reserved word. Function name cannot be 'let'";
        throw std::runtime_error(stream.str());
    }
    if(name=="fun"){
        std::stringstream stream;
        stream << "'fun' is a reserved word. Function name cannot be 'fun'";
        throw std::runtime_error(stream.str());
    }

    this->consume(TokenKind::LParen, "Expected '('");
    std::vector<std::string> params{};
    if(!this->match(TokenKind::RParen)){
        params.push_back(this->consume(TokenKind::Ident, "Expected parameter").lexeme);
        while(this->match(TokenKind::Comma)){
            this->consume(TokenKind::Comma, "Expected ','");
            if(this->match(TokenKind::RParen) || this->current().kind==TokenKind::End){
                break;
            }
            params.push_back(this->consume(TokenKind::Ident, "Expected parameter").lexeme);
        }
    }
    if(!this->match(TokenKind::RParen)){
        throw std::runtime_error("Malformed function statement. Expected ')'");
    }
    this->consume(TokenKind::RParen, "Expected ')'");
    this->consume(TokenKind::Equal, "Expected '='");
    Expr body = this->parseExpression();
    Lambda lambda(params, std::move(body));

    return makeFun(name, lambda);
}

// -*-
Expr Parser::parseExpression(void){
    return this->parseAddSub();
}

// -*-
Expr Parser::parseAddSub(void){
    Expr lhs = this->parseMulDiv();
    while(this->match(TokenKind::Plus) || this->match(TokenKind::Minus)){
        auto op = this->current().kind;
        this->consume(op, "Expected '+' or '-'");
        auto rhs = this->parseMulDiv();
        if(op==TokenKind::Plus){
            lhs = makeBinary('+', std::move(lhs), std::move(rhs));
        }else{
            lhs = makeBinary('-', std::move(lhs), std::move(rhs));
        }
    }

    return lhs;
}

// -*-
Expr Parser::parseMulDiv(void){
    Expr lhs = this->parsePow();
    while(this->match(TokenKind::Star) || this->match(TokenKind::Slash)){
        auto op = this->current().kind;
        this->consume(op, "Expected '/' or '*'");
        auto rhs = this->parseMulDiv();
        if(op==TokenKind::Slash){
            lhs = makeBinary('/', std::move(lhs), std::move(rhs));
        }else{
            lhs = makeBinary('*', std::move(lhs), std::move(rhs));
        }
    }

    return lhs;
}

// -*-
Expr Parser::parsePow(void){
    Expr base = this->parseUnary();
    if(this->match(TokenKind::Caret)){
        this->consume(TokenKind::Caret, "Expected '^'");
        Expr expo = this->parseUnary();
        return makeBinary('^', std::move(base), std::move(expo));
    }

    return base;
}

/*
class Parser final {
public:
private:
    std::vector<Token> m_tokens;
    std::size_t m_cur;

Expr Parser::parseUnary(void){}
Expr Parser::parsePrimary(void){}
Expr Parser::parseCall(void){}
Expr Parser::parseLambda(void){}
};
*/

// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola                                       -*-
// -*----------------------------------------------------------------*-