mod dsl;
use dsl::{Expr, Evaluator, Env};

mod simplify;
use simplify::Simplifier;

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
    pub fn eval(expr: &Expr, env: &Env) -> Result<Expr, String> {
        Evaluator::eval(expr, env)
    }
    pub fn simplify(expr: &Expr) -> Result<Expr, String> {
        Simplifier::simplify(expr)
    }

    pub fn diff(expr: &Expr, var: &str) -> Result<Expr, String> {
        Differentiator::diff(expr, var)
    }

    pub fn integrate(expr: &Expr, var: &str) -> Result<Expr, String> {
        todo!("")
    }

    pub fn factor(expr: &Expr) -> Result<Expr, String> {
        todo!("")
    }

    pub fn expand(expr: &Expr) -> Result<Expr, String> {
        todo!("")
    }

    pub fn roots(expr: &Expr, var: &str, guess: f64) -> Result<Option<f64>, String>{
        todo!("")
    }

    pub fn solve(expr: &Expr, var: &str, guess: f64) -> Result<Option<f64>, String> {
        todo!("")
    }

    pub fn taylor(expr: &Expr, var: &str, order: usize) -> Result<Expr, String> {
        todo!("")
    }
}