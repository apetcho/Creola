use crate::dsl::{Expr, Env};
use crate::Creola;

pub struct Simplifier;

impl Simplifier{
    pub fn simplify(expr: &Expr) -> Result<Expr, String> {
        let env = Env::default();
        Creola::eval(expr, &env)
    }
}