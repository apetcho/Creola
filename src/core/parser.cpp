#include "creola/core/parser.hpp"
#include "creola/core/engine.hpp"

#include<cctype>
#include<string>
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
    auto c = this->peek();
    if(c==EOF){
        return Token(TokenKind::End, "");
    }

    static std::map<char, TokenKind> cdict = {
        { '^', TokenKind::Caret },
        { ',', TokenKind::Comma },
        { '=', TokenKind::Equal },
        { '(', TokenKind::LParen },
        { ')', TokenKind::RParen },
        { '-', TokenKind::Minus },
        { '+', TokenKind::Plus },
        { '/', TokenKind::Slash },
        { '*', TokenKind::Star },
    };
    auto entry = cdict.find(c);
    if(entry != cdict.end()){
        this->advance();
        return Token(entry->second, c);
    }
    if(std::isdigit(c)){
        return this->read_number();
    }else if(std::isalpha(c) || c=='_'){
        return this->read_symbol();
    }

    std::stringstream ss;
    ss << "invalid token character '" << c << "' detected.";
    throw CreolaError(ss.str());
}

void Tokenizer::skip_whitespace(void){
    auto c = this->peek();
    while(std::isspace(c)){
        if(c==EOF){ break; }
        this->advance();
        c = this->peek();
    }
}

// -*-
Token Tokenizer::read_symbol(void){
    auto c = this->peek();
    std::string text{};
    static std::string chars = 
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789_";
    while(c != EOF){
        if(std::isspace(c)){ break; }
        if(chars.find(c)==std::string::npos){
            break;
        }
        text += c;
        this->advance();
        c = this->peek();
    }
    return this->match_symbol(text);
}

// -*-
Token Tokenizer::read_number(void){
    // -
    bool floating{false};
    std::stringstream ss;
    // - mantissa
    while(this->peek() != EOF){
        auto c = this->peek();
        if(std::isspace(c)){ break; }
        if(std::isdigit(c)){
            ss << c;
        }
        this->advance();
    }
    // - decimal point
    if(this->peek()=='.'){
        this->advance();
        ss << '.';
        floating = true;
    }
    // - fractional part
    while(this->peek() != EOF){
        auto c = this->peek();
        if(std::isspace(c)){ break; }
        if(std::isdigit(c)){
            ss << c;
        }
        this->advance();
    }
    if(this->peek()=='e' || this->peek()=='E'){
        ss << this->peek();
        this->advance();
        if(this->peek()=='-' || this->peek()=='+'){
            ss << this->peek();
            this->advance();
        }
        while(this->peek() != EOF){
            auto c = this->peek();
            if(std::isspace(c)){ break; }
            if(std::isdigit(c)){
                ss << c;
            }
            this->advance();
        }
    }
    
    Token token{};
    try{
        auto numstr = ss.str();
        auto num = std::stod(numstr);
        token.kind = TokenKind::Number;
        token.text = numstr;
        token.num = num;
    }catch(const std::out_of_range& err){
        throw CreolaError(std::string(err.what()));
    }catch(const std::invalid_argument& err){
        throw CreolaError(std::string(err.what()));
    }catch(...){
        throw CreolaError();
    }
    return token;
}

// -*-
Token Tokenizer::match_symbol(const std::string& text){
    if(text=="let"){ return Token(TokenKind::KwLet, text); }
    if(text=="fun"){ return Token(TokenKind::KwFun, text); }

    return Token(TokenKind::Ident, text);
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
            lhs = Creola::make_add_expr(Vec<Expr>{lhs, rhs});
        }else{
            lhs = Creola::make_add_expr(Vec<Expr>{
                lhs, Creola::make_neg_expr(rhs)
            });
        }
    }

    return std::move(lhs);
}

// -
void Parser::consume(TokenKind kind){
    this->m_curTok = this->m_tokenizer.next();
}

void Parser::expect(TokenKind kind, const char* msg){
    if(!this->match(kind)){
        throw CreolaError(msg);
    }
}


Expr Parser::parse_primary(void){
    if(this->match(TokenKind::Number)){
        auto val = this->m_curTok.num;
        this->consume(TokenKind::Number);
        return Creola::make_number_expr(val);
    }
    if(this->match(TokenKind::Ident)){// name
        auto name = this->m_curTok.text;
        this->consume(TokenKind::Ident);
        if(this->match(TokenKind::LParen)){// name(args)
            this->consume(TokenKind::LParen);
            Vec<Expr> args{};
            if(!this->match(TokenKind::RParen)){
                while(true){
                    args.push_back(this->parse());
                    if(this->match(TokenKind::Comma)){
                        this->consume(TokenKind::Comma);
                    }else{ break;}
                }
            }
            this->consume(TokenKind::RParen, "expected ')'");
            return Creola::make_funcall_expr(name, args);
        }
        return Creola::make_symbol_expr(name);
    }
    if(this->match(TokenKind::LParen)){// (expr)
        this->consume(TokenKind::LParen);
        auto expr = this->parse();
        this->consume(TokenKind::RParen, "expected ')'");
        return std::move(expr);
    }
    if(this->match(TokenKind::Minus)){// -expr
        this->consume(TokenKind::Minus);
        return Creola::make_neg_expr(this->parse_primary());
    }

    throw CreolaError("invalid primary");
}

Expr Parser::parse_pow(void){
    auto lhs = this->parse_primary();
    while(this->match(TokenKind::Caret)){// expr^expr
        this->consume(TokenKind::Caret);
        auto rhs = this->parse_primary();
        lhs = Creola::make_pow_expr(lhs, rhs);
    }

    return std::move(lhs);
}

Expr Parser::parse_term(void){
    auto lhs = this->parse_pow();
    while(this->match(TokenKind::Star) || this->match(TokenKind::Slash)){
        auto op = this->m_curTok.kind;
        this->consume(op);
        auto rhs = this->parse_pow();
        if(op==TokenKind::Star){
            lhs = Creola::make_mul_expr(Vec<Expr>{lhs, rhs});
        }else{// x/y === x * y^(-1)
            lhs = Creola::make_mul_expr(Vec<Expr>{
                lhs,
                Creola::make_pow_expr(rhs, Creola::make_number_expr(-1.0))
            });
        }
    }

    return std::move(lhs);
}

// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola::core                                 -*-
// -*----------------------------------------------------------------*-