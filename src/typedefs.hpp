#pragma once

#include<memory>
#include<cstdint>
#include<complex>

// ----------------------------------------------------------------------------
// begin::namespace::creola
// ----------------------------------------------------------------------------
namespace creola{
// -

struct Node;

using i64 = std::int64_t;
using i32 = std::int32_t;
using f64 = double;
using Expr = std::shared_ptr<const Node>;
using Complex = std::complex<f64>;


// ----------------------------------------------------------------------------
}// end::namespace::creola
// ----------------------------------------------------------------------------