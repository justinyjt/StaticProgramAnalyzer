#include "TableResult.h"
#include "BoolResult.h"

#include <iterator>

// general constructor for n-cols
TableResult::TableResult(const std::vector<std::string> &_idents,
                         const std::vector<std::vector<std::string>> &_rows) : Result(Tag::TABLE) {
    idents.insert(idents.end(), _idents.begin(), _idents.end());
    rows.insert(rows.end(), _rows.begin(), _rows.end());
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

//bool TableResult::isEmpty() {
//    return cols.size() == 0;
//}

std::unique_ptr<TableResult> TableResult::projectColumns(std::vector<std::string> projectedColumns) {
    std::vector<std::string> projectedIdents;
    std::vector<std::vector<std::string>> projectedRows;

    for (auto const &header : projectedColumns) {
        // get index of header
        int index = std::distance(idents.begin(),
                                  std::find(idents.begin(),
                                            idents.end(), header));

        projectedIdents.emplace_back(idents[index]);
        std::vector<std::string> currCol;
        for (auto const &row : rows) {
            currCol.emplace_back(row[index]);
        }
        projectedRows.emplace_back(currCol);
    }

    std::unique_ptr<TableResult> tableResult = std::make_unique<TableResult>(projectedIdents, projectedRows);
    return std::move(tableResult);
}

std::unique_ptr<Result> TableResult::join(Result &rhs) {
    if (rhs.tag == Tag::BOOL) {
        BoolResult &boolResult = dynamic_cast<BoolResult &>(rhs);
        std::unique_ptr<Result> res;
        if (boolResult.b) {
            res = std::make_unique<TableResult>(std::move(*this));
        } else {
            res = std::make_unique<BoolResult>(boolResult.b);
        }
        return res;
    }

    TableResult &t2 = dynamic_cast<TableResult &>(rhs);

    std::vector<std::string> headers1(idents.begin(), idents.end());
    std::vector<std::string> headers2(t2.idents.begin(), t2.idents.end());
    std::list<int> commonHeaders1;
    std::list<int> commonHeaders2;
    std::list<int> nonCommonHeaders1;
    std::list<int> nonCommonHeaders2;
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
    if (commonHeaders1.size() == 0) {
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
        for (int idx : commonHeaders1) {
            outputHeaders.push_back(headers1[idx]);
        }
        for (int idx : nonCommonHeaders1) {
            outputHeaders.push_back(headers1[idx]);
        }
        for (int idx : nonCommonHeaders2) {
            outputHeaders.push_back(headers2[idx]);
        }

        // separate cols in table into keys and values

        std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>> hashmap1;
        for (std::vector<std::string> &row : rows) {
            std::vector<std::string> rowVector(row.begin(), row.end());
            std::vector<std::string> keys;
            std::vector<std::string> values;
            for (auto i : commonHeaders1) {
                keys.push_back(rowVector.at(i));
            }
            for (auto i : nonCommonHeaders1) {
                values.push_back(rowVector.at(i));
            }
            hashmap1.push_back(std::make_pair(keys, values));
        }

        std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>> hashmap2;
        for (std::vector
            <std::string> &row : t2.rows) {
            std::vector<std::string> rowVector(row.begin(), row.end());
            std::vector<std::string> keys;
            std::vector<std::string> values;
            for (auto i : commonHeaders2) {
                keys.push_back(rowVector.at(i));
            }
            for (auto i : nonCommonHeaders2) {
                values.push_back(rowVector.at(i));
            }
            hashmap2.push_back(std::make_pair(keys, values));
        }

        // generate new table
        for (auto &kv : hashmap1) {
            std::vector<std::string> &key1 = kv.first;
            std::vector<std::string> &value1 = kv.second;

            // Look up the key in the second table
            for (auto &kv2 : hashmap2) {
                std::vector<std::string> &key2 = kv2.first;
                std::vector<std::string> &value2 = kv2.second;
                if (key1 == key2) {
                    // append values from t1 and t2
                    std::vector<std::string> res;
                    res.insert(res.end(), key1.begin(), key1.end());
                    res.insert(res.end(), value1.begin(), value1.end());
                    res.insert(res.end(), value2.begin(), value2.end());
                    outputColumns.push_back(res);
                }
            }
        }
    }

    std::unique_ptr<TableResult> tableResult = std::make_unique<TableResult>(outputHeaders, outputColumns);
    return std::move(tableResult);
}

void TableResult::output(std::list<std::string> &list) {
    // results already finalised from selectJoin, cols/tuples assumed to be a single element for now
    for (auto &elem : rows) {
        list.push_back(elem.front());
    }
}

//// general constructor for n-cols
//TableResult::TableResult(SelectResult &selectResult) : Result(Tag::TABLE) {
//    idents = selectResult.idents;
//    cols = selectResult.cols;
//}

bool TableResult::operator==(const Result &rhs) const {
    const TableResult *pRhs = dynamic_cast<const TableResult *>(&rhs);
    if (pRhs == nullptr || idents != pRhs->idents || rows.size() != pRhs->rows.size()) {
        return false;
    }

    std::unordered_set<std::string> a;
    for (auto &row : rows) {
        std::string s{""};
        for (auto &elem : row) s += elem;
        a.insert(s);
    }

    std::unordered_set<std::string> b;
    for (auto &row : pRhs->rows) {
        std::string s{""};
        for (auto &elem : row) s += elem;
        b.insert(s);
    }

    return a == b;
}
