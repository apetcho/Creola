use std::fmt;

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
        todo!("")
    }
}

impl Expr{
    pub fn format(&self, prec: u8) -> String {
        todo!("")
    }
}