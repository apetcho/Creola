// Typeset-style rendering: 2D Unicode text, LaTeX and Typst output.
#pragma once

#include<string>

#include "vm.hpp"

// ----------------------------------------------------------------------------
// begin::namespace::creola
// ----------------------------------------------------------------------------
namespace creola{
// -
std::string prettyText(const Value& value);
std::string prettyText(const Expr& value);
std::string toLatex(const Value& value);
std::string toTypst(const Value& value);

// ----------------------------------------------------------------------------
}// end::namespace::creola
// ----------------------------------------------------------------------------