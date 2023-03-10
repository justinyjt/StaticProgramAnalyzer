#pragma once

#include <functional>

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
inline void requireEqualRef(T &a, T &b) {
    REQUIRE(a == b);
}

template<typename T>
inline void requireNotEqual(T a, T b) {
    REQUIRE(a != b);
}
