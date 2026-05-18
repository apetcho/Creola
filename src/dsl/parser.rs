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
    pub fn new(input: &str) -> Result<Self, String>{
        let mut lexer = Lexer::new(input);
        Ok(Parser{
            tokens: lexer.tokenize()?,
            pos: 0,
        })
    }

    fn peek(&self) -> Option<&Token> {
        self.tokens.get(self.pos)
    }

    fn next(&mut self) -> Option<&Token> {
        if self.pos < self.tokens.len() {
            self.pos += 1;
        }
        self.tokens.get(self.pos - 1)
    }

    fn consume(&mut self, expected: &Token) -> bool {
        if let Some(token) = self.peek() {
            if token == expected {
                true
            }else{
                false
            }
        }else{
            false
        }
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