#include "creola/cli/cli.hpp"

#define CREOLA_USE_READLINE
#ifdef CREOLA_USE_READLINE
#include<readline/readline.h>
#include<readline/history.h>
#endif

// -*-------------------------------------------------------------------------*-
// -*- begin::namespace::creola::cli                                         -*-
// -*-------------------------------------------------------------------------*-
namespace creola::cli{
// -
std::string Repl::highlight(const std::string& line){
    using namespace creola::core;
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

// -*-
// -
void Repl::run(void){
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::cerr << "Creola COMPUTER ALGEBRA SYSTEM\n\n";
    std::cerr << "BUILTIN COMMANDS\:n";
    std::cerr << " - simplify\n";
    std::cerr << " - diff\n";
    std::cerr << " - expand\n";
    std::cerr << " - factor\n";
    std::cerr << " - integrate\n";
    std::cerr << " - taylor\n";
    std::cerr << " - limit\n";
    std::cerr << " - roots\n";

    const std::string example = R"CREOLA(
        creola>> let x = 2
        creaol>> fun f(x) = x^2 + 1
        creaol>> diff(f(x), x)
    )CREOLA";
    std::cerr << "\nEXAMPLE:\n";
    std::cerr << example << std::endl;

    creola::core::Creola creola;

#ifdef CREOLA_USE_READLINE
    while(true){
        char* input = ::readline("creaola>> ");
        if(!input){ continue; }
        std::string src(input);
        std::free(input);
        if(src=="quit" || src=="exit"){ break; }
        ::add_history(src.c_str());
        
        try{
            creola.run(src);
        }catch(std::exception& err){
            std::cerr << "Error: " << err.what() << std::endl;
        }catch(...){
            std::cerr << "Error: unknown error encountered." << std::endl;
        }
    }
#else
    std::string src{};
    while(true){
        std::cout << "creola>> ";
        if(!std::getline(std::cin, src)){ continue; }
        if(src=="quit" || src=="exit"){ break; }
        try{
            creola.run(src);
        }catch(std::exception& err){
            std::cerr << "Error: " << err.what() << std::endl;
        }catch(...){
            std::cerr << "Error: unknown error encountered." << std::endl;
        }
    }
#endif

    return;
}


// -*-------------------------------------------------------------------------*-
}//-*- end::namespace::creola::cli                                           -*-
// -*-------------------------------------------------------------------------*-