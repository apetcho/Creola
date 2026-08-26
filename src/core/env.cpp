#include "creola/core/env.hpp"
#include "creola/core/ast.hpp"

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
void Env::assign(const Str& name, const Expr& expr){
    auto entry = this->m_bindings.find(name);
    if(entry == this->m_bindings.end() && this->m_parent==nullptr){
        throw CreolaError("undefined +'" + name + "'");
    }
    if(entry != this->m_bindings.end()){
        this->m_bindings[name] = expr;
    }

    return this->m_parent->assign(name, expr);
}

// -*-
Expr Env::get(const Str& name) const{
    auto entry = this->m_bindings.find(name);
    if(entry == this->m_bindings.end() && this->m_parent==nullptr){
        throw CreolaError("undefined +'" + name + "'");
    }
    if(entry != this->m_bindings.end()){
        return entry->second;
    }

    return this->m_parent->get(name);
}

// -*-
void Env::pprint(PrettyPrinter& pprinter) const{
    Str line(80, '-');
    line += "\n";
    //          ------------------------------------------------------------------
    Str header{"               C U R R E N T     E N V I R O N M E N T           \n"};
    //          ------------------------------------------------------------------

    /*
    --------------------------------------------------------------
               C U R R E N T    E N V I R O N M E N T
    --------------------------------------------------------------
      SYMBOLS           VALUES                             TYPE
    --------------------------------------------------------------
    x[w=16]            value[w=44]                      type[w=10]
    */
    pprinter.pprint(line);
    pprinter.pprint(header);
    pprinter.pprint(line);
    pprinter.set_width(16);
    pprinter.set_justify("left");
    pprinter.pprint("SYMBOLS");
    pprinter.set_width(48);
    pprinter.set_justify("left");
    pprinter.pprint("VALUES");
    pprinter.set_width(16);
    pprinter.set_justify("left");
    pprinter.pprint("TYPES");
    Str type{};
    for(const auto& [key, expr]: this->m_bindings){
        pprinter.set_width(16);
        pprinter.set_justify("left");
        pprinter.pprint(" " + key);
        pprinter.set_width(48);
        pprinter.set_justify("left");
        if(is_symbol_expr(expr)){
            auto self = as_symbol_expr(expr);
            pprinter.pprint(self);
            pprinter.set_width(16);
            pprinter.set_justify("left");
            pprinter.pprint("var\n");
        }else if(is_number_expr(expr)){
            auto self = as_number_expr(expr);
            pprinter.pprint(self);
            pprinter.set_width(16);
            pprinter.set_justify("left");
            pprinter.pprint("number\n");
        }else if(is_neg_expr(expr)){
            auto self = as_neg_expr(expr);
            pprinter.pprint(self);
            pprinter.set_width(16);
            pprinter.set_justify("left");
            pprinter.pprint("expr\n");
        }else if(is_add_expr(expr)){
            auto self = as_add_expr(expr);
            pprinter.pprint(self);
            pprinter.set_width(16);
            pprinter.set_justify("left");
            pprinter.pprint("expr\n");
        }else if(is_mul_expr(expr)){
            auto self = as_mul_expr(expr);
            pprinter.pprint(self);
            pprinter.set_width(16);
            pprinter.set_justify("left");
            pprinter.pprint("expr\n");
        }else if(is_pow_expr(expr)){
            auto self = as_pow_expr(expr);
            pprinter.pprint(self);
            pprinter.set_width(16);
            pprinter.set_justify("left");
            pprinter.pprint("expr\n");
        }else if(is_lambda_expr(expr)){
            auto self = as_lambda_expr(expr);
            pprinter.pprint(self);
            pprinter.set_width(16);
            pprinter.set_justify("left");
            pprinter.pprint("lambda\n");
        }else if(is_call_expr(expr)){
            auto self = as_call_expr(expr);
            pprinter.pprint(self);
            pprinter.set_width(16);
            pprinter.set_justify("left");
            pprinter.pprint("expr\n");
        }else if(is_equation_expr(expr)){
            auto self = as_equation_expr(expr);
            pprinter.pprint(self);
            pprinter.set_width(16);
            pprinter.set_justify("left");
            pprinter.pprint("equation\n");
        }else if(is_system_expr(expr)){
            auto self = as_system_expr(expr);
            pprinter.pprint(self);
            pprinter.set_width(16);
            pprinter.set_justify("left");
            pprinter.pprint("system\n");
        }
    }
    pprinter.pprint(line);
    pprinter.pprint("\n");
}


// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola::core                                 -*-
// -*----------------------------------------------------------------*-