use crate::dsl::{Expr, UnaryOp, BinaryOp};

pub struct Integrator;

impl Integrator{
    pub fn integrate(expr: &Expr, var: &str) -> Result<Expr, String> {
        match expr {
            Expr::Num(c) => {
                let ans = Expr::Binary(
                    BinaryOp::Mul,
                    Box::new(Expr::Num(*c)),
                    Box::new(Expr::Var(var.to_string()))
                );
                Ok(ans)
            }

            Expr::Var(x) if x == var => {
                // ∫ x dx = x^2 / 2
                let xsquare = Expr::Binary(
                    BinaryOp::Pow,
                    Box::new(Expr::Var(var.to_string())),
                    Box::new(Expr::Num(2.0))
                );
                let ans = Expr::Binary(
                    BinaryOp::Div, Box::new(xsquare), Box::new(Expr::Num(2.0))
                );
                Ok(ans)
            }
            Expr::Var(sym) => {
                // general case: ∫ y x dx = y x^2/2
                let xsquare = Expr::Binary(
                    BinaryOp::Pow,
                    Box::new(Expr::Var(var.to_string())),
                    Box::new(Expr::Num(2.0))
                );
                let xsquare_by_two = Expr::Binary(
                    BinaryOp::Div, Box::new(xsquare), Box::new(Expr::Num(2.0))
                );
                let ans = Expr::Binary(
                    BinaryOp::Mul,
                    Box::new(Expr::Var(sym.clone())),
                    Box::new(xsquare_by_two)
                );
                Ok(ans)
            }
            Expr::Unary(_, rhs) => Integrator::integrate_unary(rhs, var),
            Expr::Binary(op, lhs, rhs) => {
                Integrator::integrate_binary(op.clone(), lhs, rhs, var)
            }
            Expr::Call(name, args) => {
                Integrator::integrate_func(name, args.clone(), var)
            }
        }
    }

    fn integrate_unary(expr: &Expr, var: &str) -> Result<Expr, String> {
        let ans = Integrator::integrate(expr, var)?;
        Ok(Expr::Unary(UnaryOp::Neg, Box::new(ans)))
    }

    fn integrate_binary(op: BinaryOp, lhs: &Expr, rhs: &Expr, var: &str) -> Result<Expr, String> {
        todo!("")
    }

    fn integrate_binary_add(lhs: &Expr, rhs: &Expr, var: &str) -> Result<Expr, String> {
        todo!("")
    }

    fn integrate_binary_sub(lhs: &Expr, rhs: &Expr, var: &str) -> Result<Expr, String> {
        todo!("")
    }

    fn integrate_binary_mul(lhs: &Expr, rhs: &Expr, var: &str) -> Result<Expr, String> {
        todo!("")
    }

    fn integrate_binary_div(lhs: &Expr, rhs: &Expr, var: &str) -> Result<Expr, String> {
        todo!("")
    }

    fn integrate_binary_pow(lhs: &Expr, rhs: &Expr, var: &str) -> Result<Expr, String> {
        todo!("")
    }

    fn integrate_func(name: &str, args: Vec<Expr>, var: &str) -> Result<Expr, String> {
        todo!("")
    }
}