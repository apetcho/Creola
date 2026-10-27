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
    //! @todo
    return {};
}

// -*-
struct Parser {
    std::vector<Token> tokens;
    size_t pos;
    const std::function<bool(const std::string&)>& callable;
    std::set<std::string> funs;

    const Token& peek(void) const;
    bool isPunct(const char* cstr) const;
    bool accept(const char* cstr);
    void expect(const char* cstr);
    static bool keyword(const std::string& str);
    static AstPtr make(Ast::Kind kind, std::string str={});
    static AstPtr make_binary_op(const char* op, AstPtr lhs, AstPtr rhs);
    std::string ident(void);
    void skip_newline(void);
    std::vector<AstPtr> parse_program(void);
    AstPtr parse_statement(void);
    AstPtr parse_expr(void);
    bool is_implicit_mul(void) const;
    AstPtr parse_term(void);
    AstPtr parse_unary(void);
    AstPtr parse_arg(void);
    AstPtr parse_spec(void);
    AstPtr parse_postfix(void);
    AstPtr parse_primary(void);
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