#include "SelectResult.h"
#include "IdentityResult.h"

#include <iterator>

// general constructor for n-cols
SelectResult::SelectResult(std::vector<std::string> &_idents, const std::vector<TableResult> &_cols) :
        Result(Tag::SELECT) {
    idents.insert(idents.end(), _idents.begin(), _idents.end());
    cols.insert(cols.end(), _cols.begin(), _cols.end());
}

std::unique_ptr<Result> SelectResult::getColsCrossProduct() {
    std::unique_ptr<Result> finalRes = std::make_unique<TableResult>(TableResult(cols[0]));
    std::vector<int> order(1, 0);
    for (int i = 1; i < cols.size(); i++) {
        finalRes = finalRes->join(cols[i]);
        order.push_back(i);
    }
    TableResult& finalTableRes = dynamic_cast<TableResult&>(*finalRes);

    return std::make_unique<TableResult>(TableResult(finalTableRes.idents, finalTableRes.rows, order));
}


std::unique_ptr<Result> SelectResult::join(Result &rhs) {
    if (rhs.tag == Tag::BOOL) {
        BoolResult &boolRes = dynamic_cast<BoolResult &>(rhs);
        if (boolRes.b) {
            return getColsCrossProduct();
        }
        return std::make_unique<TableResult>(TableResult());
    }

    TableResult &t2 = dynamic_cast<TableResult &>(rhs);
    std::unordered_set<std::string> intermediateIdentsSet(t2.idents.begin(), t2.idents.end());
    std::unique_ptr<Result> finalRes = std::make_unique<TableResult>(TableResult(t2));
    for (int i = 0; i < idents.size(); i++) {
        if (intermediateIdentsSet.find(idents[i]) == intermediateIdentsSet.end()) { // if non-overlapping
            finalRes = finalRes->join(cols[i]);
        }
    }

    TableResult& finalTableRes = dynamic_cast<TableResult&>(*finalRes);
    // get vector of indexes in order to be printed
    std::vector<int> order;
    for (int i = 0; i < idents.size(); i++) {
        for (int j = 0; j < finalTableRes.idents.size(); j++) {
            if (idents[i] == finalTableRes.idents[j]) {
                order.push_back(j);
            }
        }
    }

    return std::make_unique<TableResult>(finalTableRes.idents, finalTableRes.rows, order);
}

void SelectResult::output(std::list<std::string> &list) {
}

bool SelectResult::operator==(const Result &rhs) const {
    const SelectResult *pRhs = dynamic_cast<const SelectResult *>(&rhs);
    return pRhs != nullptr && idents == pRhs->idents && cols == pRhs->cols;
}
