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



/*
class Env final: std::enable_shared_from_this<Env>{
public:


Expr Env::assign(const Str& name, const Expr& expr);
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