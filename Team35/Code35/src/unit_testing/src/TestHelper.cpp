#include "TestHelper.h"

void requireTrue(bool b) {
    REQUIRE(b);
}

void requireFalse(bool b) {
    REQUIRE(!b);
}

void requireNotThrow(const std::function<void()> &f) {
    REQUIRE_NOTHROW(f());
}

void requireThrow(const std::function<void()> &f) {
    REQUIRE_THROWS(f());
}
