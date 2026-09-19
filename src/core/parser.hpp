#pragma once

#include "ast.hpp"
#include "tokenizer.hpp"

#include<vector>

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola                                     -*-
// -*----------------------------------------------------------------*-
namespace creola{
// -

class Parser final {
public:
    Parser(const std::string& src);
    Stmt parse(void);

private:
    std::vector<Token> m_tokens;
    std::size_t m_cur;

    Token current(void) const;
    bool match(TokenKind kind) const;
    [[maybe_unused]] Token consume(TokenKind kind, const std::string& msg);
    void advance(void);

    Stmt parseStatement(void);
    Stmt parseLet(void);
    Stmt parseFun(void);

    Expr parseExpression(void);
    Expr parseAddSub(void);
    Expr parseMulDiv(void);
    Expr parsePow(void);
    Expr parseUnary(void);
    Expr parsePrimary(void);
    //Expr parseCall(void);
    //Expr parseLambda(void);
};


// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola                                       -*-
// -*----------------------------------------------------------------*-