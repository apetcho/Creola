#pragma once

#include "ast.hpp"
#include<map>
#include<string>


// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola                                     -*-
// -*----------------------------------------------------------------*-
namespace creola{
// -

class Env final {
public:
    Env();
    Env(Env* parent);

    void put(const std::string& key, Expr expr);
    Expr get(const std::string& key) const;
    bool hashKey(const std::string& key) const;
    Env* parent;

private:
    std::map<std::string, Expr> m_bindings;
};


// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola                                       -*-
// -*----------------------------------------------------------------*-