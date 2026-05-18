
use crate::dsl::{Expr, Env, BinaryOp, UnaryOp};

pub struct Evaluator;

impl Evaluator{
    pub fn eval(expr: &Expr, env: &Env) -> Expr{
        todo!("")
    }

    fn eval_unary(expr: &Expr) -> Expr {
        todo!("")
    }

    fn eval_binary(op: BinaryOp, lhs: &Expr, rhs: &Expr) -> Expr{
        todo!("")
    }

    fn eval_function_call(name: &str, args: Vec<Expr>) -> Expr {
        todo!("")
    }
}