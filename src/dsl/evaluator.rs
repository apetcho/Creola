
use crate::dsl::{Expr, Env, BinaryOp, UnaryOp};

pub struct Evaluator;

impl Evaluator{
    pub fn eval(expr: &Expr, env: &Env) -> Result<Expr, String>{
        todo!("")
    }

    fn eval_unary(expr: &Expr, env: &Env) -> Result<Expr, String> {
        if let Expr::Num(num) = Evaluator::eval(expr, env)?{
            Ok(Expr::Num(-num))
        }else{
            Ok(Expr::Unary(UnaryOp::Neg, Box::new(Evaluator::eval(expr, env)?)))
        }
    }

    fn eval_binary(op: BinaryOp, lhs: &Expr, rhs: &Expr, env: &Env) -> Result<Expr, String>{
        let lhs = Evaluator::eval(lhs, env)?;
        let rhs = Evaluator::eval(rhs, env)?;
        match (op, lhs, rhs){
            (BinaryOp::Add, Expr::Num(x), Expr::Num(y)) => Ok(Expr::Num(x+y)),
            (BinaryOp::Sub, Expr::Num(x), Expr::Num(y)) => Ok(Expr::Num(x-y)),
            (BinaryOp::Mul, Expr::Num(x), Expr::Num(y)) => Ok(Expr::Num(x*y)),
            (BinaryOp::Div, Expr::Num(x), Expr::Num(y)) => {
                if y == 0.0 {
                    return Err("division by zero".into());
                }
                Ok(Expr::Num(x/y))
            }
            (BinaryOp::Pow, Expr::Num(x), Expr::Num(y)) => Ok(Expr::Num(x.powf(y))),
            (op, lhs, rhs) => Ok(Expr::Binary(op, Box::new(lhs), Box::new(rhs))),
        }
    }

    fn eval_function_call(name: &str, args: Vec<Expr>) -> Result<Expr, String> {
        todo!("")
    }
}