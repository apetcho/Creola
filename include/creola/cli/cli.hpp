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

private:
};

// -*------------------------------------------------------------------------*-
}//-*- end::namespace::creola::cli                                          -*-
// -*------------------------------------------------------------------------*-