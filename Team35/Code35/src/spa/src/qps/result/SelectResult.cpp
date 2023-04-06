#include "SelectResult.h"

#include <cstdint>
#include <memory>
#include <utility>

#include "BoolResult.h"

// general constructor for n-cols
SelectResult::SelectResult(std::vector<std::string> &_idents, const std::vector<TableResult> &_cols) :
        Result(Tag::SELECT) {
    idents.insert(idents.end(), _idents.begin(), _idents.end());
    cols.insert(cols.end(), _cols.begin(), _cols.end());
}

std::vector<uint32_t> SelectResult::getOutputOrder(const TableResult &intermediateRes) const {
    std::vector<uint32_t> order;
    auto &intermediateResIdents = intermediateRes.getTableHeaders();
    for (const auto &ident : idents) {
        for (int j = 0; j < intermediateResIdents.size(); ++j) {
            if (ident == intermediateResIdents.at(j)) {
                order.push_back(j);
            }
        }
    }
    return order;
}

std::unique_ptr<TableResult> SelectResult::getResultForOutput(std::unique_ptr<Result> finalRes) {
    std::unordered_set<std::string> selectIdents(idents.begin(), idents.end());
    std::unique_ptr<TableResult> finalTableRes = (dynamic_cast<TableResult &>(*finalRes)).projectColumns(selectIdents);
    std::vector<uint32_t> order = getOutputOrder(*finalTableRes);
    return std::make_unique<TableResult>(*finalTableRes, order);
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
    auto &intermediateResIdents = t2.getTableHeaders();
    std::unordered_set<std::string> intermediateIdentsSet(intermediateResIdents.begin(), intermediateResIdents.end());
    std::unique_ptr<Result> finalRes = std::make_unique<TableResult>(t2);
    for (int i = 0; i < idents.size(); i++) {
        if (intermediateIdentsSet.find(idents[i]) == intermediateIdentsSet.end()) {  // if non-overlapping
            finalRes = finalRes->join(cols[i]);
        }
    }

    return getResultForOutput(std::move(finalRes));
}

void SelectResult::output(std::list<std::string> &list) const {}

bool SelectResult::operator==(const Result &rhs) const {
    const auto *pRhs = dynamic_cast<const SelectResult *>(&rhs);
    return pRhs != nullptr && idents == pRhs->idents && cols == pRhs->cols;
}

bool SelectResult::isNull() const {
    return false;
}
