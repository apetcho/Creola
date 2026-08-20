#pragma once

#include "creola/core/ast.hpp"
#include "creola/core/env.hpp"
#include "creola/core/parser.hpp"

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola::core                               -*-
// -*----------------------------------------------------------------*-
namespace creola::core{
// -
struct Configure{
    /*
    - interpreterConfig: JSON
    - rootsConfig: JSON
    - solveConfig: JSON
    - limitConfig: JSON
    - integrateConfig: JSON
    - pprintConfig: JSON
    - plotConfig: JSON
    */
};

// -*-
class Interpreter final{
public:


private:
    Env m_runtime;
    /*
    static HashSet<Str> keywords;
    static HashSet<Str> builtinFunctions;
    static HashSet<Str> commands;
    static Dict<Str, MathFun> knownIntegrals;
    static Dict<Str, MathFun> knownDerivatives;
    static Dict<Str, MathFun> knownLimits;
    */
};

// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola::core                                 -*-
// -*----------------------------------------------------------------*-