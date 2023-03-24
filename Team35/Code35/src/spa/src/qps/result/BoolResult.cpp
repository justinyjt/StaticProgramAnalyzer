#include "BoolResult.h"
#include "TableResult.h"

BoolResult::BoolResult(bool b) : Result(Tag::BOOL), b(b) {}

std::unique_ptr<Result> BoolResult::getFinalResult(TableResult intermediateResultTable) {
    return std::move(std::make_unique<BoolResult>(intermediateResultTable.rows.size() == 0));
}

void BoolResult::output(std::list<std::string>& list) {
    list.emplace_back(b ? "TRUE" : "FALSE");
}

bool BoolResult::operator==(const Result& rhs) const {
    const BoolResult* pRhs = dynamic_cast<const BoolResult*>(&rhs);
    return pRhs != nullptr && b == pRhs->b;
}
