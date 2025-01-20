#pragma once

#include <tuple>

template<typename Tuple>
std::tuple<> subtuple(Tuple& t) {
    return std::make_tuple();
}

template<size_t firstIdx, size_t... tailIdxs, typename Tuple>
auto subtuple(const Tuple& t) {
    return std::tuple_cat(std::make_tuple(std::get<firstIdx>(t)), subtuple<tailIdxs...>(t));
}