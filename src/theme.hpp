// Colour themes shared by the GUI, the HTML documentation and the terminal highlighter.
#pragma once

#include<string>

// ----------------------------------------------------------------------------
// begin::namespace::creola
// ----------------------------------------------------------------------------
namespace creola{
// -
enum class Token {
    Plain, Keyword, Number, String, Comment, Constant, Parameter,
    Definition, Operator, Punct, Error,
};

struct Theme {
    std::string name;   // "light" | "dark"
    bool dark{false};
    // -
    std::string window;
    std::string base;
    std::string alternateBase;
    std::string text;
    std::string mutedText;
    std::string accent;
    std::string border;
    std::string button;
    std::string highlight;
    std::string highlightedText;
    std::string link;

    // -
    std::string keyword;
    std::string number;
    std::string string;
    std::string comment;
    std::string command;
    std::string function;
    std::string constant;
    std::string parameter;
    std::string definition;
    std::string op;
    std::string error;

    const std::string& color(Token& tok) const;
};

const Theme& lightTheme(void);
const Theme& darkTheme(void);
const Theme& themeByName(const std::string& name, bool systemIsDark=false);

// ----------------------------------------------------------------------------
}// end::namespace::creola
// ----------------------------------------------------------------------------