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

    pub fn parse(&mut self) -> Result<Stmt, String> {
        if let Some(token) = self.peek() {
            match token {
                Token::Let => {
                    self.next();
                    self.parse_let()
                }
                Token::Fun => {
                    self.next();
                    self.parse_fun()
                }
                _ => {
                    let expr = self.parse_expr()?;
                    Ok(Stmt::Expr(expr))
                }
            }
        }else{
            Err("unexpected error.".into())
        }
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



    fn parse_let(&mut self) -> Result<Stmt, String> {
        let name = match self.next(){
            Some(Token::Ident(ident)) => ident.clone(),
            _ => {
                return Err("Expected identifier after 'let'".into());
            }
        };
        if !self.consume(&Token::Equal){
            return Err("Expected '=' in let binding".into());
        }

        let expr = self.parse_expr()?;
        Ok(Stmt::Let(name, expr))
    }

    fn parse_fun(&mut self) -> Result<Stmt, String> {
        let name = match self.next(){
            Some(Token::Ident(ident)) => ident.clone(),
            _ => {
                return Err("Expected identifier after 'fun'".into());
            }
        };
        if !self.consume(&Token::Equal) {
            return Err("Expected '=' in fun definition".into());
        }

        // (x, y, ...) -> body
        if !self.consume(&Token::LParen){
            return Err("Expected '(' after 'fun f ='".into());
        }
        let mut params = Vec::new();
        loop{
            match self.next(){
                Some(Token::Ident(id)) => params.push(id.clone()),
                Some(Token::RParen) => { break; },
                Some(Token::Comma) => { continue; },
                _ => {
                    return Err("Invalid parameter list in function definition".into());
                }
            }

            if self.consume(&Token::RParen){
                break;
            }else if self.consume(&Token::Comma){
                continue;
            }
        }

        if !self.consume(&Token::Arrow){
            return Err("Expected '->' in function definition".into());
        }

        let body = self.parse_expr()?;
        Ok(Stmt::Fun(name, params, body))
    }

    fn parse_expr(&mut self) -> Result<Expr, String> {
        self.parse_add_sub()
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