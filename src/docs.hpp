// Command documentation as structured data as a rich text (HTML), for the GUI
// help panel.
#pragma once

#include<vector>
#include<string>
#include<map>

#include "theme.hpp"


// ----------------------------------------------------------------------------
// begin::namespace::creola
// ----------------------------------------------------------------------------
namespace creola{
// -

struct DocEntry{
    std::string name;
    std::string syntax;
    std::string text;
    std::string category;
    std::string example;
};

const std::map<std::string, DocEntry>& docEntries(void);
const std::vector<std::string>& docCategory(void);

// -
std::string docIndexHtml(const Theme& theme);
std::string docPageHtml(const std::string& name, const Theme& theme);
std::string docConfigHtml(const Theme& theme);


// ----------------------------------------------------------------------------
}// end::namespace::creola
// ----------------------------------------------------------------------------