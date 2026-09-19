#include "env.hpp"
#include<stdexcept>

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

// -*-
bool Env::hashKey(const std::string& key) const{
    if(this->m_bindings.count(key)){ return true; }
    if(this->parent){ return this->parent->hashKey(key); }

    return false;
}

// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola                                       -*-
// -*----------------------------------------------------------------*-