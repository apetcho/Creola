use crate::dsl::Expr;

pub struct Factorizer;

impl Factorizer{
    pub fn factorize(expr: &Expr) -> Result<Expr, String> {
        Ok(expr.clone())
    }
}