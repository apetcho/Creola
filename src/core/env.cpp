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

// -*-
void Env::put(const std::string& key, Expr expr){
    this->m_bindings[key] = expr;
}

// -
Expr Env::get(const std::string& key) const{
    if(this->m_bindings.count(key)){
        return this->m_bindings.at(key);
    }
    if(this->parent){
        return this->parent->get(key);
    }
    throw std::runtime_error("Undefined variable: " + key);
}

/*
class Env final {
public:


bool Env::hashKey(const std::string& key) const{}
    Env* parent;

private:
    std::map<std::string, Expr> m_bindings;
};
*/

// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola                                       -*-
// -*----------------------------------------------------------------*-