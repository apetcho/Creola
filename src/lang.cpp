#include "lang.hpp"
#include "typedefs.hpp"

#include<stdexcept>
#include<cctype>
#include<set>

// ----------------------------------------------------------------------------
// begin::namespace::creola
// ----------------------------------------------------------------------------
namespace creola{
// -
namespace {
// -*-
enum class TokenKind {
    Num, Ident, Str, Punct, Newline, End,
};

struct Token {
    TokenKind kind;
    std::string text;
    f64 num;
};

// -*-
[[noreturn]] void fail(const std::string& msg){
    throw std::runtime_error("syntax error: " + msg);
}

// -*-
std::vector<Token> lex(const std::string& src){
    std::vector<Token> result{};
    size_t i = 0;
    size_t len = src.length();
    i32 depth = 0;

    // -*-
    auto is_identity_start = [](unsigned char c){
        return std::isalpha(c) || c == '_' || c >= 0x80;
    };

    auto digit_p = [&](size_t k){
        return k < len && std::isdigit(static_cast<unsigned char>(src[k]));
    };

    while(i < len){
        unsigned char c = src[i];
        if(c == '\n'){
            if(depth==0){
                result.push_back({TokenKind::Newline, "\n"});
                ++i;
                continue;
            }
        }
        if(std::isspace(c)){
            ++i;
            continue;
        }
        if(c=='/' && i + 1 < len && src[i+1]=='/'){
            while(i < len && src[i] != '\n'){
                ++i;
                continue;
            }
        }
        if(digit_p(i) || (c=='.' && digit_p(i+1))){
            size_t j = i;
            while(j < len && (digit_p(j) || src[j] == '.')){ ++j; }
            if(j < len && (src[j]=='e' || src[j]=='E')){
                size_t k = j + 1;
                if(k < len && (src[k] == '+' || src[k] == '-')){ ++k; }
                if(digit_p(k)){
                    while(digit_p(k)){ ++k;}
                    j = k;
                }
            }
            auto numstr = src.substr(i, j-i);
            result.push_back({TokenKind::Num, numstr, std::stod(numstr)});
            i = j;
            continue;
        }
        if(is_identity_start(c)){
            size_t j = i;
            while(j < len && (is_identity_start(src[j]) || digit_p(j))){ ++j; }
            result.push_back({TokenKind::Ident, src.substr(i, j-i)});
            i = j;
            continue;
        }
        if(c=='"'){
            std::string str;
            ++i;
            while(i < len && src[i] != '"'){
                if(src[i]=='\\' && i+1 < len){ ++i; }
                str += src[i++];
            }
            if(i >= len){
                fail("unterminated string");
            }
            ++i;
            result.push_back({TokenKind::Str, str});
            continue;
        }
        if(std::string("+-*/^()[]{},=:;").find(static_cast<char>(c)) != std::string::npos){
            if(c=='(' || c == '[' || c=='{'){ ++depth; }
            if((c==')' || c == ']' || c=='}') && depth > 0){ --depth; }
            if(c==';'){
                result.push_back({TokenKind::Newline, ";"});
            }else{
                result.push_back({TokenKind::Punct, std::string(1, static_cast<char>(c))});
            }
            ++i;
            continue;
        }
        fail(std::string("unexpected character '") + static_cast<char>(c) + "'");
    }
    result.push_back({TokenKind::End, ""});

    return result;
}

// -*-
struct Parser {
    std::vector<Token> tokens;
    size_t pos;
    const std::function<bool(const std::string&)>& callable;
    std::set<std::string> funs;

    // -*-
    const Token& peek(void) const{ return tokens[pos]; }

    // -*-
    bool isPunct(const char* cstr) const{
        return tokens[pos].kind==TokenKind::Punct && tokens[pos].text==cstr;
    }

    // -*-
    bool accept(const char* cstr){
        if(this->isPunct(cstr)){
            ++pos;
            return true;
        }
        return false;
    }

    // -*-
    void expect(const char* cstr){
        if(!this->accept(cstr)){
            fail(std::string("expected '") + cstr + "' but found '" + this->peek().text + "'");
        }
    }

    // -*-
    static bool isKeyword(const std::string& str){
        return str=="let" || str=="fun";
    }

    static AstPtr make(Ast::Kind kind, std::string str={}){
        auto self = std::make_shared<Ast>();
        self->kind = kind;
        self->text = std::move(str);
        return self;
    }

    // -*-
    static AstPtr makeBinaryOp(const char* op, AstPtr lhs, AstPtr rhs){
        auto self = Parser::make(Ast::BinOp, op);
        self->children = {std::move(lhs), std::move(rhs)};
        return self;
    }

