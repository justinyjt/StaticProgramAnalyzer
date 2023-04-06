#include "TableResult.h"

#include <cstdint>
#include <iterator>
#include <memory>
#include <utility>
#include <set>

#include "BoolResult.h"

// constructor for empty table
TableResult::TableResult() : Result(Tag::TABLE) {}

// general constructor for n-cols
TableResult::TableResult(const TableHeaders &headers, const TableRows &rows)
        : Result(Tag::TABLE), table_headers_(headers), rows_(std::move(rows)) {}

// constructor for SelectResult Output
TableResult::TableResult(const TableResult &tableResult, const std::vector<uint32_t> &order)
        : Result(Tag::TABLE), order_(order), table_headers_(tableResult.table_headers_), rows_(tableResult.rows_) {}

std::unique_ptr<TableResult> TableResult::projectColumns(const std::unordered_set<std::string> &projectedCols) const {
    std::vector<std::string> projectedIdents;
    std::vector<std::vector<std::string>> projectedRowsWithDuplicates(rows_.size(), std::vector<std::string>());

    for (int i = 0; i < table_headers_.size(); i++) {
        if (projectedCols.find(table_headers_[i]) == projectedCols.end()) {
            continue;
        }
        projectedIdents.emplace_back(table_headers_[i]);
        for (int j = 0; j < rows_.size(); j++) {
            projectedRowsWithDuplicates[j].emplace_back(rows_[j][i]);
        }
    }

    // remove duplicated rows
    std::set<std::vector<std::string>> rowSet(projectedRowsWithDuplicates.begin(), projectedRowsWithDuplicates.end());
    std::vector<std::vector<std::string>> projectedRowsWithoutDuplicates;
    projectedRowsWithoutDuplicates.assign(rowSet.begin(), rowSet.end());
    return std::make_unique<TableResult>(projectedIdents, projectedRowsWithoutDuplicates);
}

std::unique_ptr<Result> TableResult::join(Result &rhs) {
    if (rhs.tag == Tag::BOOL) {
        auto &boolResult = dynamic_cast<BoolResult &>(rhs);
        std::unique_ptr<Result> res;
        if (boolResult.b) {
            res = std::make_unique<TableResult>(*this);
        } else {
            res = std::make_unique<BoolResult>(false);
        }
        return res;
    }

    auto &t2 = dynamic_cast<TableResult &>(rhs);

    std::vector<std::string> headers1(table_headers_.begin(), table_headers_.end());
    std::vector<std::string> headers2(t2.table_headers_.begin(), t2.table_headers_.end());
    std::vector<int> commonHeaders1;
    std::vector<int> commonHeaders2;
    std::vector<int> nonCommonHeaders1;
    std::vector<int> nonCommonHeaders2;
    std::vector<std::string> outputHeaders;
    std::vector<std::vector<std::string>> outputColumns;

    // find common headers for table 1 and 2
    for (int i = 0; i < headers1.size(); i++) {
        for (int j = 0; j < headers2.size(); j++) {
            if (headers1[i] == headers2[j]) {
                commonHeaders1.push_back(i);
                commonHeaders2.push_back(j);
            }
        }
    }

    // find non common headers for table 1
    for (int i = 0; i < headers1.size(); i++) {
        auto it = std::find(commonHeaders1.begin(), commonHeaders1.end(), i);
        if (it == commonHeaders1.end()) {
            nonCommonHeaders1.push_back(i);
        }
    }

    // find non common headers for table 2
    for (int i = 0; i < headers2.size(); i++) {
        auto it = std::find(commonHeaders2.begin(), commonHeaders2.end(), i);
        if (it == commonHeaders2.end()) {
            nonCommonHeaders2.push_back(i);
        }
    }

    // if no matching headers
    if (commonHeaders1.empty()) {
        // append all headers
        outputHeaders.insert(outputHeaders.end(), headers1.begin(), headers1.end());
        outputHeaders.insert(outputHeaders.end(), headers2.begin(), headers2.end());

        for (std::vector<std::string> &row1 : rows_) {
            for (std::vector<std::string> &row2 : t2.rows_) {
                std::vector<std::string> concat;
                concat.insert(concat.end(), row1.begin(), row1.end());
                concat.insert(concat.end(), row2.begin(), row2.end());
                outputColumns.push_back(concat);
            }
        }
    } else {
        // there are matching column headers, inner join
        // produce output headers
        outputHeaders.insert(outputHeaders.end(), table_headers_.begin(), table_headers_.end());
        for (int idx : nonCommonHeaders2) {
            outputHeaders.push_back(headers2[idx]);
        }

        for (auto &row : rows_) {
            for (auto &j : t2.rows_) {
                bool match = true;
                for (int k = 0; k < commonHeaders1.size(); ++k) {
                    if (row[commonHeaders1[k]] != j[commonHeaders2[k]]) {
                        match = false;
                        break;
                    }
                }
                if (!match) {
                    continue;
                }
                std::vector<std::string> concat;
                for (auto &value : row) {
                    concat.push_back(value);
                }
                for (int k = 0; k < j.size(); ++k) {
                    bool canAdd = true;
                    for (auto &header : commonHeaders2) {
                        if (header == k) {
                            canAdd = false;
                            break;
                        }
                    }
                    if (canAdd) {
                        concat.push_back(j[k]);
                    }
                }
                outputColumns.push_back(concat);
            }
        }
    }

    return std::make_unique<TableResult>(outputHeaders, outputColumns);
}

void TableResult::output(std::list<std::string> &list) const {
    if (!order_.has_value()) {
        for (auto const &row : rows_) {
            for (auto const &ele : row) {
                list.push_back(ele);
            }
        }
        return;
    }
    uint32_t order_size = order_.value().size();
    for (auto const &row : rows_) {
        std::string curr_res;
        for (int i = 0; i < order_size; i++) {
            curr_res += row[order_.value()[i]];

            if (i != order_size - 1) {
                curr_res += " ";
            }
        }
        list.push_back(curr_res);
    }
}

bool TableResult::operator==(const Result &rhs) const {
    const auto *pRhs = dynamic_cast<const TableResult *>(&rhs);
    if (pRhs == nullptr || table_headers_ != pRhs->table_headers_ || rows_.size() != pRhs->rows_.size()) {
        return false;
    }

    std::unordered_set<std::string> a;
    for (auto &row : rows_) {
        std::string s;
        for (auto &elem : row) s += elem;
        a.insert(s);
    }

    std::unordered_set<std::string> b;
    for (auto &row : pRhs->rows_) {
        std::string s;
        for (auto &elem : row) s += elem;
        b.insert(s);
    }

    return a == b;
}

bool TableResult::isNull() const {
    return rows_.empty();
}

const TableHeaders &TableResult::getTableHeaders() const {
    return table_headers_;
}
