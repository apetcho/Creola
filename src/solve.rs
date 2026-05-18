use crate::dsl::Expr;
use crate::Creola;

pub struct Solver;

impl Solver{
    pub fn solve(expr: &Expr, var: &str, guess: f64) -> Result<Expr, String> {
        let expr = Creola::roots(expr, var, guess)?;
        Ok(expr)
    }
}