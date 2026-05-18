
use crate::dsl::{Expr, Env, BinaryOp, UnaryOp};

pub struct Evaluator;

impl Evaluator{
    pub fn eval(expr: &Expr, env: &Env) -> Result<Expr, String>{
        match expr {
            Expr::Num(_) => Ok(expr.clone()),
            Expr::Var(name) => {
                if let Some(v) = env.vars.get(name).cloned(){
                    Ok(v)
                }else{
                    Err(format!("Undefined variable {}", name.clone()))
                }
            }
            Expr::Unary(_, rhs) => {
                Evaluator::eval_unary(rhs, env)
            }
            Expr::Binary(op, lhs, rhs) => {
                Evaluator::eval_binary(op.clone(), lhs, rhs, env)
            }
            Expr::Call(name, args) => {
                Evaluator::eval_function_call(name, args.clone(), env)
            }
        }
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

    fn eval_function_call(name: &str, args: Vec<Expr>, env: &Env) -> Result<Expr, String> {
        let mut argv: Vec<Expr> = Vec::new();
        for arg in args.iter(){
            match Evaluator::eval(arg, env) {
                Ok(expr) => argv.push(expr),
                Err(msg) => {
                    return Err(msg.clone());
                }
            }
        }

        match name{
            "exp" => {
                if let [Expr::Num(x)] = argv.as_slice(){
                    Ok(Expr::Num(x.exp()))
                }else{
                    Ok(Expr::Call(name.to_string(), argv))
                }
            }

            "sin" => {
                if let [Expr::Num(x)] = argv.as_slice(){
                    Ok(Expr::Num(x.sin()))
                }else{
                    Ok(Expr::Call(name.to_string(), argv))
                }
            }

            "cos" => {
                if let [Expr::Num(x)] = argv.as_slice(){
                    Ok(Expr::Num(x.cos()))
                }else{
                    Ok(Expr::Call(name.to_string(), argv))
                }
            }
            _ => {
                if let Some(def) = env.funcs.get(name){
                    if def.params.len() == argv.len(){
                        let mut local_env = env.clone();
                        for (p, v) in def.params.iter().zip(argv.iter()){
                            local_env.vars.insert(p.clone(), v.clone());
                        }
                        Evaluator::eval(&def.body, &local_env)
                    }else{
                        Ok(Expr::Call(name.to_string(), argv))
                    }
                }else{
                    Ok(Expr::Call(name.to_string(), argv))
                }
            }
        }
    }
}