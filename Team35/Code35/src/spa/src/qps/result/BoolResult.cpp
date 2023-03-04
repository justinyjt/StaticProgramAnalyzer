#include "BoolResult.h"

BoolResult::BoolResult(bool b) : Result(Tag::BOOL), b(b) {}

void BoolResult::output(std::list<std::string>& list) {
    b ? list.emplace_back("TRUE") : list.emplace_back("FALSE");
}

bool BoolResult::operator==(const Result& rhs) const {
    const BoolResult* pRhs = dynamic_cast<const BoolResult*>(&rhs);
    return pRhs != nullptr && b == pRhs->b;
}
