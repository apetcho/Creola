use std::collections::HashMap;

mod expr;
pub use expr::{Expr, UnaryOp, BinaryOp};

mod lexer;
pub use lexer::{Token, Lexer};

mod parser;
pub use parser::{Stmt, Parser};

mod evaluator;
use evaluator::Evaluator;

// ===================
// -*- Environment -*-
// ===================
#[derive(Clone)]
pub struct FunctionDef{
    pub params: Vec<String>,
    pub body: Expr,
}

#[derive(Clone, Default)]
pub struct Env{
    pub vars: HashMap<String, Expr>,
    pub funcs: HashMap<String, FunctionDef>,
}

// -*-
pub fn eval(expr: &Expr, env: &Env) -> Expr {
    Evaluator::eval(expr, env)
}