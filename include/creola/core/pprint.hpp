#pragma once

#include "creola/core/visitors.hpp"
#include<set>

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola::core                               -*-
// -*----------------------------------------------------------------*-
namespace creola::core{
// -

// -*--------------------------*-
// -*- Box type and utilities -*-
// -*--------------------------*-
class Box final {
public:
    explicit Box(const Vec<Str>& lines={});

    static Box make_text(const std::string& text);
    static Box make_fraction(const Box& num, const Box& den);
    static Box make_power(const Box& base, const Box& expo);
    static Box make_parens(const Box& inner);

    Box vpad(int top, int bottom) const;
    Box hcat(const Box& rhs) const;
    Box vcat(const Box& rhs) const;
    Box center(usize width) const;

    Str to_string(void) const;

private:
    Vec<Str> m_lines;
    usize m_width = 0;
    usize m_height = 0;
};

// -------------
// -*- Boxer -*-
// -------------
class Boxer final: protected ToBoxVisitor{
public:
    explicit Boxer();
    explicit Boxer(const Expr& expr);
    explicit Boxer(Expr&& expr);

    Box to_box(void) const;

private:
    Expr m_expr;

    void to_box(const SymbolExpr& expr, Box& box) const override;
    void to_box(const NumberExpr& expr, Box& box) const override;
    void to_box(const NegExpr& expr, Box& box) const override;
    void to_box(const AddExpr& expr, Box& box) const override;
    void to_box(const MulExpr& expr, Box& box) const override;
    void to_box(const PowExpr& expr, Box& box) const override;
    void to_box(const LambdaExpr& expr, Box& box) const override;
    void to_box(const CallExpr& expr, Box& box) const override;
    void to_box(const EquationExpr& expr, Box& box) const override;
    void to_box(const SystemExpr& expr, Box& box) const override;
};


// -*-
class PrettyPrinter final: protected PrintVisitor {
public:
    explicit PrettyPrinter()
    : m_stream{std::cout}, m_justify{"left"}{}
    explicit PrettyPrinter(std::ostream& os)
    : m_stream{os}, m_justify{"left"}{}

    void pprint(i64 num);
    void pprint(f64 num);
    void pprint(const Str& str);
    void pprint(const SymbolExpr& expr);
    void pprint(const NumberExpr& expr);
    void pprint(const NegExpr& expr);
    void pprint(const AddExpr& expr);
    void pprint(const MulExpr& expr);
    void pprint(const PowExpr& expr);
    void pprint(const LambdaExpr& expr);
    void pprint(const CallExpr& expr);
    void pprint(const EquationExpr& expr);
    void pprint(const SystemExpr& expr);

    void set_width(i32 w){
        this->m_width = w;
    }

    void set_justify(const Str& value){
        static std::set<Str> JUSTIFICATIONS = {"left", "center", "right" };
        auto entry = JUSTIFICATIONS.find(value);
        if(entry == JUSTIFICATIONS.end()){
            this->m_justify = "left";
        }else{
            this->m_justify = value;
        }
    }

private:
    std::ostream& m_stream;
    i32 m_width{-1};
    Str m_justify; // "left", "center", "right"

    void print(std::ostream& os, const SymbolExpr& expr) override;
    void print(std::ostream& os, const NumberExpr& expr) override;
    void print(std::ostream& os, const NegExpr& expr) override;
    void print(std::ostream& os, const AddExpr& expr) override;
    void print(std::ostream& os, const MulExpr& expr) override;
    void print(std::ostream& os, const PowExpr& expr) override;
    void print(std::ostream& os, const LambdaExpr& expr) override;
    void print(std::ostream& os, const CallExpr& expr) override;
    void print(std::ostream& os, const EquationExpr& expr) override;
    void print(std::ostream& os, const SystemExpr& expr) override;
};


// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola::core                                      -*-
// -*----------------------------------------------------------------*-