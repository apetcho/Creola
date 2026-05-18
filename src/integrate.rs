use crate::dsl::{Expr, UnaryOp, BinaryOp};

pub struct Integrator;

impl Integrator{
    pub fn integrate(expr: &Expr, var: &str) -> Expr {
        todo!("")
    }

    fn integrate_unary(expr: &Expr) -> Expr {
        todo!("")
    }

    fn integrate_binary(op: BinaryOp, lhs: &Expr, rhs: &Expr) -> Expr {
        todo!("")
    }

    fn integrate_func(name: &str, args: Vec<Expr>) -> Expr {
        todo!("")
    }
}