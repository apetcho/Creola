#include "creola/core/lexer.hpp"

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola::core                               -*-
// -*----------------------------------------------------------------*-
namespace creola::core{
//

// -*-
Lexer::Lexer(const Str& src) : m_src{}
{}

// -*-
Token Lexer::next(void){
    this->skip_ws();
    if(this->is_eos()){ return Token(); }

    auto c = this->peek();
    // read number 
    if(std::isdigit(c) || c == '.'){ return this->read_number(); }

    // read string := "text" | 'text'
    if(c=='"' || c == '\''){ return this->read_string(); }

    // read identifier
    if(std::isalpha(c) || c == '_'){ return this->read_identifier(); }

    // -*-
    this->advance();
    switch(c){
        case '+': return Token(TokenKind::Plot, "+");
        case '-': return Token(TokenKind::Minus, "-");
        case '*': return Token(TokenKind::Star, "*");
        case '/': return Token(TokenKind::Slash, "/");
        case '^': return Token(TokenKind::Caret, "^");
        case '(': return Token(TokenKind::LParen, "(");
        case ')': return Token(TokenKind::RParen, ")");
        case ',': return Token(TokenKind::Comma, ",");
        case '=': return Token(TokenKind::Equal, "=");
        case ';': return Token(TokenKind::Semi, ";");
    }

    return Token();
}

// -*-
void Lexer::skip_ws(void){
    while(!this->is_eos() && std::isspace(this->peek())){
        this->advance();
    }
}

// -*-
char Lexer::peek(void){
    if(this->is_eos()){ return EOF; }
    return this->m_src[this->m_pos];
}

// -*-
Token Lexer::read_number(void){
    auto c = this->peek();
    auto ndec = (c=='.') ? 1 : 0;
    Str numstr{};
    Str text{};

    while(!this->is_eos() && std::isdigit(c)){
        numstr += this->peek();
        this->advance();
    }
    if(this->is_eos()){
        if(ndec){ return Token(TokenKind::Float, numstr); }
        return Token(TokenKind::Float, numstr);
    }
    if(ndec==1 && this->match(this->peek(), '.')){ // already read the decimal point
        throw CreolaError("LexerError", "error while reading a number.");
    }

    if(ndec==0 && this->match(this->peek(), '.')){ // read fractional part
        numstr += this->peek();
        this->advance();
        while(!this->is_eos() && std::isdigit(this->peek())){
            numstr += this->peek();
            this->advance();
        }
        return Token(TokenKind::Float, numstr);
    }

    if(this->peek()=='e' || this->peek()=='E'){
        Str expo{};
        expo += 'e';
        if(this->match('-', 1)){ expo += '-';}
        if(this->match('+', 1)){ expo += '+';}
        if(std::isdigit(this->m_src[this->m_pos+2])){
            this->m_pos += 2;
            while(!this->is_eos() && std::isdigit(this->peek())){
                expo += this->peek();
                this->advance();
            }
            numstr += expo;
            return Token(TokenKind::Float, numstr);
        }
    }

    throw CreolaError("LexerError", "error while reading a number.");
}

// -*-
Token Lexer::read_string(void){
    Str lexeme{};
    if(this->peek()=='\''){
        this->advance();
        auto c = this->peek();
        while(!this->is_eos() && (std::isalnum(c) || c == '_')){
            if(c=='\\'){
                this->advance();
                c = this->peek();
                switch(c){
                    case 't': lexeme += "\t"; break;
                    case 'n': lexeme += "\n"; break;
                    case 'r': lexeme += "\r"; break;
                    case 'b': lexeme += "\b"; break;
                    case 'f': lexeme += "\f"; break;
                    case '\\': lexeme += "\\"; break;
                    case '\'': lexeme += "'"; break;
                    default: lexeme += c; break;
                }
                this->advance();
                continue;
            }
            lexeme += c;
        }
        if(this->is_eos()){
            throw CreolaError("LexerError", "error while reading string. Unexpected end of input.");
        }

        return Token(TokenKind::String, lexeme);
    }
    // double quoted string
    this->advance();
    auto c = this->peek();
    while(!this->is_eos() && (std::isalnum(c) || c == '_')){
        if(c=='\\'){
            this->advance();
            c = this->peek();
            switch(c){
                case 't': lexeme += "\t"; break;
                case 'n': lexeme += "\n"; break;
                case 'r': lexeme += "\r"; break;
                case 'b': lexeme += "\b"; break;
                case 'f': lexeme += "\f"; break;
                case '\\': lexeme += "\\"; break;
                case '\'': lexeme += "'"; break;
                default: lexeme += c; break;
            }
            continue;
        }
        lexeme += c;
    }
    if(this->is_eos()){
        throw CreolaError("LexerError", "error while reading string. Unexpected end of input.");
    }

    return Token(TokenKind::String, lexeme);
}

// -*-
Token Lexer::read_identifier(void){
    Str lexeme{};
    auto c = this->peek();
    this->advance();
    while(!this->is_eos() && (std::isalnum(c) || c == '_')){
        lexeme += c;
        this->advance();
        c = this->peek();
    }

    static Dict<Str, TokenKind> reservedWords = {
#define CREOLA_DEF(kind, text)  {text, TokenKind::kind},
        CREOLA_BUILTIN_FUNCTIONS()
        CREOLA_COMMANDS()
        CREOLA_KEYWORDS()
#undef CREOLA_DEF
    };

    auto entry = reservedWords.find(lexeme);
    if(entry != reservedWords.end()){
        return Token(entry->second, lexeme);
    }

    return Token(TokenKind::Ident, lexeme);
}


// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola::core                                 -*-
// -*----------------------------------------------------------------*-