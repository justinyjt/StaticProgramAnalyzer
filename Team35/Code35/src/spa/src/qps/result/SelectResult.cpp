#include "SelectResult.h"

#include <iterator>
#include <memory>
#include <utility>

#include "BoolResult.h"

// general constructor for n-cols
SelectResult::SelectResult(std::vector<std::string> &_idents, const std::vector<TableResult> &_cols) :
    Result(Tag::SELECT) {
    idents.insert(idents.end(), _idents.begin(), _idents.end());
    cols.insert(cols.end(), _cols.begin(), _cols.end());
}

std::vector<int> SelectResult::getOutputOrder(const TableResult &intermediateRes) const {
    std::vector<int> order;
    for (const auto &ident : idents) {
        for (int j = 0; j < intermediateRes.idents.size(); j++) {
            if (ident == intermediateRes.idents[j]) {
                order.push_back(j);
            }
        }
    }
    return order;
}

std::unique_ptr<TableResult> SelectResult::getResultForOutput(std::unique_ptr<Result> finalRes) {
    std::unique_ptr<TableResult> finalTableRes = std::make_unique<TableResult>(dynamic_cast<TableResult &>(*finalRes));
    std::unordered_set<std::string> selectIdents(idents.begin(), idents.end());
    finalTableRes = finalTableRes->projectColumns(selectIdents);
    std::vector<int> order = getOutputOrder(*finalTableRes);
    return std::make_unique<TableResult>(finalTableRes->idents, finalTableRes->rows, order);
}

std::unique_ptr<Result> SelectResult::getColsCrossProduct() {
    std::unique_ptr<Result> finalRes = std::make_unique<TableResult>(cols[0]);
    for (int i = 1; i < cols.size(); i++) {
        finalRes = finalRes->join(cols[i]);
    }
    return getResultForOutput(std::move(finalRes));
}

std::unique_ptr<Result> SelectResult::join(Result &rhs) {
    if (rhs.tag == Result::Tag::IDENTITY) {
        return getColsCrossProduct();
    }
    if (rhs.tag == Tag::BOOL) {
        auto &boolRes = dynamic_cast<BoolResult &>(rhs);
        if (boolRes.b) {
            return getColsCrossProduct();
        }
        return std::make_unique<TableResult>();
    }

    auto &t2 = dynamic_cast<TableResult &>(rhs);
    std::unordered_set<std::string> intermediateIdentsSet(t2.idents.begin(), t2.idents.end());
    std::unique_ptr<Result> finalRes = std::make_unique<TableResult>(TableResult(t2));
    for (int i = 0; i < idents.size(); i++) {
        if (intermediateIdentsSet.find(idents[i]) == intermediateIdentsSet.end()) {  // if non-overlapping
            finalRes = finalRes->join(cols[i]);
        }
    }

    return getResultForOutput(std::move(finalRes));
}

void SelectResult::output(std::list<std::string> &list) {}

bool SelectResult::operator==(const Result &rhs) const {
    const auto *pRhs = dynamic_cast<const SelectResult *>(&rhs);
    return pRhs != nullptr && idents == pRhs->idents && cols == pRhs->cols;
}

bool SelectResult::isNull() const {
    return false;
}
