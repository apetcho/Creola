#include "env.hpp"

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola                                     -*-
// -*----------------------------------------------------------------*-
namespace creola{
// -

Env::Env()
: parent{nullptr}
{}


Env::Env(Env* parent)
: parent{parent}
{}

/*
class Env final {
public:


void Env::put(const std::string& key, Expr expr){}
Expr Env::get(const std::string& key) const{}
bool Env::hashKey(const std::string& key) const{}
    Env* parent;

private:
    std::map<std::string, Expr> m_bindings;
};
*/

// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola                                       -*-
// -*----------------------------------------------------------------*-