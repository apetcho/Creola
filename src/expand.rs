use crate::dsl::Expr;

pub struct Expander;

impl Expander{
    pub fn expand(expr: &Expr) -> Result<Expr, String> {
        Ok(expr.clone())
    }
}