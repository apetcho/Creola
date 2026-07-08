#pragma once
#include<memory>
#include<vector>
#include<cstdint>
#include<unordered_map>
#include<map>

#define CREOLA_UNUSED(arg)  (void)arg

// -*----------------------------------------------------------------*-
// -*- begin::namespace::creola                                     -*-
// -*----------------------------------------------------------------*-
namespace creola {
// -

template<typename T>
using Vec = std::vector<T>;

template<typename T>
using Shared = std::shared_ptr<T>;

template<typename Key, typename Value>
using HashMap = std::unordered_map<Key, Value>;

template<typename Key, typename Value>
using Dict = std::map<Key, Value>;

using f64 = double;
using i64 = std::int64_t;
using usize = size_t;


// -*----------------------------------------------------------------*-
}//-*- end::namespace::creola                                       -*-
// -*----------------------------------------------------------------*-