mod dsl;
use dsl::Expr;

mod simplify;
use simplify::Simplify;

mod diff;
use diff::Differentiator;

mod integrate;
use integrate::Integrator;

mod factor;
use factor::Factorizer;

mod expand;
use expand::Expander;

mod roots;
use roots::RootFinder;

mod solve;
use solve::Solver;

mod series;
use series::Series;

mod repl;
pub use repl::Repl;

pub struct Creola;


impl Creola{
    pub fn simplify(expr: &Expr) -> Expr {
        todo!("")
    }

    pub fn diff(expr: &Expr, var: &str) -> Expr {
        todo!("")
    }

    pub fn integrate(expr: &Expr, var: &str) -> Expr {
        todo!("")
    }

    pub fn factor(expr: &Expr) -> Expr {
        todo!("")
    }

    pub fn expand(expr: &Expr) -> Expr {
        todo!("")
    }

    pub fn roots(expr: &Expr, var: &str, guess: f64) -> Option<f64>{
        todo!("")
    }

    pub fn solve(expr: &Expr, var: &str, guess: f64) -> Option<f64> {
        todo!("")
    }

    pub fn taylor(expr: &Expr, var: &str, order: usize) -> Expr {
        todo!("")
    }
}