#pragma once

#include <functional>

#include "catch.hpp"

void RequireTrue(bool b);
void RequireFalse(bool b);
void RequireNotThrow(const std::function<void()> &f);
void RequireThrow(const std::function<void()> &f);

template<typename T>
void RequireEqual(T a, T b) {
    REQUIRE(a == b);
}

template<typename T>
void RequireNotEqual(T a, T b) {
    REQUIRE(a != b);
}
