use crate::dsl::{Expr, BinaryOp};

pub struct Factorizer;

impl Factorizer{
    pub fn factorize(expr: &Expr) -> Result<Expr, String> {
        let var = "x"; // or detect automatically
        if let Some(poly) = expr.to_polynomial(var){
            let factors = poly.factor();
            let mut out = Expr::Num(1.0);
            for factor in factors {
                out = Expr::Binary(
                    BinaryOp::Mul,
                    Box::new(out),
                    Box::new(factor.to_expr())
                );
            }

            Ok(out)
        }else{
            Ok(expr.clone())
        }
    }
}