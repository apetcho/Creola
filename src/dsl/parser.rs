use crate::dsl::{Expr, Token, Lexer};

#[derive(Debug)]
pub enum Stmt {
    Let(String, Expr),
    Fun(String, Vec<String>, Expr),
    Expr(Expr),
}

#[derive(Debug)]
pub struct Parser{
    tokens: Vec<Token>,
    pos: usize,
}

impl Parser{
    pub fn new(input: &str) -> Self{
        todo!("")
    }

    fn peek(&self) -> Option<&Token> {
        todo!("")
    }

    fn next(&mut self) -> Option<&Token> {
        todo!("")
    }

    fn consume(&mut self, expected: &Token) -> bool {
        todo!("")
    }

    pub fn parse(&mut self) -> Result<Stmt, String> {
        todo!("")
    }

    fn parse_let(&mut self) -> Result<Stmt, String> {
        todo!("")
    }

    fn parse_fun(&mut self) -> Result<Stmt, String> {
        todo!("")
    }

    fn parse_expr(&mut self) -> Result<Expr, String> {
        todo!("")
    }

    fn parse_add_sub(&mut self) -> Result<Expr, String> {
        todo!("")
    }

    fn parse_mul_div(&mut self) -> Result<Expr, String> {
        todo!("")
    }

    fn parse_pow(&mut self) -> Result<Expr, String> {
        todo!("")
    }

    fn parse_unary(&mut self) -> Result<Expr, String> {
        todo!("")
    }

    fn parse_primary(&mut self) -> Result<Expr, String> {
        todo!("")
    }
}