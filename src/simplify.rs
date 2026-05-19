use crate::dsl::{Expr, Env, BinaryOp, UnaryOp};
use crate::Creola;

pub struct Simplifier;

impl Simplifier{
    pub fn simplify(expr: &Expr) -> Result<Expr, String> {
        let mut current = expr.clone();
        loop{
            let next = Simplifier::simplify_once(expr)?;
            if next == current{
                return Ok(next);
            }
            current = next;
        }
    }

    // -
    fn simplify_once(expr: &Expr) -> Result<Expr, String> {
        match expr {
            Expr::Num(_) | Expr::Var(_) => Ok(expr.clone()),

            Expr::Unary(UnaryOp::Neg, rhs) => {
                let s = Simplifier::simplify_once(rhs)?;
                match s {
                    Expr::Num(n) => Ok(Expr::Num(-n)),
                    Expr::Unary(UnaryOp::Neg, e) => Ok(*e),
                    _ => Ok(Expr::Unary(UnaryOp::Neg, Box::new(s))),
                }
            }

            Expr::Binary(op, lhs, rhs) => {
                let x = Simplifier::simplify_once(lhs)?;
                let y = Simplifier::simplify_once(rhs)?;
                Simplifier::simplify_binop(*op, x, y)
            }

            Expr::Call(name, args) => {
                let mut argv: Vec<Expr> = vec![];
                for arg in args.iter(){
                    let arg = Simplifier::simplify_once(arg)?;
                    argv.push(arg);
                }
                Simplifier::simplify_fun(name, argv)
            }
        }
    }

    fn simplify_binop(op: BinaryOp, lhs: Expr, rhs: Expr) -> Result<Expr, String> {
        todo!("")
    }

    fn flatten_add(expr: Expr, out: &mut Vec<Expr>) {
        todo!("")
    }

    fn simplify_add(lhs: Expr, rhs: Expr) -> Result<Expr, String> {
        todo!("")
    }

    fn simplify_sub(lhs: Expr, rhs: Expr) -> Result<Expr, String> {
        todo!("")
    }

    fn flatten_mul(expr: Expr, out: &mut Vec<Expr>) {
        todo!("")
    }

    fn simplify_mul() -> Result<Expr, String> {
        todo!("")
    }

    fn simplify_div(lhs: Expr, rhs: Expr) -> Result<Expr, String> {
        todo!("")
    }

    fn simplify_pow(lhs: Expr, rhs: Expr) -> Result<Expr, String> {
        todo!("")
    }

    fn simplify_fun(name: &str, args: Vec<Expr>) -> Result<Expr, String> {
        todo!("")
    }
}