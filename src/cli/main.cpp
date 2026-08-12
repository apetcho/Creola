#include "creola/cli/cli.hpp"

// -----------------------------
// -*- M A I N   D R I V E R -*-
// -----------------------------
int main(int argc, char** argv){
    creola::cli::Repl repl;
    repl.run();
    return EXIT_SUCCESS;
}