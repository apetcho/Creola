use std::{collections::BTreeMap, fmt};
use crate::Polynomial;

#[derive(Clone, Debug, PartialEq)]
pub enum Expr {
    Num(f64),
    Var(String),
    Unary(UnaryOp, Box<Expr>),
    Binary(BinaryOp, Box<Expr>, Box<Expr>),
    Call(String, Vec<Expr>),
}

#[derive(Clone, Copy, Debug, PartialEq)]
pub enum UnaryOp {
    Neg
}

#[derive(Clone, Copy, Debug, PartialEq)]
pub enum BinaryOp {
    Add,
    Sub,
    Mul,
    Div,
    Pow,
}

// ======================
// -*- Pretty Printer -*-
// ======================
impl fmt::Display for Expr{
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.format(0))
    }
}

impl Expr{
    pub fn format(&self, prec: u8) -> String {
        use Expr::*;
        match self{
            Num(n) => {
                if n.fract() == 0.0 {
                    format!("{}", *n as i64)
                }else{
                    format!("{}", n)
                }
            }

            Var(name) => {
                let greek = match name.as_str(){
                    "alpha" => "α",
                    "beta" => "β",
                    "gamma" => "γ",
                    "delta" => "δ",
                    "lambda" => "λ",
                    "pi" => "π",
                _   => name,
                };
                format!("{}", greek)
            }

            Unary(UnaryOp::Neg, rhs) => format!("-{}", rhs),

            Binary(op, lhs, rhs) => {
                let (p, sym) = match op {
                    BinaryOp::Add => (1, " + "),
                    BinaryOp::Sub => (1, " - "),
                    BinaryOp::Mul => (2, " * "),
                    BinaryOp::Div => (2, " / "),
                    BinaryOp::Pow => (3, "^"),
                };

                let need_paren = p < prec;
                let mut text = String::new();
                if need_paren {
                    text.push_str("(");
                }

                match op{
                    BinaryOp::Div => {
                        text.push_str(format!("{} / {}", lhs, rhs).as_str());
                    }
                    BinaryOp::Pow => {
                        text.push_str(format!("{}^{}", lhs, rhs).as_str());
                    }

                    _ => {
                        text.push_str(format!("{}", lhs.format(p)).as_str());
                        text.push_str(format!("{}", sym).as_str());
                        text.push_str(format!("{}", rhs.format(p)).as_str());
                    }
                }
                if need_paren {
                    text.push_str(")");
                }
                text
            }

            Call(name, args) => {
                let greek = match name.as_str() {
                    "sqrt" => "√",
                    "exp" => "exp",
                    "sin" => "sin",
                    "cos" => "cos",
                    "tan" => "tan",
                    _ => name,
                };
                let mut text = String::new();
                text.push_str(format!("{}(", greek).as_str());
                for (i, arg) in args.iter().enumerate(){
                    if i > 0 {
                        text.push_str(", ");
                    }
                    text.push_str(format!("{}", arg).as_str());
                }
                text.push_str(")");
                text
            }
        }
    }

    // -
    pub fn to_polynomial(&self, var: &str) -> Option<Polynomial> {
        match self {
            Expr::Num(n) => {
                let mut poly = Polynomial{
                    var: var.to_string(),
                    coeffs: BTreeMap::new(),
                };
                let (num, den) = Polynomial::reduce((*n*1_000_000_.0) as i64, 1_000_000);
                poly.coeffs.insert(0, (num, den));
                Some(poly)
            }

            Expr::Var(v) if v == var => {
                let mut poly = Polynomial{
                    var: var.to_string(),
                    coeffs: BTreeMap::new(),
                };
                poly.coeffs.insert(1,(1, 1));
                Some(poly)
            }

            Expr::Binary(op, lhs, rhs) => {
                let xpoly = lhs.to_polynomial(var)?;
                let ypoly = rhs.to_polynomial(var)?;
                match op {
                    BinaryOp::Add => Some(xpoly + ypoly),
                    BinaryOp::Sub => Some(xpoly - ypoly),
                    BinaryOp::Mul => Some(xpoly * ypoly),
                    BinaryOp::Pow => {
                        if let Expr::Num(n) = **rhs{
                            let mut r = Polynomial::one(var);
                            for _ in 0..(n as i32) {
                                r = r * xpoly.clone();
                            }
                            Some(r)
                        }else{ None }
                    }
                    _ => None,
                }
            }
            _ => None,
        }
    }
}