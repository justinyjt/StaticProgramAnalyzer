#include "BoolResult.h"

BoolResult::BoolResult(bool b) : Result(Tag::BOOL), b(b) {}

void BoolResult::output(std::list<std::string>& list, std::string& selected) {
}

bool BoolResult::operator==(const Result& rhs) const {
    const BoolResult* pRhs = dynamic_cast<const BoolResult*>(&rhs);
    return pRhs != nullptr && b == pRhs->b;
}
