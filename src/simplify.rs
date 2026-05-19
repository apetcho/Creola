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
        match op {
            BinaryOp::Add => Simplifier::simplify_add(lhs, rhs),
            BinaryOp::Sub => Simplifier::simplify_sub(lhs, rhs),
            BinaryOp::Mul => Simplifier::simplify_mul(lhs, rhs),
            BinaryOp::Div => Simplifier::simplify_div(lhs, rhs),
            BinaryOp::Pow => Simplifier::simplify_pow(lhs, rhs),
        }
    }

    fn flatten_add(expr: Expr, out: &mut Vec<Expr>) {
        if let Expr::Binary(BinaryOp::Add, lhs, rhs) = expr {
            Simplifier::flatten_add(*lhs, out);
            Simplifier::flatten_add(*rhs, out);
        }else{
            out.push(expr);
        }
    }

    fn simplify_add(lhs: Expr, rhs: Expr) -> Result<Expr, String> {
        // flatten nested additions
        let mut terms = Vec::new();
        Simplifier::flatten_add(lhs, &mut terms);
        Simplifier::flatten_add(rhs, &mut terms);

        // combine numeric constants
        let mut const_sum = 0.0;
        let mut others = Vec::new();
        for term in terms{
            if let Expr::Num(n) = term{
                const_sum+= n;
            }else{
                others.push(term);
            }
        }

        let mut result: Option<Expr> = None;
        if const_sum != 0.0 {
            result = Some(Expr::Num(const_sum));
        }

        for term in others {
            result = Some(match result {
                None => term,
                Some(acc) => Expr::Binary(BinaryOp::Add, Box::new(acc), Box::new(term)),
            });
        }
        match result{
            None => {
                return Ok(Expr::Num(0.0));
            }
            Some(e) => {
                return Ok(e);
            }
        }
    }

    fn simplify_sub(lhs: Expr, rhs: Expr) -> Result<Expr, String> {
        // a - b ==> a + (-b)
        let rhs = Expr::Unary(UnaryOp::Neg, Box::new(rhs));
        Simplifier::simplify_add(lhs, rhs)
    }

    fn flatten_mul(expr: Expr, out: &mut Vec<Expr>) {
        todo!("")
    }

    fn simplify_mul(lhs: Expr, rhs: Expr) -> Result<Expr, String> {
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