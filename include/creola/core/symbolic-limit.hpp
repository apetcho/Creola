#pragma once

#include "creola/core/common.hpp"

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola::core                               -*-
// -*----------------------------------------------------------------*-
namespace creola::core {
// -

struct SymbolicLimit{
    enum class Kind{ Finite, PosInf, NegInf, NaN };

    static Kind classify_value(f64 val);
    static Kind classify_at(const Expr& expr, const std::string& var, f64 val);

    static Expr limit(const Expr& expr, const std::string& var, f64 val, u32 max_iter=5);
};



// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola::core                                 -*-
// -*----------------------------------------------------------------*-