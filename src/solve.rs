use crate::dsl::Expr;
use crate::Creola;

pub struct Solver;

impl Solver{
    pub fn solve(expr: &Expr, var: &str, guess: f64) -> Result<Option<f64>, String> {
        Creola::roots(expr, var, guess)
    }
}