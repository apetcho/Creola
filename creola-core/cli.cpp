#include "expr.hpp"
#include "parser.hpp"

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola                                     -*-
// -*----------------------------------------------------------------*-
namespace creola{
// -
Expr parse(const std::string& line);
void handle_line(const std::string& line);

// -*-
class Handler{
public:
    explicit Handler(const std::string& line);

    void handle(void);

private:
    Parser m_parser;

    void handle_let(void);
    void handle_fun(void);
    
    void handle_simplify(void);
    void handle_expand(void);
    void handle_factor(void);
    void handle_diff(void);
    void handle_integrate(void);
    void handle_taylor(void);
    void handle_limit(void);
    void handle_roots(void);
    void handle_groebner(void);
};

class Repl{
public:

    void run(void);

    static std::string highlight(const std::string& line);
private:
};

// -*----------------------------------------------------------------*-
};//-*- end::namespace::creola                                      -*-
// -*----------------------------------------------------------------*-