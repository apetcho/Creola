use crate::dsl::{Expr, UnaryOp, BinaryOp};

pub struct Differentiator;

impl Differentiator{
    pub fn diff(expr: &Expr, var: &str) -> Result<Expr, String> {
        match expr{
            Expr::Num(_) => Ok(Expr::Num(0.0)),
            Expr::Var(name) => {
                if name == var { Ok(Expr::Num(1.0)) }
                else{ Ok(Expr::Num(0.0)) }
            }
            Expr::Unary(_, rhs) => Differentiator::diff_unary(rhs, var),
            Expr::Binary(op, lhs, rhs) => {
                Differentiator::diff_binary(op.clone(), lhs, rhs, var)
            }
            Expr::Call(name, args) => {
                Differentiator::diff_function_call(name, args.clone())
            }
        }
    }

    fn diff_unary(expr: &Expr, var: &str) -> Result<Expr, String> {
        todo!("")
    }

    fn diff_binary(op: BinaryOp, lhs: &Expr, rhs: &Expr, var: &str) -> Result<Expr, String> {
        todo!("")
    }

    fn diff_function_call(name: &str, args: Vec<Expr>) -> Result<Expr, String> {
        todo!("")
    }
}