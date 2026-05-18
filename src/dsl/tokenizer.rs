
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
        Lexer {
            chars: input.chars().collect(),
            pos: 0,
        }
    }

    pub fn tokenize(&mut self) -> Result<Vec<Token>, String>{
        let mut tokens = Vec::new();
        loop{
            let token = self.next_token()?;
            if token == Token::Eof {
                tokens.push(token);
                break;
            }
            tokens.push(token);
        }

        Ok(tokens)
    }

    fn peek(&self) -> Option<char> {
        self.chars.get(self.pos).copied()
    }

    fn next(&mut self) -> Option<char> {
        if self.pos >= self.chars.len() {
            None
        }else{
            let c = self.chars[self.pos];
            self.pos += 1;
            Some(c)
        }
    }

    fn skip_ws(&mut self) {
        while matches!(self.peek(), Some(c) if c.is_whitespace()){
            self.next();
        }
    }

    fn read_number(&mut self, first: char) -> Result<Token, String> {
        let mut numstr = String::new();
        numstr.push(first);
        let mut has_dot = first == '.';
        while let Some(c) = self.peek() {
            if c.is_ascii_digit(){
                numstr.push(c);
                self.next();
            }else if c == '.' && !has_dot {
                has_dot = true;
                numstr.push(c);
                self.next();
            }else{
                break;
            }
        }
        if let Ok(num) = numstr.parse::<f64>() {
            Ok(Token::Num(num))
        }else{
            Err(format!("Invalid number found: '{}'", numstr))
        }
    }

    fn read_ident(&mut self, first: char) -> Token {
        let mut ident = String::new();
        ident.push(first);
        while let Some(c) = self.peek() {
            if c.is_alphanumeric() || c == '_'{
                ident.push(c);
                self.next();
            }else{
                break;
            }
        }

        match ident.as_str(){
            "let" => Token::Let,
            "fun" => Token::Fun,
            _ => Token::Ident(ident),
        }
    }

    fn next_token(&mut self) -> Result<Token, String> {
        self.skip_ws();
        match self.next() {
            None => Ok(Token::Eof),
            Some(c) => match c {
                '+' => Ok(Token::Plus),
                '-' => {
                    if self.peek() == Some('>') {
                        self.next();
                        Ok(Token::Arrow)
                    }else{
                        Ok(Token::Minus)
                    }
                }
                '*' => Ok(Token::Star),
                '/' => Ok(Token::Slash),
                '^' => Ok(Token::Caret),
                '(' => Ok(Token::LParen),
                ')' => Ok(Token::RParen),
                ',' => Ok(Token::Comma),
                '=' => Ok(Token::Equal),
                c if c.is_ascii_digit() || c == '.' => self.read_number(c),
                c if c.is_alphabetic() => Ok(self.read_ident(c)),
                _ => Ok(Token::Eof),
            }
        }
    }
}