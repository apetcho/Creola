#include "core/repl.hpp"

#include<iostream>


// ------------------------------
// -*- M A I N    D R I V E R -*-
// ------------------------------
int main(void){
    creola::Repl repl;
    std::cerr << "Creola Computer Algebra System v0.1" << std::endl;
    std::cerr << "Type 'exit' to quit." << std::endl;
    repl.run();

    return 0;
}


/*
// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola                                     -*-
// -*----------------------------------------------------------------*-
namespace creola{
// -


// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola                                       -*-
// -*----------------------------------------------------------------*-
*/