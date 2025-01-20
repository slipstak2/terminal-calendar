#pragma once

#include <iostream>
#include <tuple>

template<typename Tuple, size_t N = 0>
void print_tuple(const Tuple& t, std::ostream& stream = std::cout) {
    if constexpr (std::tuple_size<Tuple>() <= N) {
        return;
    }
    else {
        stream << std::get<N>(t) << " ";
        print_tuple<Tuple, N + 1>(t, stream);
    }
}


template<class Tuple, size_t... I>
void print_tuple_pretty(const Tuple& tup, std::index_sequence<I...>, std::ostream& stream = std::cout) {
    stream << "(";
    (..., (stream << (I == 0 ? "" : ", ") << std::get<I>(tup))); // fold expression
    stream << ")";
}

template<class... T>
void print_tuple_pretty(const std::tuple<T...>& tup, std::ostream& s = std::cout) {
    print_tuple_pretty(tup, std::make_index_sequence<sizeof ...(T)>(), s);
}