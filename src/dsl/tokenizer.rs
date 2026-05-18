
#[derive(Debug, Clone, PartialEq)]
pub enum Token {
    Num(f64),
    Ident(String),
    Plus,
    Minus,
    Star,
    Slash,
    Caret,
    LParen,
    RParen,
    Comma,
    Equal,
    Arrow,
    Let,
    Fun,
    Eof,
}


// -
#[derive(Debug)]
pub struct Lexer{
    chars: Vec<char>,
    pos: usize,
}

impl Lexer{
    pub fn new(input: &str) -> Self{
        todo!("")
    }

    pub fn tokenize(&mut self) -> Vec<Token>{
        todo!("")
    }

    fn peek(&self) -> Option<char> {
        todo!("")
    }

    fn next(&mut self) -> Option<char> {
        todo!("")
    }

    fn skip_ws(&mut self) {
        todo!("")
    }

    fn read_number(&mut self, first: char) -> Token {
        todo!("")
    }

    fn read_ident(&mut self, first: char) -> Token {
        todo!("")
    }

    fn next_token(&mut self) -> Token {
        todo!("")
    }
}