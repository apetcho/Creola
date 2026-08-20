#pragma once

#include "creola/core/common.hpp"
#include "creola/core/pprint.hpp"

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola                                     -*-
// -*----------------------------------------------------------------*-
namespace creola::core{
// -

class Env final: std::enable_shared_from_this<Env>{
public:
    explicit Env(Env* parent=nullptr);
    Env(const Env&) = default;
    Env(Env&&) = default;
    Env& operator=(const Env&) = default;
    Env& operator=(Env&&) = default;
    ~Env() = default;

    void define(const Str& name, const Expr& expr);
    Expr assign(const Str& name, const Expr& expr);
    Expr& get(const Str& name) const;

    void pprint(PrettyPrinter& pprinter) const;

private:
    Dict<Str, Expr> m_bindings;
    Env* m_parent;
};

// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola                                       -*-
// -*----------------------------------------------------------------*-