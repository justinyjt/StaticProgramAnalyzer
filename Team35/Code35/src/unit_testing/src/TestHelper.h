#pragma once

#include <functional>

#include "catch.hpp"

void requireTrue(bool b);
void requireFalse(bool b);
void requireNotThrow(const std::function<void()> &f);
void requireThrow(const std::function<void()> &f);

template<typename T>
void requireEqual(T a, T b) {
    REQUIRE(a == b);
}

template<typename T>
void requireNotEqual(T a, T b) {
    REQUIRE(a != b);
}
