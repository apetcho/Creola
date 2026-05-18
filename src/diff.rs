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
                Differentiator::diff_function_call(name, args.clone(), var)
            }
        }
    }

    fn diff_unary(expr: &Expr, var: &str) -> Result<Expr, String> {
        let ans = Differentiator::diff(expr, var)?;
        Ok(Expr::Unary(UnaryOp::Neg, Box::new(ans)))
    }

    fn diff_binary(op: BinaryOp, lhs: &Expr, rhs: &Expr, var: &str) -> Result<Expr, String> {
        match op {
            BinaryOp::Add => Differentiator::diff_binary_add(lhs, rhs, var),
            BinaryOp::Sub => Differentiator::diff_binary_sub(lhs, rhs, var),
            BinaryOp::Mul => Differentiator::diff_binary_mul(lhs, rhs, var),
            BinaryOp::Div => Differentiator::diff_binary_div(op, lhs, rhs, var),
            BinaryOp::Pow => Differentiator::diff_binary_pow(op, lhs, rhs, var),
        }
    }

    fn diff_binary_add(lhs: &Expr, rhs: &Expr, var: &str) -> Result<Expr, String> {
        let lhs = Differentiator::diff(lhs, var)?;
        let rhs = Differentiator::diff(rhs, var)?;
        Ok(Expr::Binary(BinaryOp::Add, Box::new(lhs), Box::new(rhs)))
    }

    fn diff_binary_sub(lhs: &Expr, rhs: &Expr, var: &str) -> Result<Expr, String> {
        let lhs = Differentiator::diff(lhs, var)?;
        let rhs = Differentiator::diff(rhs, var)?;
        Ok(Expr::Binary(BinaryOp::Sub, Box::new(lhs), Box::new(rhs)))
    }

    fn diff_binary_mul(lhs: &Expr, rhs: &Expr, var: &str) -> Result<Expr, String> {
        // product rule: (uv)' = u'v + uv'
        let df = Differentiator::diff(lhs, var)?;
        let dh = Differentiator::diff(rhs, var)?;
        let xlhs = Expr::Binary(BinaryOp::Mul, Box::new(df.clone()), Box::new(rhs.clone()));
        let xrhs = Expr::Binary(BinaryOp::Mul, Box::new(lhs.clone()), Box::new(dh.clone()));

        Ok(Expr::Binary(BinaryOp::Add, Box::new(xlhs), Box::new(xrhs)))
    }

    fn diff_binary_div(op: BinaryOp, lhs: &Expr, rhs: &Expr, var: &str) -> Result<Expr, String> {
        // quotient rule: (u/v)' = (u'v - uv')/v^2
        let uprime = Differentiator::diff(lhs, var)?;
        let vprime = Differentiator::diff(rhs, var)?;
        let uprime_v = Expr::Binary(BinaryOp::Mul, Box::new(uprime.clone()), Box::new(rhs.clone()));
        let u_vprime = Expr::Binary(BinaryOp::Mul, Box::new(lhs.clone()), Box::new(vprime.clone()));
        let vsquare = Expr::Binary(BinaryOp::Pow, Box::new(rhs.clone()), Box::new(Expr::Num(2.0)));
        let numertor = Expr::Binary(BinaryOp::Sub, Box::new(uprime_v), Box::new(u_vprime));
        let ans = Expr::Binary(BinaryOp::Div, Box::new(numertor), Box::new(vsquare));
        Ok(ans)
    }

    fn diff_binary_pow(op: BinaryOp, base: &Expr, expo: &Expr, var: &str) -> Result<Expr, String> {
        match (base, expo) {
            (Expr::Var(name), Expr::Num(n)) if name == var => {
                // d/dx x^n = n x^(n-1)
                let m = *n - 1.0;
                let rhs = Expr::Binary(BinaryOp::Pow, Box::new(Expr::Var(name.clone())), Box::new(Expr::Num(m)));
                let ans = Expr::Binary(BinaryOp::Mul, Box::new(Expr::Num(*n)), Box::new(rhs));
                Ok(ans)
            }
            _ => {
                // general case: (f^g)' = f^g (g' ln f + g f'/f)
                let f = base;
                let g = expo;
                let df = Differentiator::diff(f, var)?;
                let dg = Differentiator::diff(g, var)?;
                let f_pow_g = Expr::Binary(BinaryOp::Pow, Box::new(f.clone()), Box::new(g.clone()));
                let dg_ln_f = Expr::Binary(
                    BinaryOp::Mul,
                    Box::new(dg),
                    Box::new(Expr::Call("ln".into(), vec![(*f).clone()]))
                );
                let g_df_by_f = Expr::Binary(
                    BinaryOp::Mul,
                    Box::new((*g).clone()),
                    Box::new(
                        Expr::Binary(
                            BinaryOp::Div,
                            Box::new(df),
                            Box::new((*f).clone())
                        )
                    )
                );
                let ans = Expr::Binary(
                    BinaryOp::Mul,
                    Box::new(f_pow_g),
                    Box::new(Expr::Binary(
                        BinaryOp::Add,
                        Box::new(dg_ln_f),
                        Box::new(g_df_by_f)
                    ))
                );
                Ok(ans)
            }
        }
    }


    fn diff_function_call(name: &str, args: Vec<Expr>, var: &str) -> Result<Expr, String> {
        match name {
            "exp" => {
                // (exp(f))' = f' exp(f)
                let f = args[0].clone();
                let df = Differentiator::diff(&f, var)?;
                let expf = Expr::Call("exp".into(), vec![f.clone()]);
                let ans = Expr::Binary(
                    BinaryOp::Mul, Box::new(df), Box::new(expf)
                );
                Ok(ans)
            }

            "sin" => {
                let f = args[0].clone();
                let df = Differentiator::diff(&f, var)?;
                let cos = Expr::Call("cos".into(), vec![f.clone()]);
                let ans = Expr::Binary(
                    BinaryOp::Mul, Box::new(df), Box::new(cos)
                );
                Ok(ans)
            }

            "cos" => {
                let f = args[0].clone();
                let df = Differentiator::diff(&f, var)?;
                let sin = Expr::Call("sin".into(), vec![f.clone()]);
                let ans = Expr::Unary(
                    UnaryOp::Neg,
                    Box::new(Expr::Binary(BinaryOp::Mul, Box::new(df), Box::new(sin)))
                );
                Ok(ans)
            }
            _ => {
                // generic: (f(g))' = f'(g) * g'
                Ok(Expr::Call(format!("d_{}{}", name, var), args.clone()))
            }
        }
    }
}