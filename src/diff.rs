use crate::dsl::{Expr, UnaryOp, BinaryOp};

pub struct Differentiator;

impl Differentiator{
    pub fn diff(expr: &Expr, var: &str) -> Expr {
        todo!("")
    }

    fn diff_unary(expr: &Expr, var: &str) -> Expr {
        todo!("")
    }

    fn diff_binary(op: BinaryOp, lhs: &Expr, rhs: &Expr, var: &str) -> Expr {
        todo!("")
    }

    fn diff_function_call(name: &str, args: Vec<Expr>) -> Expr {
        todo!("")
    }
}