#pragma once

#include "common.hpp"
#include "expr.hpp"


// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola                                     -*-
// -*----------------------------------------------------------------*-
namespace creola{
//

using MatchEnv = HashMap<std::string, Expr>;

// -
class PatternMatcher{
public:
    explicit PatternMatcher(Expr pattern);

    bool match(const Expr& expr, MatchEnv& env);

private:
    Expr m_pattern;

    bool match_symbol(const Shared<Symbol>& pattern, const Expr& expr, MatchEnv& env);
    Expr subst_pattern(const Expr& pattern, const MatchEnv& env);
};


struct Rule{
    Expr pattern;
    Expr replacement;

    bool apply_once(const Expr& expr, Expr& result) const;

    static Expr rewrite(const Expr& expr, const Vec<Rule>& rules, int max_iter=20);
};

// -*-
Rule sin2cos = {
    //! @todo
};


Rule _ = {
    //! @todo
};


// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola                                       -*-
// -*----------------------------------------------------------------*-