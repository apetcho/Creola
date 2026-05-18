use crate::dsl::{Expr, Env, BinaryOp};
use crate::Creola;

pub struct Series;

impl Series{
    pub fn taylor(expr: &Expr, var: &str, order: usize) -> Result<Expr, String> {
        let mut terms: Vec<Expr> = Vec::new();
        let mut diff = expr.clone();
        for k in 0..=order{
            let env = {
                let mut ctx = Env::default();
                ctx.vars.insert(var.to_string(), Expr::Num(0.0));
                ctx
            };

            let val = match Creola::eval(expr, &env)? {
                Expr::Num(n) => n,
                _ => 0.0,
            };

            let coeff = val / factorial(k) as f64;
            let term = if k == 0 {
                Expr::Num(coeff)
            }else{
                Expr::Binary(
                    BinaryOp::Mul,
                    Box::new(Expr::Num(coeff)),
                    Box::new(Expr::Binary(
                        BinaryOp::Pow,
                        Box::new(Expr::Var(var.to_string())),
                        Box::new(Expr::Num(k as f64))
                    ))
                )
            };
            terms.push(term);
            diff = Creola::diff(&diff, var)?;
        }

        let ans = terms
            .into_iter()
            .reduce(|x, y| Expr::Binary(BinaryOp::Add, Box::new(x), Box::new(y)))
            .unwrap_or(Expr::Num(0.0));

        Ok(ans)
    }
}

fn factorial(n: usize) -> usize{
    (1..=n).product::<usize>().max(1)
}