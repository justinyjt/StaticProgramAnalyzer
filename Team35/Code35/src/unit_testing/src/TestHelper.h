#pragma once

#include <functional>
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

template<typename E>
void requireThrowAs(const std::function<void()> &f) {
    REQUIRE_THROWS_AS(f(), E);
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

template<typename T>
inline void requireNotEqualRef(const T &a, const T &b) {
    REQUIRE(a != b);
}

template<typename T>
inline bool hasIntersection(const T &a, const T &b) {
    for (auto &a_element : a) {
        for (auto &b_element : b) {
            if (a_element == b_element) {
                return true;
            }
        }
    }
    return false;
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
