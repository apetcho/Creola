#pragma once
#include "creola/creola.hpp"

// -*------------------------------------------------------------------------*-
// -*- begin::namespace::creola::cli                                        -*-
// -*------------------------------------------------------------------------*-
namespace creola::cli{
// -
class Repl{
public:
    void run(void);
    static std::string highlight(const std::string& line);

    static creola::core::Vec<creola::core::Token> tokenize(const std::string& src);
    static creola::core::Vec<creola::core::Expr> parse(const std::string& src);

private:
};

// -*------------------------------------------------------------------------*-
}//-*- end::namespace::creola::cli                                          -*-
// -*------------------------------------------------------------------------*-