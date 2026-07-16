#include "core-expr.hpp"

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola                                     -*-
// -*----------------------------------------------------------------*-
namespace creola{
//

struct FuncionDef{
    std::string param; // univariate function
    core::Expr body;
};

class Creola{
public:

    // - handle_line()
    void run(const std::string& src);
    // - parse_expression_only()
    core::Expr parse(const std::string& src);

private:
    HashMap<std::string, core::Expr> m_vars;
    HashMap<std::string, FuncionDef> m_funcs;

    core::Expr substitute(const core::Expr& expr, const std::string& var, const core::Expr& val);
    // apply_user_func ==> apply 
    core::Expr apply(const std::string& name, const Vec<core::Expr>& args);
};

// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola                                       -*-
// -*----------------------------------------------------------------*-