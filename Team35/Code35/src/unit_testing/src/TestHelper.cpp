#include "TestHelper.h"

void RequireTrue(bool b) {
    REQUIRE(b);
}

void RequireFalse(bool b) {
    REQUIRE(!b);
}

void RequireNotThrow(const std::function<void()> &f) {
    REQUIRE_NOTHROW(f());
}

void RequireThrow(const std::function<void()> &f) {
    REQUIRE_THROWS(f());
}
