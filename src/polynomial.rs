use std::collections::BTreeMap;
use std::ops::{Add, Sub, Mul};

use crate::Expr;

#[derive(Clone, Debug)]
pub struct Polynomial{
    pub var: String,
    pub coeffs: BTreeMap<i32, (i64, i64)>, // (num, den)
}

fn gcd(m: i64, n: i64) -> i64 {
    if n == 0 { m.abs()} else{ gcd(n, m % n) }
}

fn lcm(m: i64, n: i64) -> i64{
    (m / gcd(m, n)) * n
}

fn divisors(n: i64) -> Vec<i64>{
    todo!("")
}


// -
impl Polynomial{
    pub fn zero(var: &str) -> Self {
        todo!("")
    }

    pub fn one(var: &str) -> Self {
        todo!("")
    }

    pub fn from_int(n: i64, var: &str) -> Polynomial {
        todo!("")
    }

    pub fn rational_roots(&self) -> Vec<f64> {
        todo!("")
    }

    pub fn eval(&self, x: f64) -> f64 {
        todo!("");
    }

    pub fn factor(&self) -> Vec<Polynomial> {
        todo!("")
    }

    fn synthetic_div(&self, r: f64) -> Polynomial {
        todo!("")
    }

    pub fn to_expr(&self) -> Expr {
        todo!("")
    }

    pub fn reduce(num: i64, den: i64) -> (i64, i64) {
        let g = gcd(num, den);
        let mut n = num / g;
        let mut d = den / g;
        if d < 0 {
            d = -d;
            n = -n;
        }
        (n, d)
    }
}

// -
impl Add for Polynomial {
    type Output = Polynomial;

    fn add(self, rhs: Self) -> Self::Output {
        todo!("")
    }
}

impl Sub for Polynomial{
    type Output = Polynomial;

    fn sub(self, rhs: Self) -> Self::Output {
        todo!("")
    }
}

impl Mul for Polynomial{
    type Output = Polynomial;

    fn mul(self, rhs: Self) -> Self::Output {
        todo!("")
    }
}