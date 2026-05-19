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
    let mut divs = Vec::new();
    for i in 1..=n.abs() {
        if n % i == 0 { divs.push(i); }
    }
    divs
}


// -
impl Polynomial{
    pub fn zero(var: &str) -> Self {
        Polynomial { var: var.into(), coeffs: BTreeMap::new() }
    }

    pub fn one(var: &str) -> Self {
        let mut poly = Polynomial::zero(var);
        poly.coeffs.insert(0, (1, 1));
        poly
    }

    pub fn from_int(n: i64, var: &str) -> Polynomial {
        let mut poly = Polynomial::zero(var);
        poly.coeffs.insert(0, (n, 1));
        poly
    }

    pub fn rational_roots(&self) -> Vec<f64> {
        let leading = self.coeffs.iter().rev().next().unwrap().1.0;
        let constant = self.coeffs.get(&0).map(|c| c.0).unwrap_or(0);

        let mut roots = Vec::new();

        let divs_const = divisors(constant);
        let divs_lead = divisors(leading);

        for x in divs_const {
            for y in &divs_lead {
                let candidates = [x as f64 / *y as f64, -(x as f64 / *y as f64)];
                for r in candidates {
                    if self.eval(r).abs() < 1e-9 {
                        roots.push(r);
                    }
                }
            }
        }

        roots
    }

    pub fn eval(&self, x: f64) -> f64 {
        let mut ans = 0.0;
        for (pow, (n, d)) in &self.coeffs {
            ans += (*n as f64 / *d as f64) * x.powi(*pow);
        }

        ans
    }

    pub fn factor(&self) -> Vec<Polynomial> {
        let mut factors = Vec::new();
        let mut current = self.clone();

        loop {
            let roots = current.rational_roots();
            if roots.is_empty(){ break; }

            let r = roots[0];
            let (num, den) = Polynomial::reduce((r * 1_000_000.0) as i64, 1_000_000);

            // factor (x - r)
            let mut lin = Polynomial::zero(&self.var);
            lin.coeffs.insert(1, (1, 1));
            lin.coeffs.insert(0, (-num, den));

            factors.push(lin.clone());
            current = current.synthetic_div(r);
        }

        if !current.coeffs.is_empty() {
            factors.push(current);
        }

        factors
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

    fn add(mut self, rhs: Self) -> Self::Output {
        for (k, (rn, rd)) in rhs.coeffs {
            let (ln, ld) = self.coeffs.get(&k).cloned().unwrap_or((0, 1));
            let lcm_d = lcm(ld, rd);
            let ln2 = ln * (lcm_d/ld);
            let rn2 = rn * (lcm_d/rd);
            let (n, d) = Polynomial::reduce(ln2+rn2, lcm_d);
            if n == 0 { self.coeffs.remove(&k); }
            else{ self.coeffs.insert(k, (n, d)); }
        }
        self
    }
}

impl Sub for Polynomial{
    type Output = Polynomial;

    fn sub(self, rhs: Self) -> Self::Output {
        let var = rhs.clone().var;
        self + rhs * Polynomial::from_int(-1, var.as_str())
    }
}

impl Mul for Polynomial{
    type Output = Polynomial;

    fn mul(self, rhs: Self) -> Self::Output {
        let mut out = Polynomial::zero(&self.var);
        for (xpow, (xn, xd)) in self.coeffs{
            for (ypow, (yn, yd)) in &rhs.coeffs {
                let pow = xpow + ypow;
                let (n, d) = Polynomial::reduce(xn * yn,  xd * yd);
                let (ln, ld) = out.coeffs.get(&pow).cloned().unwrap_or((0, 1));
                let lcm_d = lcm(ld, d);
                let ln2 = ln * (lcm_d/ld);
                let rn2 = n * (lcm_d/d);
                let (n3, d3) = Polynomial::reduce(ln2 + rn2, lcm_d);
                if n3 == 0 { out.coeffs.remove(&pow); }
                else { out.coeffs.insert(pow, (n3, d3)); }
            }
        }
        out
    }
}