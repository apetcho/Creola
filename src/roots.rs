use crate::dsl::{Expr, Env};
use crate::Creola;

static MAX_ITERATIONS: usize = 50;

pub struct RootFinder;

// -
impl RootFinder{
    pub fn roots(expr: &Expr, var: &str, guess: f64) -> Result<Option<f64>, String> {
        let mut x = guess;
        for _ in 0..MAX_ITERATIONS {
            let env = {
                let mut ctx = Env::default();
                ctx.vars.insert(var.to_string(), Expr::Num(x));
                ctx
            };

            let fx = match Creola::eval(expr, &env)? {
                Expr::Num(n) => n,
                _ => {
                    return Ok(None);
                }
            };

            let diff = Creola::diff(expr, var)?;
            let dfx = match Creola::eval(&diff, &env)? {
                Expr::Num(n) => n,
                _ => {
                    return Ok(None);
                }
            };

            if dfx.abs() < 1e-12 {
                break;
            }
            let xnew = x - fx/dfx;
            if (xnew-x).abs() < 1e-12 {
                return Ok(Some(xnew));
            }
            x = xnew;
        }
        Ok(None)
    }
}