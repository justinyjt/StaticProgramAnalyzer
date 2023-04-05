#pragma once

#include <functional>
#include <list>
#include <vector>

#include "catch.hpp"

inline void requireTrue(bool b) {
    REQUIRE(b);
}

inline void requireFalse(bool b) {
    REQUIRE(!b);
}

inline void requireNotThrow(const std::function<void()> &f) {
    REQUIRE_NOTHROW(f());
}

inline void requireThrow(const std::function<void()> &f) {
    REQUIRE_THROWS(f());
}

template<typename T>
inline void requireEqual(T a, T b) {
    REQUIRE(a == b);
}

template<typename T>
inline void requireEqualRef(const T &a, const T &b) {
    REQUIRE(a == b);
}

template<typename T>
inline void requireNotEqual(T a, T b) {
    REQUIRE(a != b);
}

/**
 * [1,2,2,2,2]
 * [1,1,1,2,2]
 *
 * @tparam T
 * @param a
 * @param b
 */
template<typename T>
inline void requireSameListNoOrder(const std::list<T> &a, const std::list<T> &b) {
    requireEqual(a.size(), b.size());
    for (auto &a_element : a) {
        requireTrue(std::find(b.begin(), b.end(), a_element) != b.end());
    }
    for (auto &b_element : b) {
        requireTrue(std::find(a.begin(), a.end(), b_element) != a.end());
    }
}

template<typename T>
inline void requireSameVectorNoOrder(const std::vector<T> &a, const std::vector<T> &b) {
    requireEqual(a.size(), b.size());
    for (auto &a_element : a) {
        requireTrue(std::find(b.begin(), b.end(), a_element) != b.end());
    }
    for (auto &b_element : b) {
        requireTrue(std::find(a.begin(), a.end(), b_element) != a.end());
    }
}
