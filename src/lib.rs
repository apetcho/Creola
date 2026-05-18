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
        let expr = Evaluator::eval(expr, env)?;
        Creola::simplify(&expr)
    }
    pub fn simplify(expr: &Expr) -> Result<Expr, String> {
        Simplifier::simplify(expr)
    }

    pub fn diff(expr: &Expr, var: &str) -> Result<Expr, String> {
        let expr = Differentiator::diff(expr, var)?;
        Creola::simplify(&expr)
    }

    pub fn integrate(expr: &Expr, var: &str) -> Result<Expr, String> {
        let expr = Integrator::integrate(expr, var)?;
        Creola::simplify(&expr)
    }

    pub fn factor(expr: &Expr) -> Result<Expr, String> {
        let expr = Factorizer::factorize(expr)?;
        Creola::simplify(&expr)
    }

    pub fn expand(expr: &Expr) -> Result<Expr, String> {
        let expr = Expander::expand(expr)?;
        Creola::simplify(&expr)
    }

    pub fn roots(expr: &Expr, var: &str, guess: f64) -> Result<Expr, String>{
        let expr = RootFinder::roots(expr, var, guess)?;
        let expr = match expr {
            Some(e) => Expr::Num(e),
            None => {
                return Err("no roots founds".into());
            }
        };
        Creola::simplify(&expr)
    }

    pub fn solve(expr: &Expr, var: &str, guess: f64) -> Result<Expr, String> {
        let expr = Solver::solve(expr, var, guess)?;
        let expr = match expr{
            Some(e) => {
                Expr::Num(e)
            }
            None => {
                return Err("no solution found".into());
            }
        };
        Creola::simplify(&expr)
    }

    pub fn taylor(expr: &Expr, var: &str, order: usize) -> Result<Expr, String> {
        let expr = Series::taylor(expr, var, order)?;
        Creola::simplify(&expr)
    }
}