    // -*-
    std::string ident(void){
        if(this->peek().kind != TokenKind::Ident || this->isKeyword(this->peek().text)){
            fail("expected a name");
            return ""; // never reached
        }
        return tokens[pos++].text;
    }

    // -*-
    void skipNewline(void){
        while(this->peek().kind == TokenKind::Newline){
            ++pos;
        }
    }

    // -*-
    std::vector<AstPtr> parseProgram(void){
        std::vector<AstPtr> result{};
        this->skipNewline();
        while(this->peek().kind != TokenKind::End){
            result.push_back(this->parseStatement());
            if(this->peek().kind != TokenKind::End && this->peek().kind != TokenKind::Newline){
                fail("unexpected '" + this->peek().text + "'");
                return {}; // never reached
            }
            this->skipNewline();
        }
        return result;
    }

    // -*-
    AstPtr parseStatement(void){
        if(this->peek().kind == TokenKind::Ident && this->peek().text == "let"){
            ++pos;
            auto self = Parser::make(Ast::Let, this->ident());
            this->expect("=");
            self->children = {this->parseExpr()};
            return self;
        }
        if(this->peek().kind==TokenKind::Ident && this->peek().text=="fun"){
            ++pos;
            auto self = Parser::make(Ast::Fun, this->ident());
            this->expect("(");
            if(!this->isPunct(")")){
                do{
                    self->params.push_back(this->ident());
                }while(this->accept(","));
            }
            this->expect(")");
            this->funs.insert(self->text); // allow recursion
            this->expect("=");
            self->children = {this->parseExpr()};
            return self;
        }

        return this->parseExpr();
    }

    // -*-
    AstPtr parseExpr(void){
        auto self = this->parseTerm();
        while(true){
            if(this->accept("+")){
                self = Parser::makeBinaryOp("+", self, this->parseTerm());
            }else if(this->accept("-")){
                self = Parser::makeBinaryOp("-", self, this->parseTerm());
            }else{
                return self;
            }
        }
    }

    // -*-
    bool isImplicitMul(void) const{
        return (
            (this->peek().kind == TokenKind::Ident && !this->isKeyword(this->peek().text))
            || this->isPunct("(")
        );
    }

    // -*-
    AstPtr parseTerm(void){
        auto self = this->parseUnary();
        while(true){
            if(this->accept("*")){
                self = Parser::makeBinaryOp("*", self, this->parseUnary());
            }else if(this->accept("/")){
                self = Parser::makeBinaryOp("/", self, this->parseUnary());
            }else if(this->isImplicitMul()){
                self = Parser::makeBinaryOp("*", self, this->parseUnary());
            }else{
                return self;
            }
        }
    }

    // -*-
    AstPtr parseUnary(void){
        if(this->accept("-")){
            auto self = Parser::make(Ast::Neg);
            self->children = {this->parseUnary()};
            return self;
        }
        if(this->accept("+")){ return this->parseUnary(); }
        auto base = this->parsePostfix();
        if(this->accept("^")){
            return Parser::makeBinaryOp("^", base, this->parseUnary());
        }

        return base;
    }

    // -*- expr | name=expr
    AstPtr parseArg(void){
        if(this->peek().kind == TokenKind::Ident && !this->isKeyword(this->peek().text) && tokens[pos+1].kind==TokenKind::Punct && tokens[pos+1].text=="="){
            auto self = Parser::make(Ast::Named, tokens[pos].text);
            pos += 2;
            self->children = {this->parseExpr()};
            return self;
        }
        return this->parseExpr();
    }

    // -*-
    // index spec: expr | [expr]: [expr]
    AstPtr parseIndexSpec(void){
        auto lo = this->isPunct(":") ? nullptr : this->parseExpr();
        if(!this->accept(":")){ return lo; }

        auto makeNumber = [](f64 num){
            auto self = Parser::make(Ast::Num);
            self->num = num;
            return self;
        };

        AstPtr hi = (this->isPunct(",") || this->isPunct("]")) ? makeNumber(1e9) : this->parseExpr();
        auto self = Parser::make(Ast::Slice);
        self->children = {lo ? lo : makeNumber(0), hi};
        return self;
    }

    // -*-
    AstPtr parsePostfix(void);
    AstPtr parsePrimary(void);
};


}// namespace
// -
std::vector<AstPtr> parse(
    const std::string& src,
    const std::function<bool(const std::string&)>& isCallable
){
    //! @todo
}

// ----------------------------------------------------------------------------
}// end::namespace::creola
// ----------------------------------------------------------------------------