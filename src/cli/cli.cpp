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
creola::core::Vec<creola::core::Token> Repl::tokenize(const std::string& src){
    creola::core::Tokenizer tokenizer(src);
    creola::core::Vec<creola::core::Token> tokens{};
    auto token = tokenizer.next();
    while(token.kind != creola::core::TokenKind::End){
        tokens.push_back(token);
        token = tokenizer.next();
    }

    return tokens;
}

// -
creola::core::Vec<creola::core::Expr> Repl::parse(const std::string& src){
    creola::core::Vec<creola::core::Expr> result{};
    creola::core::Parser parser(src);
    while(parser.current().kind != creola::core::TokenKind::End){
        auto expr = parser.parse();
        result.push_back(std::move(expr));
    }

    return result;
}

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
    std::cerr << "BUILTIN COMMANDS:\n";
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
            // --
            // std::cerr << "<input value=" << std::quoted(src) << ">" << std::endl;
            auto kindToString = [](const creola::core::TokenKind kind){
                switch(kind){
                case creola::core::TokenKind::End:
                    return "END";
                case creola::core::TokenKind::Caret:
                    return "CARET";
                case creola::core::TokenKind::Comma:
                    return "COMMA";
                case creola::core::TokenKind::Equal:
                    return "EQUAL";
                case creola::core::TokenKind::Ident:
                    return "IDENT";
                case creola::core::TokenKind::KwFun:
                    return "FUN";
                case creola::core::TokenKind::KwLet:
                    return "LET";
                case creola::core::TokenKind::LParen:
                    return "LPAREN";
                case creola::core::TokenKind::Minus:
                    return "MINUS";
                case creola::core::TokenKind::Number:
                    return "NUMBER";
                case creola::core::TokenKind::Plus:
                    return "PLUS";
                case creola::core::TokenKind::RParen:
                    return "RPAREN";
                case creola::core::TokenKind::Slash:
                    return "SLASH";
                case creola::core::TokenKind::Star:
                    return "STAR";
                default:
                    throw std::runtime_error("unknown token kind.");
                }
            };

            // -*-
            // auto tokens = Repl::tokenize(src);
            // for(const auto& token: tokens){
            //     std::cout << "Token{ kind=" << kindToString(token.kind);
            //     std::cout << ", text=" << std::quoted(token.text);
            //     std::cout << ", num=" << token.num;
            //     std::cout << " }" << std::endl;
            // }

            // --
            {
                // std::vector<creola::core::Expr> exprs{};
                try{
                    auto exprs = Repl::parse(src);
                    for(size_t i=0; i < exprs.size(); ++i){
                        creola::core::Creola::println(
                            std::cerr, "items[", i, "] =", exprs[i]
                        );
                    }
                }catch(const creola::core::CreolaError& err){
                    creola::core::Creola::println(std::cerr, err.what());
                }catch(const std::exception& err){
                    creola::core::Creola::println(std::cerr, err.what());
                }catch(...){
                    creola::core::Creola::println(std::cerr, "Fatal error");
                }
            }

            // // --
            // auto result = creola.run(src);
            // if(result.is_ok()){
            //     std::cout << result.value() << std::endl;
            // }else{
            //     throw creola::core::CreolaError(result.value());
            // }
        }catch(const creola::core::CreolaError& err){
            std::cerr << "CreolaError: " << err.what() << std::endl;
        }catch(std::exception& err){
            std::cerr << "***Error***: " << err.what() << std::endl;
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