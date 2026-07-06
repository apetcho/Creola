#pragma once
#include "common.hpp"


// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola                                     -*-
// -*----------------------------------------------------------------*-
namespace creola{
// -

// - Box type and utilities
class Box{
public:
    explicit Box(const Vec<std::string>& lines={});

    static Box make_text(const std::string& text);
    static Box make_fraction(const Box& num, const Box& den);
    static Box make_power(const Box& base, const Box& expo);
    static Box make_parens(const Box& inner);

    Box vpad(int top, int bottom) const;
    Box hcat(const Box& rhs) const;
    Box vcat(const Box& rhs) const;
    Box center(usize width) const;

private:
    Vec<std::string> m_lines;
    usize m_width = 0;
    usize m_height = 0;
};


// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola                                       -*-
// -*----------------------------------------------------------------*-