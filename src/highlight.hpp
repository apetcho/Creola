// Tolerant syntax tokenizer (never throws) with terminal (ANSI) and HTML renderers.
#pragma once
#include<vector>
#include<string>

#include "theme.hpp"

// ----------------------------------------------------------------------------
// begin::namespace::creola
// ----------------------------------------------------------------------------
namespace creola{
// -
class VM;

struct Span {
    size_t start = 0;
    size_t length = 0;
    Token kind = Token::Plain;
};

// -
std::vector<Span> highlight(const std::string& src, const VM* vm=nullptr);

std::string ansiHighlight(const std::string& src, const VM* vm=nullptr);
std::string htmlHightlight(const std::string& src, const Theme& theme, const VM* vm=nullptr);
std::string htmlEscape(const std::string& s);

// -
std::vector<Token> tokensPerCodePoint(const std::string& src, const VM* vm=nullptr);

// ----------------------------------------------------------------------------
}// end::namespace::creola
// ----------------------------------------------------------------------------