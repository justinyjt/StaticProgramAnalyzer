#include "SelectResult.h"

#include <iterator>

// general constructor for n-cols
SelectResult::SelectResult(std::vector<std::string> &_idents, const std::vector<TableResult> &_cols) :
        Result(Tag::SELECT) {
    idents.insert(idents.end(), _idents.begin(), _idents.end());
    cols.insert(cols.end(), _cols.begin(), _cols.end());
}

std::unique_ptr<Result> SelectResult::join(Result &rhs) {
    TableResult &t2 = dynamic_cast<TableResult &>(rhs);
    std::unordered_set<std::string> intermediateIdentsSet(t2.idents.begin(), t2.idents.end());
    TableResult finalRes = TableResult(t2);
    for (int i = 0; i < idents.size(); i++) {
        if (intermediateIdentsSet.find(idents[i]) == intermediateIdentsSet.end()) { // if non-overlapping
            finalRes.join(cols[i]);
        }
    }

    // get vector of indexes in order to be printed
    std::vector<int> order;
    for (int i = 0; i < idents.size(); i++) {
        for (int j = 0; j < finalRes.idents.size(); j++) {
            if (idents[i] == finalRes.idents[j]) {
                order.push_back(j);
            }
        }
    }

    return std::make_unique<TableResult>(finalRes.idents, finalRes.rows, order);
}

void SelectResult::output(std::list<std::string> &list) {
}

bool SelectResult::operator==(const Result &rhs) const {
    const SelectResult *pRhs = dynamic_cast<const SelectResult *>(&rhs);
    return pRhs != nullptr && idents == pRhs->idents && cols == pRhs->cols;
}
