#include "creola/core/parser.hpp"
#include "creola/core/engine.hpp"

#include<cctype>
#include<stdexcept>


// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola::core                               -*-
// -*----------------------------------------------------------------*-
namespace creola::core{
//
Tokenizer::Tokenizer(const std::string& src)
: m_src{src}, m_pos{0}
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
Parser::Parser(const std::string& src)
: m_tokenizer{src}{
    this->m_curTok = this->m_tokenizer.next();
}

Expr Parser::parse(void){
    auto lhs = this->parse_term();
    while(this->match(TokenKind::Plus) || this->match(TokenKind::Minus)){
        auto op = this->m_curTok.kind;
        this->consume(op);
        auto rhs = this->parse_term();
        if(op == TokenKind::Plus){
            lhs = std::make_shared<Add>(Vec<Expr>{lhs, rhs});
        }else{
            lhs = std::make_shared<Add>(Vec<Expr>{
                lhs,
                std::make_shared<Neg>(rhs)
            });
        }
    }

    return std::move(lhs);
}


void Parser::consume(TokenKind kind){
    if(this->match(kind)){
        this->m_curTok = this->m_tokenizer.next();
    }
}

void Parser::expect(TokenKind kind, const char* msg){
    if(!this->match(kind)){
        throw std::runtime_error(msg);
    }
    this->consume(kind);
}


Expr Parser::parse_primary(void){
    if(this->match(TokenKind::Number)){
        auto val = this->m_curTok.num;
        this->consume(TokenKind::Number);
        return Creola::number(val);
    }
    if(this->match(TokenKind::Ident)){
        auto name = this->m_curTok.text;
        this->consume(TokenKind::Ident);
        if(this->match(TokenKind::LParen)){
            this->consume(TokenKind::LParen);
            Vec<Expr> args{};
            if(!this->match(TokenKind::RParen)){
                while(true){
                    args.push_back(this->parse());
                    if(this->match(TokenKind::Comma)){
                        this->consume(TokenKind::Comma);
                    }else{
                        break;
                    }
                }
            }
            this->expect(TokenKind::RParen, "expected ')'");
            //this->consume(TokenKind::RParen);
            return std::make_shared<FuncCall>(name, args);
        }
        return Creola::symbol(name);
    }
    if(this->match(TokenKind::LParen)){
        this->consume(TokenKind::LParen);
        auto expr = this->parse();
        this->expect(TokenKind::RParen, "expected ')'");
        //this->consume(TokenKind::RParen);
        return std::move(expr);
    }
    if(this->match(TokenKind::Minus)){
        this->consume(TokenKind::Minus);
        return std::make_shared<Neg>(this->parse_primary());
    }

    throw std::runtime_error("invalid primary");
}

Expr Parser::parse_pow(void){
    auto lhs = this->parse_primary();
    while(this->match(TokenKind::Caret)){
        this->consume(TokenKind::Caret);
        auto rhs = this->parse_primary();
        lhs = std::make_shared<Pow>(lhs, rhs);
    }

    return lhs;
}

Expr Parser::parse_term(void){
    auto lhs = this->parse_pow();
    while(this->match(TokenKind::Star) || this->match(TokenKind::Slash)){
        auto op = this->m_curTok.kind;
        this->consume(op);
        auto rhs = this->parse_pow();
        if(op==TokenKind::Star){
            lhs = std::make_shared<Mul>(Vec<Expr>{lhs, rhs});
        }else{
            lhs = std::make_shared<Mul>(Vec<Expr>{
                lhs,
                std::make_shared<Pow>(rhs, Creola::number(-1.0))
            });
        }
    }

    return std::move(lhs);
}

// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola::core                                 -*-
// -*----------------------------------------------------------------*-