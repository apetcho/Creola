#pragma once

#include "parser.hpp"
#include "evaluator.hpp"
#include "env.hpp"

#include<string>


// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola                                     -*-
// -*----------------------------------------------------------------*-
namespace creola{
// -

class Repl final{
public:
    Repl();
    void run(void);

private:
    Env m_env;

    std::string prompt(void) const;
    void processLine(const std::string& line);
};

// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola                                       -*-
// -*----------------------------------------------------------------*-