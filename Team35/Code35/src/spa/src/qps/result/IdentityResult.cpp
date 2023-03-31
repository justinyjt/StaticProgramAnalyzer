#include "IdentityResult.h"

#include <memory>
#include <utility>

IdentityResult::IdentityResult() : Result(Tag::IDENTITY) {}

std::unique_ptr<Result> IdentityResult::join(Result &rhs) {
    throw std::runtime_error("Identity join should not be called");
}

void IdentityResult::output(std::list<std::string> &list) {}

bool IdentityResult::empty() const {
    return false;
}
