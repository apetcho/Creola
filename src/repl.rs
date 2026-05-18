use rustyline::error::ReadlineError;
use rustyline::{Editor, Helper, Context};
use rustyline::completion::Completer;
use rustyline::hint::Hinter;
use rustyline::highlight::{Highlighter, MatchingBracketHighlighter};
use rustyline::validate::Validator;

use crate::dsl::{Env, Expr};
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

        self.info();
    }

    fn info(&self) {
        eprintln!("Creola COMPUTER ALGEBRA SYSTEM Version 0.0.1");
        eprintln!("Example Creola session:\n");
        eprintln!(" creola:1>> let x = 3.14");
        eprintln!(" creola:2>> fun f = (x) -> x^2 + 1");
        eprintln!(" creola:3>> diff(\"x^2 + 1\", \"x\")");
        eprintln!(" x");
        eprintln!(" creola:4>> integrate(\"2x\", \"x\")");
        eprintln!(" x^2");
    }

    fn handle_builtin_commands(&mut self, input: &str) -> Option<String> {
        todo!("")
    }

    fn parse_two_args(s: &str) -> Option<(Expr, String)> {
        todo!("")
    }

    fn handle_diff(&mut self, input: &str) -> Option<String> {
        todo!("")
    }

    fn handle_integrate(&mut self, input: &str) -> Option<String> {
        todo!("")
    }

    fn handle_taylor(&mut self, input: &str) -> Option<String> {
        todo!("")
    }

    fn handle_roots(&mut self, input: &str) -> Option<String> {
        todo!("")
    }

    fn handle_factor(&mut self, input: &str) -> Option<String> {
        todo!("")
    }

    fn handle_expand(&mut self, input: &str) -> Option<String> {
        todo!("")
    }

    fn handle_simplify(&mut self, input: &str) -> Option<String> {
        todo!("")
    }
}