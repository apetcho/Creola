
use crate::dsl::{Expr, Env, BinaryOp, UnaryOp};

pub struct Evaluator;

impl Evaluator{
    pub fn eval(expr: &Expr, env: &Env) -> Expr{
        todo!("")
    }

    fn eval_unary(expr: &Expr, env: &Env) -> Expr {
        if let Expr::Num(num) = Evaluator::eval(expr, env){
            Expr::Num(-num)
        }else{
            Expr::Unary(UnaryOp::Neg, Box::new(Evaluator::eval(expr, env)))
        }
    }

    fn eval_binary(op: BinaryOp, lhs: &Expr, rhs: &Expr, env: &Env) -> Expr{
        todo!("")
    }

    fn eval_function_call(name: &str, args: Vec<Expr>) -> Expr {
        todo!("")
    }
}