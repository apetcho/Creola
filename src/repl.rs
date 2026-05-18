use rustyline::error::ReadlineError;
use rustyline::{Editor, Helper, Context};
use rustyline::completion::Completer;
use rustyline::hint::Hinter;
use rustyline::highlight::{Highlighter, MatchingBracketHighlighter};
use rustyline::validate::Validator;

use crate::dsl::{Env, Expr, Parser, Stmt, FunctionDef};
use crate::Creola;

struct CreolaHelper{
    bracket: MatchingBracketHighlighter,
}

impl Helper for CreolaHelper{}
impl Completer for CreolaHelper{
    type Candidate = String;
}

impl Hinter for CreolaHelper{
    type Hint = String;
}

impl Highlighter for CreolaHelper{
    fn highlight<'l>(&self, line: &'l str, pos: usize) -> std::borrow::Cow<'l, str> {
        // Very minimal: just highlight "let" and "fun"
        if line.trim_start().starts_with("let") || line.trim_start().starts_with("fun"){
            std::borrow::Cow::Owned(format!("\x1b[1;34m{}\x1b[0m", line))
        }else{
            self.bracket.highlight(line, pos)
        }
    }
}

impl Validator for CreolaHelper{}

impl CreolaHelper{
    pub fn new() -> Self {
        CreolaHelper { bracket: MatchingBracketHighlighter::new() }
    }
}

// -
pub struct Repl{
    env: Env,
}

impl Repl{
    pub fn new() -> Self {
        Repl { env: Env::default() }
    }

    pub fn run(&mut self) {
        let mut rl = Editor::new().expect("failed to create line editor");
        rl.set_helper(Some(CreolaHelper::new()));
        let mut id: usize = 1;

        self.info();

        loop{
            id += 1;
            let prompt = format!("\x1b[32mcreola\x1b[0m:\x1b[33m{}\x1b[0m>> ", id);
            let readline = rl.readline(prompt.as_str());
            match readline {
                Ok(line) => {
                    let mut line = line.trim();
                    if line.is_empty(){ continue; }
                    rl.add_history_entry(line);

                    // Special commands: diff(...), integrate(...) etc. via DSL
                    if let Some(ans) = self.handle_builtin_commands(&mut line){
                        println!("{}", ans);
                        continue;
                    }

                    let input = line.clone();
                    let mut parser = match Parser::new(input){
                        Ok(p) => p,
                        Err(msg) => {
                            eprintln!("{}", msg);
                            continue;
                        }
                    };

                    match parser.parse() {
                        Ok(stmt) => match stmt {
                            Stmt::Let(name, expr) => {
                                let val = match Creola::eval(&expr, &self.env){
                                    Ok(ans) => ans,
                                    Err(msg) => {
                                        eprintln!("{}", msg);
                                        continue;
                                    }
                                };
                                self.env.vars.insert(name.clone(), val.clone());
                                println!("{} = {}", name, val);
                            }
                            Stmt::Fun(name, params, body) => {
                                let def = FunctionDef{
                                    params: params.clone(),
                                    body: body.clone(),
                                };
                                self.env.funcs.insert(name.clone(), def.clone());
                                let mut my_params = String::new();
                                my_params.push('(');
                                for (i, param) in params.iter().enumerate(){
                                    if i > 0 {
                                        my_params.push_str(", ");
                                    }
                                    my_params.push_str(param.as_str());
                                }
                                my_params.push(')');
                                println!("{}{} = {}", name, my_params, body);
                            }
                            Stmt::Expr(expr) => {
                                let ans = match Creola::eval(&expr, &self.env){
                                    Ok(e) => e,
                                    Err(msg) => {
                                        eprintln!("{}", msg);
                                        continue;
                                    }
                                };
                                println!("{}", ans);
                            }
                        }
                        Err(msg) => {
                            eprintln!("ParserError: {}", msg);
                        }
                    }
                }
                Err(ReadlineError::Interrupted) => { continue; }
                Err(ReadlineError::Eof) => {
                    println!("\nBye");
                    break;
                }
                Err(err) => {
                    eprintln!("Error: {err:?}");
                    break;
                }
            }
        }
    }

    fn info(&self) {
        eprintln!("Creola COMPUTER ALGEBRA SYSTEM Version 0.0.1");
        eprintln!("Example Creola session:\n");
        eprintln!("creola:1>> let x = 3.14");
        eprintln!("x = 3.14");
        eprintln!("creola:2>> fun f = (x) -> x^2 + 1");
        eprintln!("f(x) = x^2 + 1");
        eprintln!("creola:3>> diff(\"x^2 + 1\", \"x\")");
        eprintln!("x");
        eprintln!("creola:4>> integrate(\"2x\", \"x\")");
        eprintln!("x^2");
    }

    fn handle_builtin_commands(&mut self, input: &str) -> Option<String> {
        todo!("")
    }

