#pragma once
#include "creola/core/common.hpp"

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola::core                               -*-
// -*----------------------------------------------------------------*-
namespace creola::core{
// -
//! @todo: Add support for complex number tokenization

// -*- Keywords
#define CREOLA_KEYWORDS()   \
    CREOLA_DEF(Let, "let")  \
    CREOLA_DEF(Fun, "fun")

// -*- Builtin Functions
#define CREOLA_BUILTIN_FUNCTIONS()      \
    CREOLA_DEF(Equation, "equation")    \
    CREOLA_DEF(System, "system")

// -*- Commands
#define CREOLA_COMMANDS()               \
    CREOLA_DEF(Simplify, "simplify")    \
    CREOLA_DEF(Diff, "diff")            \
    CREOLA_DEF(Expand, "expand")        \
    CREOLA_DEF(Factor, "factor")        \
    CREOLA_DEF(Integral, "integral")    \
    CREOLA_DEF(Integrate, "integrate")  \
    CREOLA_DEF(Taylor, "taylor")        \
    CREOLA_DEF(Limit, "limit")          \
    CREOLA_DEF(Roots, "roots")          \
    CREOLA_DEF(Solve, "solve")          \
    CREOLA_DEF(Fibonacci, "fibonacci")  \
    CREOLA_DEF(Factorial, "factorial")  \
    CREOLA_DEF(Modulo, "modulo")        \
    CREOLA_DEF(Prime, "prime")          \
    CREOLA_DEF(Gcd, "gcd")              \
    CREOLA_DEF(Lcm, "lcm")              \
    CREOLA_DEF(Help, "help")            \
    CREOLA_DEF(Show, "show")            \
    CREOLA_DEF(Plot, "plot")            \
    CREOLA_DEF(Config, "config")

// -*- Symbols
#define CREOLA_SYMBOLS()        \
    CREOLA_DEF(LParen, "(")     \
    CREOLA_DEF(RParen, ")")     \
    CREOLA_DEF(Comma, ",")      \
    CREOLA_DEF(Semi, ";")       \
    CREOLA_DEF(Equal, "=")

// -*- Operators
#define CREOLA_OPERATORS()  \
    CREOLA_DEF(Plus, "+")   \
    CREOLA_DEF(Minus, "-")  \
    CREOLA_DEF(Star, "*")   \
    CREOLA_DEF(Slash, "/")  \
    CREOLA_DEF(Caret, "^")


#define CREOLA_TOKENS()             \
    CREOLA_DEF(End, "END")          \
    CREOLA_DEF(Ident, "IDENT")      \
    CREOLA_DEF(String, "STRING")    \
    CREOLA_DEF(Integer, "INTEGER")  \
    CREOLA_DEF(Float, "FLOAT")      \
    CREOLA_KEYWORDS()               \
    CREOLA_BUILTIN_FUNCTIONS()      \
    CREOLA_COMMANDS()               \
    CREOLA_SYMBOLS()                \
    CREOLA_OPERATORS()


// -----------------
// -*- TokenKind -*-
// -----------------
enum class TokenKind{
#define CREOLA_DEF(kind, _) kind,
    CREOLA_TOKENS()
#undef CREOLA_DEF
};

// -*-
struct Token{
    TokenKind kind;
    Str text;
    Token()
    : kind{TokenKind::End}, text{""}
    {}

    Token(TokenKind kind, const Str& lexeme)
    : kind{kind}, text{lexeme}
    {}
};

// -*-
class Lexer{
public:
    explicit Lexer(const Str& src);
    ~Lexer() = default;
    Token next(void);

private:
    Str m_src;
    usize m_pos = 0;

    void skip_ws(void);
    char peek(void);
    void advance(void){
        this->m_pos++;
    }
    bool is_eos(void){ return this->m_pos >= this->m_src.length(); }
    Token read_number(void);
    Token read_string(void);
    Token read_identifier(void);

    // -*-
    bool match(char c, u32 pos=0){
        auto idx = this->m_pos + pos;
        if(idx >= this->m_src.length() && c == EOF){ return true; }
        if(idx >= this->m_src.length()){ return false; }

        return this->m_src[idx]==c;
    }
};


// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola::core                                 -*-
// -*----------------------------------------------------------------*-