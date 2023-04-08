#include "BoolResult.h"

#include <memory>
#include <utility>

BoolResult::BoolResult(bool b) : Result(Tag::BOOL), b(b) {}

std::unique_ptr<Result> BoolResult::join(Result &rhs) {
    if (rhs.tag == Result::Tag::IDENTITY) {
        return std::make_unique<BoolResult>(b);
    }
    return std::make_unique<BoolResult>(this->b && !rhs.isNull());
}

void BoolResult::output(std::list<std::string> &list) const {
    list.emplace_back(b ? "TRUE" : "FALSE");
}

bool BoolResult::operator==(const Result &rhs) const {
    const auto *pRhs = dynamic_cast<const BoolResult *>(&rhs);
    return pRhs != nullptr && b == pRhs->b;
}

bool BoolResult::isNull() const {
    return !b;
}