    fn parse_two_args(s: &str) -> Result<Option<(Expr, String)>, String>{
        let first = match s.find('('){
            Some(n) => n,
            None => {
                return Ok(None);
            }
        };
        let last = match s.rfind(')'){
            Some(n) => n,
            None => {
                return Ok(None);
            }
        };
        let inner = &s[&first+1..last];
        let mut depth = 0;
        let mut split_pos = None;
        for (i, c) in inner.char_indices(){
            match c {
                '(' => depth += 1,
                ')' => depth -= 1,
                ',' if depth == 0 => {
                    split_pos = Some(i);
                    break;
                }
                _ => {}
            }
        }

        let split = match split_pos{
            Some(v) => v,
            None => {
                return Ok(None);
            }
        };
        let expr_str = inner[..split].trim();
        let var = inner[split+1..].trim();
        let mut parser = match Parser::new(expr_str){
            Ok(p) => p,
            Err(msg) => {
                return Err(msg);
            }
        };
        let expr = match parser.parse_expr(){
            Ok(e) => e,
            Err(msg) => {
                return Err(msg);
            }
        };

        Ok(Some((expr, var.to_string())))
    }

    fn handle_diff(&mut self, input: &str) -> Result<Option<String>, String> {
        match Repl::parse_two_args(input){
            Ok(Some((expr, var))) => {
                let diff = match Creola::diff(&expr, var.as_str()){
                    Ok(ans) => ans,
                    Err(msg) => {
                        return Err(msg);
                    }
                };
                Ok(Some(format!("{}", diff)))
            }
            Ok(None) => {
                return Err("ill-formed 'diff' expression".into());
            }
            Err(msg) => {
                return Err(msg);
            }
        }
    }

    fn handle_integrate(&mut self, input: &str) -> Result<Option<String>, String> {
        match Repl::parse_two_args(input){
            Ok(Some((expr, var))) => {
                let diff = match Creola::integrate(&expr, var.as_str()){
                    Ok(ans) => ans,
                    Err(msg) => {
                        return Err(msg);
                    }
                };
                Ok(Some(format!("{}", diff)))
            }
            Ok(None) => {
                return Err("ill-formed 'integrate' expression".into());
            }
            Err(msg) => {
                return Err(msg);
            }
        }
    }

    fn handle_taylor(&mut self, input: &str) -> Result<Option<String>, String> {
        // taylor(expr, x, 5)
        let first = match input.find('('){
            Some(p) => p,
            None => {
                return Ok(None);
            }
        };
        let last = match input.rfind(')'){
            Some(p) => p,
            None => {
                return Ok(None);
            }
        };

        let inner = &input[first+1..last];
        let parts: Vec<&str> = inner.split(',').map(|s| s.trim()).collect();
        if parts.len() == 3{
            let expr_str = parts[0];
            let var = parts[1];
            let order: usize = match parts[2].parse(){
                Ok(n) => n,
                Err(err) => {
                    return Err(format!("{:?}", err));
                }
            };

            let mut parser = match Parser::new(expr_str){
                Ok(p) => p,
                Err(msg) => {
                    return Err(msg);
                }
            };

            let expr = match parser.parse_expr(){
                Ok(e) => e,
                Err(msg) => {
                    return Err(msg);
                }
            };

            let expr = match Creola::taylor(&expr, var, order){
                Ok(e) => e,
                Err(msg) => {
                    return Err(msg);
                }
            };

            return Ok(Some(format!("{}", expr)));
        }
        Err("ill-format 'taylor' expression".into())
    }

    fn handle_roots(&mut self, input: &str) -> Result<Option<String>, String> {
        // roots(expr, x, guess)
        let first = match input.find('('){
            Some(p) => p,
            None => {
                return Ok(None);
            }
        };
        let last = match input.rfind(')'){
            Some(p) => p,
            None => {
                return Ok(None);
            }
        };

        let inner = &input[first+1..last];
        let parts: Vec<&str> = inner.split(',').map(|s| s.trim()).collect();
        if parts.len() == 3 {
            let expr_str = parts[0];
            let var = parts[1];
            let guess: f64 = match parts[2].parse(){
                Ok(v) => v,
                Err(err) => {
                    return Err(format!("{:?}", err));
                }
            };

            let mut parser = match Parser::new(expr_str){
                Ok(p) => p,
                Err(msg) => {
                    return Err(msg);
                }
            };
            let expr = match parser.parse_expr(){
                Ok(e) => e,
                Err(msg) => {
                    return Err(msg);
                }
            };
            let ans = match Creola::roots(&expr, var, guess){
                Ok(e) => e,
                Err(msg) => {
                    return Err(msg);
                }
            };

            return Ok(Some(format!("{}", ans)));
        }
        Err("No root found".into())
    }

    fn handle_factor(&mut self, input: &str) -> Result<Option<String>, String> {
        // factor(expr)
        let first = match input.find('('){
            Some(p) => p,
            None => {
                return Ok(None);
            }
        };
        let last = match input.rfind(')'){
            Some(p) => p,
            None => {
                return Ok(None);
            }
        };
        let inner = &input[first+1..last];
        let mut parser = match Parser::new(inner){
            Ok(p) => p,
            Err(msg) => {
                return Err(msg);
            }
        };
        let expr = match parser.parse_expr(){
            Ok(e) => e,
            Err(msg) => {
                return Err(msg);
            }
        };
        let ans = match Creola::factor(&expr){
            Ok(e) => e,
            Err(msg) => {
                return Err(msg);
            }
        };

        Ok(Some(format!("{}", ans)))
    }

    fn handle_expand(&mut self, input: &str) -> Result<Option<String>, String> {
        todo!("")
    }

    fn handle_simplify(&mut self, input: &str) -> Result<Option<String>, String> {
        todo!("")
    }
}