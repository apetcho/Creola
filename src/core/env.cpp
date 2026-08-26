#include "creola/core/env.hpp"

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola::core                               -*-
// -*----------------------------------------------------------------*-
namespace creola::core{
//

Env::Env(Env* parent)
: m_bindings{}
, m_parent{parent}
{}

// -*-
void Env::define(const Str& name, const Expr& expr){
    this->m_bindings[name] = expr;
}

// -*-
Expr Env::assign(const Str& name, const Expr& expr){
    auto entry = this->m_bindings.find(name);
    if(entry == this->m_bindings.end() && this->m_parent==nullptr){
        throw CreolaError("undefined +'" + name + "'");
    }
    if(entry != this->m_bindings.end()){
        return entry->second;
    }

    return this->m_parent->assign(name, expr);
}

/*
class Env final: std::enable_shared_from_this<Env>{
public:


Expr& Env::get(const Str& name) const;
void Env::pprint(PrettyPrinter& pprinter) const;

private:
    Dict<Str, Expr> m_bindings;
    Env* m_parent;
};

*/

// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola::core                                 -*-
// -*----------------------------------------------------------------*-