use crate::dsl::{Expr, UnaryOp, BinaryOp};

pub struct Factorizer;

impl Factorizer{
    pub fn factorize(expr: &Expr) -> Result<Expr, String> {
        Ok(expr.clone())
    }
}