#include "creola.hpp"

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola                                     -*-
// -*----------------------------------------------------------------*-
namespace creola{
// -
// Expr parse(const std::string& line);
// void handle_line(const std::string& line);

// -*-
class Handler{
public:
    explicit Handler(const std::string& line){}

    void handle(void){}

private:
    Parser m_parser;

    void handle_let(void){}
    void handle_fun(void){}
    
    void handle_simplify(void){}
    void handle_expand(void){}
    void handle_factor(void){}
    void handle_diff(void){}
    void handle_integrate(void){}
    void handle_taylor(void){}
    void handle_limit(void){}
    void handle_roots(void){}
    void handle_groebner(void){}
};

class Repl{
public:

    void run(void){}

    static std::string highlight(const std::string& line){
        // very simple: number in CYAN, identifier in GREEN, keyowrds in MAGENTA
        std::string result{};
        Tokenizer tokenizer(line);
        Token token;

        while((token=tokenizer.next()).kind != TokenKind::End){
            std::string color{};
            std::string reset{"\x1b[0m"};
            switch(token.kind){
            case TokenKind::Number:
                color = "\x1b[36m";     // CYAN
                break;
            case TokenKind::Ident:
                color = "\x1b[32m";     // GREEN
                break;
            case TokenKind::KwFun:
            case TokenKind::KwLet:
                color = "\x1b[35m";     // MAGENTA
                break;
            default:
                color = "";
                break;
            }
            if(!color.empty()){
                result += color + token.text + reset;
            }else{
                result += token.text;
            }
            result += " ";
        }

        return result;
    }
private:
};

// -*----------------------------------------------------------------*-
};//-*- end::namespace::creola                                      -*-
// -*----------------------------------------------------------------*-