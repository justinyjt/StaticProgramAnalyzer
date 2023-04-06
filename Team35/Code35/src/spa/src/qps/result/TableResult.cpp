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
TableResult::TableResult(TableHeaders headers, TableRows rows)
        : Result(Tag::TABLE), table_headers_(std::move(headers)), rows_(std::move(rows)) {}

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

std::unique_ptr<TableResult> TableResult::getCrossProduct(const TableResult &rhs) const {
    TableHeaders headers;
    headers.insert(headers.end(), table_headers_.begin(), table_headers_.end());
    headers.insert(headers.end(), rhs.table_headers_.begin(), rhs.table_headers_.end());

    TableRows rows;
    for (auto &row1 : rows_) {
        for (auto &row2 : rhs.rows_) {
            TableRow newRow;
            newRow.insert(newRow.end(), row1.begin(), row1.end());
            newRow.insert(newRow.end(), row2.begin(), row2.end());
            rows.emplace_back(newRow);
        }
    }
    return std::make_unique<TableResult>(headers, rows);
}

bool TableResult::isRowInIntersection(const TableRow &lhs_row,
                                      const TableRow &rhs_row,
                                      const std::vector<ColumnIndex> &common_headers1,
                                      const std::vector<ColumnIndex> &common_headers2) {
    for (int k = 0; k < common_headers1.size(); ++k) {
        if (lhs_row[common_headers1[k]] != rhs_row[common_headers2[k]]) {
            return false;
        }
    }
    return true;
}

std::unique_ptr<TableResult> TableResult::getTableIntersection(const TableResult &rhs,
                                                               const std::vector<ColumnIndex> &common_headers1,
                                                               const std::vector<ColumnIndex> &common_headers2) const {
    // find non common headers for RHS table
    std::vector<ColumnIndex> rhs_non_common_header_indices;
    for (ColumnIndex i = 0; i < rhs.table_headers_.size(); ++i) {
        auto it = std::find(common_headers2.begin(), common_headers2.end(), i);
        if (it == common_headers2.end()) {
            rhs_non_common_header_indices.push_back(i);
        }
    }

    TableHeaders output_headers;
    output_headers.insert(output_headers.end(), table_headers_.begin(), table_headers_.end());
    for (ColumnIndex idx : rhs_non_common_header_indices) {
        output_headers.push_back(rhs.table_headers_.at(idx));
    }

    TableRows output_rows;
    for (auto &lhs_row : rows_) {
        for (auto &rhs_row : rhs.rows_) {
            if (!isRowInIntersection(lhs_row, rhs_row, common_headers1, common_headers2)) {
                continue;
            }
            TableRow newRow;
            for (auto &value : lhs_row) {
                newRow.push_back(value);
            }
            for (auto &idx : rhs_non_common_header_indices) {
                newRow.push_back(rhs_row[idx]);
            }
            output_rows.push_back(newRow);
        }
    }
    return std::make_unique<TableResult>(output_headers, output_rows);
}

std::unique_ptr<Result> TableResult::join(Result &rhs) {
    if (rhs.tag == Tag::BOOL) {
        auto &boolResult = dynamic_cast<BoolResult &>(rhs);
        if (boolResult.b) {
            return std::make_unique<TableResult>(*this);
        }
        return std::make_unique<TableResult>();
    }

    auto &t2 = dynamic_cast<TableResult &>(rhs);

    TableHeaders &headers1 = table_headers_;
    TableHeaders &headers2 = t2.table_headers_;
    std::vector<ColumnIndex> common_headers1;
    std::vector<ColumnIndex> common_headers2;

    // find common headers for table 1 and 2
    for (int i = 0; i < headers1.size(); ++i) {
        for (int j = 0; j < headers2.size(); ++j) {
            if (headers1[i] == headers2[j]) {
                common_headers1.push_back(i);
                common_headers2.push_back(j);
            }
        }
    }

    // if no matching headers
    if (common_headers1.empty()) {
        return getCrossProduct(t2);
    } else {
        return getTableIntersection(t2, common_headers1, common_headers2);
    }
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

    std::unordered_set < std::string > a;
    for (auto &row : rows_) {
        std::string s;
        for (auto &elem : row) s += elem;
        a.insert(s);
    }

    std::unordered_set < std::string > b;
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
