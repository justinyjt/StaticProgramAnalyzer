#include "IdentityResult.h"

#include <memory>
#include <utility>

IdentityResult::IdentityResult() : Result(Tag::IDENTITY) {
}

std::unique_ptr<Result> IdentityResult::join(Result &rhs) {
}

void IdentityResult::output(std::list<std::string>& list) {
}
