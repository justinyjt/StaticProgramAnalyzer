#include "BoolResult.h"

BoolResult::BoolResult(bool b) : Result(Tag::BOOL), b(b) {}

void BoolResult::output(std::list<std::string>& list) {
  list.push_back(b ? "true" : "false");
}

bool BoolResult::operator==(const Result& rhs) const {
    const BoolResult* pRhs = dynamic_cast<const BoolResult*>(&rhs);
    if (pRhs != nullptr) {
        return equal(*pRhs) && b == pRhs->b;
    }
    return false;
}