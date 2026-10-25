// Context-aware code completion shared by the terminal and the GUI notebook.
#pragma once
#include<string>
#include<vector>

// ----------------------------------------------------------------------------
// begin::namespace::creola
// ----------------------------------------------------------------------------
namespace creola{
// -
class VM;

struct Completion {
    size_t start = 0;
    std::string prefix;
    std::vector<std::string> items;
    std::string commonPrefix(void) const;
};

Completion complete(const std::string& text, size_t cursor, const VM& vm);


// ----------------------------------------------------------------------------
}// end::namespace::creola
// ----------------------------------------------------------------------------
