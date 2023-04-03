#include "TableResult.h"

#include <iterator>
#include <memory>
#include <utility>
#include <set>

#include "BoolResult.h"

// general constructor for n-cols
TableResult::TableResult(const std::vector<std::string> &_idents,
                         const std::vector<std::vector<std::string>> &_rows) : Result(Tag::TABLE) {
    idents.insert(idents.end(), _idents.begin(), _idents.end());
    rows.insert(rows.end(), _rows.begin(), _rows.end());
}

// constructor for SelectResult Output
TableResult::TableResult(const std::vector<std::string> &_idents,
                         const std::vector<std::vector<std::string>> &_rows,
                         const std::vector<int> &_order) : Result(Tag::TABLE) {
    idents.insert(idents.end(), _idents.begin(), _idents.end());
    rows.insert(rows.end(), _rows.begin(), _rows.end());
    order = std::make_optional(_order);
}

// constructor for empty table
TableResult::TableResult() : Result(Tag::TABLE) {}

// for 2 cols with STMT_ENT_SET
TableResult::TableResult(const std::string &ident1, const std::string &ident2,
                         STMT_ENT_SET &set) : Result(Tag::TABLE) {
    idents.push_back(ident1);
    idents.push_back(ident2);
    for (auto &p : set) {
        rows.emplace_back(
            std::vector<std::string>
                {std::to_string(p.first), p.second});
    }
}

// for 2 cols with STMT_STMT_SET
TableResult::TableResult(const std::string &ident1, const std::string &ident2,
                         STMT_STMT_SET &set) : Result(Tag::TABLE) {
    if (ident1 == ident2) {
        idents.push_back(ident1);
        for (auto &p : set) {
            rows.emplace_back(
                std::vector<std::string>
                    {std::to_string(p.first)});
        }
    } else {
        idents.push_back(ident1);
        idents.push_back(ident2);
        for (auto &p : set) {
            rows.emplace_back(
                std::vector<std::string>
                    {std::to_string(p.first), std::to_string(p.second)});
        }
    }
}

// for 2 cols with ENT_ENT_SET
TableResult::TableResult(const std::string &ident1, const std::string &ident2,
                         ENT_ENT_SET &set) : Result(Tag::TABLE) {
    idents.push_back(ident1);
    idents.push_back(ident2);
    for (auto &p : set) {
        rows.emplace_back(
            std::vector<std::string>
                {p.first, p.second});
    }
}

// for 2 cols with vector<list<string>>
TableResult::TableResult(const std::string &ident1, const std::string &ident2,
                         const std::vector<std::vector<std::string>> &vec) : Result(Tag::TABLE) {
    idents.push_back(ident1);
    idents.push_back(ident2);
    rows.insert(rows.end(), vec.begin(), vec.end());
}

// for 1 col with ENT_SET
TableResult::TableResult(const std::string &ident, ENT_SET &set) : Result(Tag::TABLE) {
    idents.push_back(ident);
    for (auto &elem : set)
        rows.emplace_back(
            std::initializer_list<std::string>{elem});
}

// for 1 col with STMT_SET
TableResult::TableResult(const std::string &ident, STMT_SET &set) : Result(Tag::TABLE) {
    idents.push_back(ident);
    for (auto &elem : set)
        rows.emplace_back(
            std::vector<std::string>{std::to_string(elem)});
}

// 2 col with ENT_SET
TableResult::TableResult(const std::string &ident1, const std::string &ident2,
                         const std::vector<ENT_NAME> &set) : Result(Tag::TABLE) {
    idents.push_back(ident1);
    idents.push_back(ident2);
    for (auto &ent : set) {
        rows.emplace_back(std::vector<ENT_NAME>{ent, ent});
    }
}

std::unique_ptr<TableResult> TableResult::projectColumns(const
                                                         std::unordered_set<std::string> &projectedColumns) const {
    std::vector<std::string> projectedIdents;
    std::vector<std::vector<std::string>> projectedRowsWithDuplicates(rows.size(), std::vector<std::string>());

    for (int i = 0; i < idents.size(); i++) {
        if (projectedColumns.find(idents[i]) == projectedColumns.end()) {
            continue;
        }
        projectedIdents.emplace_back(idents[i]);
        for (int j = 0; j < rows.size(); j++) {
            projectedRowsWithDuplicates[j].emplace_back(rows[j][i]);
        }
    }

    // remove duplicated rows
    std::set<std::vector<std::string>> rowSet(projectedRowsWithDuplicates.begin(), projectedRowsWithDuplicates.end());
    std::vector<std::vector<std::string>> projectedRowsWithoutDuplicates;
    projectedRowsWithoutDuplicates.assign(rowSet.begin(), rowSet.end());
    std::unique_ptr<TableResult> tableResult = std::make_unique<TableResult>(
        projectedIdents, projectedRowsWithoutDuplicates);
    return std::move(tableResult);
}

std::unique_ptr<Result> TableResult::join(Result &rhs) {
    if (rhs.tag == Tag::BOOL) {
        auto &boolResult = dynamic_cast<BoolResult &>(rhs);
        std::unique_ptr<Result> res;
        if (boolResult.b) {
            res = std::make_unique<TableResult>(std::move(*this));
        } else {
            res = std::make_unique<BoolResult>(boolResult.b);
        }
        return res;
    }

    auto &t2 = dynamic_cast<TableResult &>(rhs);

    std::vector<std::string> headers1(idents.begin(), idents.end());
    std::vector<std::string> headers2(t2.idents.begin(), t2.idents.end());
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

        for (std::vector<std::string> &row1 : rows) {
            for (std::vector<std::string> &row2 : t2.rows) {
                std::vector<std::string> concat;
                concat.insert(concat.end(), row1.begin(), row1.end());
                concat.insert(concat.end(), row2.begin(), row2.end());
                outputColumns.push_back(concat);
            }
        }
    } else {
        // there are matching column headers, inner join
        // produce output headers
        outputHeaders.insert(outputHeaders.end(), idents.begin(), idents.end());
        for (int idx : nonCommonHeaders2) {
            outputHeaders.push_back(headers2[idx]);
        }

        for (auto &row : rows) {
            for (auto &j : t2.rows) {
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

    std::unique_ptr<TableResult> tableResult = std::make_unique<TableResult>(outputHeaders, outputColumns);
    return std::move(tableResult);
}

void TableResult::output(std::list<std::string> &list) {
    if (order.has_value()) {
        int order_size = order.value().size();
        for (auto const &row : rows) {
            std::string curr_res;
            for (int i = 0; i < order_size; i++) {
                curr_res += row[order.value()[i]];

                if (i != order_size - 1) {
                    curr_res += " ";
                }
            }
            list.push_back(curr_res);
        }
    } else {
        for (auto const &row : rows) {
            for (auto const &ele : row) {
                list.push_back(ele);
            }
        }
    }
}

bool TableResult::operator==(const Result &rhs) const {
    const auto *pRhs = dynamic_cast<const TableResult *>(&rhs);
    if (pRhs == nullptr || idents != pRhs->idents || rows.size() != pRhs->rows.size()) {
        return false;
    }

    std::unordered_set<std::string> a;
    for (auto &row : rows) {
        std::string s;
        for (auto &elem : row) s += elem;
        a.insert(s);
    }

    std::unordered_set<std::string> b;
    for (auto &row : pRhs->rows) {
        std::string s;
        for (auto &elem : row) s += elem;
        b.insert(s);
    }

    return a == b;
}

bool TableResult::isNull() const {
    return rows.empty();
}
