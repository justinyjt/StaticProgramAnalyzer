#include "BoolResult.h"

BoolResult::BoolResult(bool b) : Result(Tag::BOOL), b(b) {
    isSelectBool = false;
}

BoolResult::BoolResult(bool b, bool isSelectBool) : Result(Tag::BOOL), b(b), isSelectBool(isSelectBool) {}

std::unique_ptr<Result> BoolResult::join(Result &rhs) {
    if (!b) {
        return std::make_unique<BoolResult>(std::move(*this));
    }
    if (rhs.tag == Tag::BOOL) {
        BoolResult &boolResult = dynamic_cast<BoolResult &>(rhs);
        return std::make_unique<BoolResult>(this->b && boolResult.b);
    } else {
        if (isSelectBool) {
            TableResult &tableResult = dynamic_cast<TableResult &>(rhs);
            return std::make_unique<BoolResult>(BoolResult(!tableResult.rows.empty()));
        } else {
            TableResult &tableResult = dynamic_cast<TableResult &>(rhs);
            return std::make_unique<TableResult>(tableResult.idents, tableResult.rows);
        }
    }
}

void BoolResult::output(std::list<std::string>& list) {
    list.emplace_back(b ? "TRUE" : "FALSE");
}

bool BoolResult::operator==(const Result& rhs) const {
    const BoolResult* pRhs = dynamic_cast<const BoolResult*>(&rhs);
    return pRhs != nullptr && b == pRhs->b;
}
