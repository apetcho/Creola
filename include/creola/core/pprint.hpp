#pragma once
#include "creola/core/common.hpp"


// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola::core                               -*-
// -*----------------------------------------------------------------*-
namespace creola::core{
// -

struct Number;
struct Symbol;
struct Neg;
struct Add;
struct Mul;
struct Pow;
struct FuncCall;

/*
struct Simplifier;
struct Factorizer;
struct Expander;
struct Integrator;
struct Differentiator;
struct Series;
struct LimitFinder;
struct RootsFinder;
*/

// -*- Display::display -*-
struct PrintVisitor {
    virtual ~PrintVisitor() = default;

    virtual void visit(std::ostream& os, const Number& expr, int prec=0) const = 0;
    virtual void visit(std::ostream& os, const Symbol& expr, int prec=0) const = 0;
    virtual void visit(std::ostream& os, const Neg& expr, int prec=0) const = 0;
    virtual void visit(std::ostream& os, const Add& expr, int prec=0) const = 0;
    virtual void visit(std::ostream& os, const Mul& expr, int prec=0) const = 0;
    virtual void visit(std::ostream& os, const Pow& expr, int prec=0) const = 0;
    virtual void visit(std::ostream& os, const FuncCall& expr, int prec=0) const = 0;
};

// // - Box type and utilities
// class Box{
// public:
//     explicit Box(const Vec<std::string>& lines={}){}

//     static Box make_text(const std::string& text){}
//     static Box make_fraction(const Box& num, const Box& den){}
//     static Box make_power(const Box& base, const Box& expo){}
//     static Box make_parens(const Box& inner){}

//     Box vpad(int top, int bottom) const{}
//     Box hcat(const Box& rhs) const{}
//     Box vcat(const Box& rhs) const{}
//     Box center(usize width) const{}

// private:
//     Vec<std::string> m_lines;
//     usize m_width = 0;
//     usize m_height = 0;
// };


// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola::core                                      -*-
// -*----------------------------------------------------------------*-