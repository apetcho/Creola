use crate::dsl::{Expr, BinaryOp};

pub struct Expander;

impl Expander{
    pub fn expand(expr: &Expr) -> Result<Expr, String> {
        Expander::expand_helper(expr)
    }

    fn expand_helper(expr: &Expr) -> Result<Expr, String> {
        match expr {
            Expr::Num(_) | Expr::Var(_) => Ok(expr.clone()),

            Expr::Unary(op, rhs) => {
                let e = Expander::expand_helper(rhs)?;
                Ok(Expr::Unary(*op, Box::new(e)))
            }

            Expr::Binary(op, lhs, rhs) => {
                let lhs = Expander::expand_helper(lhs)?;
                let rhs = Expander::expand_helper(rhs)?;
                match op {
                    BinaryOp::Add => Ok(Expr::Binary(BinaryOp::Add, Box::new(lhs), Box::new(rhs))),
                    BinaryOp::Sub => Ok(Expr::Binary(BinaryOp::Sub, Box::new(lhs), Box::new(rhs))),
                    BinaryOp::Mul => Expander::expand_mul(lhs, rhs),
                    BinaryOp::Div => {
                        // we don't distribute division; just expand numberator/denominator
                        Ok(Expr::Binary(BinaryOp::Div, Box::new(lhs), Box::new(rhs)))
                    }
                    BinaryOp::Pow => Expander::expand_pow(lhs, rhs),
                }
            }

            Expr::Call(name, args) => {
                let mut argv = vec![];
                for arg in args.iter() {
                    let arg = Expander::expand_helper(arg)?;
                    argv.push(arg);
                }

                Ok(Expr::Call(name.clone(), argv))
            }
        }
    }

    fn expand_mul(lhs: Expr, rhs: Expr) -> Result<Expr, String> {
        match (lhs, rhs) {
            // (A + B) * C => A*C + B*C
            (Expr::Binary(BinaryOp::Add, x, y), z) => {
                let x = Expander::expand_mul(*x, z.clone())?;
                let y = Expander::expand_mul(*y, z)?;
                Ok(Expr::Binary(BinaryOp::Add, Box::new(x), Box::new(y)))
            }

            // (A - B) * C => A*C - B*C
            (Expr::Binary(BinaryOp::Sub, x, y), z) => {
                let x = Expander::expand_mul(*x, z.clone())?;
                let y = Expander::expand_mul(*y, z)?;
                Ok(Expr::Binary(BinaryOp::Sub, Box::new(x), Box::new(y)))
            }

            // C * (A + B) => C*A + C*B
            (x, Expr::Binary(BinaryOp::Add, y, z)) => {
                let a = Expander::expand_mul(x.clone(), *y)?;
                let b = Expander::expand_mul(x, *z)?;
                Ok(Expr::Binary(BinaryOp::Add, Box::new(a), Box::new(b)))
            }

            // C * (A - B) => C*A - C*B
            (x, Expr::Binary(BinaryOp::Sub, y, z)) => {
                let a = Expander::expand_mul(x.clone(), *y)?;
                let b = Expander::expand_mul(x, *z)?;
                Ok(Expr::Binary(BinaryOp::Sub, Box::new(a), Box::new(b)))
            }

            // otherwise just x*y
            (x, y) => Ok(Expr::Binary(BinaryOp::Mul, Box::new(x), Box::new(y))),
        }
    }

    fn expand_pow(base: Expr, expo: Expr) -> Result<Expr, String> {
        match expo {
            Expr::Num(n) => {
                let k = n.round() as i32;
                if (n - k as f64).abs() < 1e-12 && k >= 0 && k <= 16 {
                    // small non-negative integer exponent: repeated multiplication
                    Expander::expand_pow_int(base, k as usize)
                }else{
                    Ok(Expr::Binary(BinaryOp::Pow, Box::new(base), Box::new(Expr::Num(n))))
                }
            }

            _ => Ok(Expr::Binary(BinaryOp::Pow, Box::new(base), Box::new(expo)))
        }
    }

    fn expand_pow_int(base: Expr, n: usize) -> Result<Expr, String> {
        if n == 0 {
            return Ok(Expr::Num(1.0));
        }

        if n == 1 {
            return Ok(base);
        }

        // exponentiation by squaring with expansion at each step
        let mut result = Expr::Num(1.0);
        let mut b = base;
        let mut e = n;

        while e > 0 {
            if e % 2 == 1 {
                result = Expander::expand_mul(result, b.clone())?;
            }

            if e > 1 {
                b = Expander::expand_mul(b.clone(), b)?;
            }

            e /= 2;
        }

        Ok(result)
    }
}