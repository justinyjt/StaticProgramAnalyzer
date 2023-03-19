#include "Result.h"

#include <utility>
#include <set>
#include <algorithm>

#include "BoolResult.h"
#include "TableResult.h"
#include "SelectResult.h"

Result::Result(Result::Tag _tag) : tag(_tag) {}

// select v
std::unique_ptr<Result> Result::join(Result &lhs, Result &rhs) {
    // case bool <-> bool:
    if (lhs.tag == Tag::BOOL && rhs.tag == Tag::BOOL) {
        BoolResult &l = dynamic_cast<BoolResult &>(lhs);
        BoolResult &r = dynamic_cast<BoolResult &>(rhs);
        std::unique_ptr<Result> res = std::make_unique<BoolResult>(l.b && r.b);
        return std::move(res);
    }

    // case table <-> table:
    if (lhs.tag == Tag::TABLE && rhs.tag == Tag::TABLE) {
        std::unique_ptr<Result> res = Result::tableJoin(lhs, rhs);
        return std::move(res);
    }

    // case bool <-> table:
    if (lhs.tag == Tag::BOOL) {
        if (dynamic_cast<BoolResult&>(lhs).b) {
            TableResult& tbl = dynamic_cast<TableResult&>(rhs);
            return std::move(std::make_unique<TableResult>(tbl));
        } else {
            return std::move(std::make_unique<BoolResult>(false));
        }
    }

    // case table <-> bool:
    if (dynamic_cast<BoolResult&>(rhs).b) {
        TableResult& tbl = dynamic_cast<TableResult&>(lhs);
        return std::move(std::make_unique<TableResult>(tbl));
    } else {
        return std::move(std::make_unique<BoolResult>(false));
    }
}

std::unique_ptr<Result> Result::tableJoin(Result &lhs, Result &rhs) {
    TableResult &t1 = dynamic_cast<TableResult &>(lhs);
    TableResult &t2 = dynamic_cast<TableResult &>(rhs);

    std::vector<std::string> headers1(t1.idents.begin(), t1.idents.end());
    std::vector<std::string> headers2(t2.idents.begin(), t2.idents.end());
    std::list<int> commonHeaders1;
    std::list<int> commonHeaders2;
    std::list<int> nonCommonHeaders1;
    std::list<int> nonCommonHeaders2;
    std::list<std::string> outputHeaders;
    std::vector<std::list<std::string>> outputColumns;

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

        for (std::list<std::string> &row1 : t1.rows) {
            for (std::list<std::string> &row2 : t2.rows) {
                std::list<std::string> concat;
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

        // separate rows in table into keys and values

        std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>> hashmap1;
        for (std::list<std::string> &row : t1.rows) {
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
        for (std::list<std::string> &row : t2.rows) {
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
                    std::list<std::string> res;
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

std::unique_ptr<Result> Result::selectJoin(Result &lhs, Result &rhs) {
    // LHS is always SelectResult as evaluated by SelectClause
    if (lhs.tag == Tag::BOOL && rhs.tag == Tag::BOOL) {
        BoolResult &l = dynamic_cast<BoolResult &>(lhs);
        BoolResult &r = dynamic_cast<BoolResult &>(rhs);
        std::unique_ptr<Result> res = std::make_unique<BoolResult>(l.b && r.b);
        return std::move(res);
    } else if (lhs.tag == Tag::BOOL && rhs.tag == Tag::TABLE) {
        // check RHS table is empty
        TableResult &r = dynamic_cast<TableResult &>(rhs);
        std::unique_ptr<Result> res = std::make_unique<BoolResult>(r.rows.size() != 0);
        return std::move(res);
    } else if (lhs.tag == Tag::SELECT && rhs.tag == Tag::BOOL) {
        BoolResult &r = dynamic_cast<BoolResult &>(rhs);
        SelectResult& tbl = dynamic_cast<SelectResult &>(lhs);
        return r.b ? std::move(std::make_unique<SelectResult>(tbl)) : std::move(std::make_unique<SelectResult>());
    } else if (lhs.tag == Tag::SELECT && rhs.tag == Tag::TABLE) {
        int selectedIdx = 0;
        SelectResult& selectList = dynamic_cast<SelectResult&>(lhs);
        TableResult& r = dynamic_cast<TableResult&>(rhs);
        std::string selected = selectList.selected;
        std::list<std::string> resultTableHeaders;
        std::vector<std::list<std::string>> rows;
        std::unordered_set<std::string> resultSet;
        resultTableHeaders.push_back(selected);

        // select clause or result clause is empty
        if (r.rows.size() == 0 || selectList.rows.size() == 0) {
            return std::move(std::make_unique<TableResult>());
        }

        // join select with rhs, then selected the column 'selected'
        TableResult selectTable = TableResult(selectList);
        std::unique_ptr<Result> res = Result::join(selectTable, r);
        TableResult& tbl = dynamic_cast<TableResult&>(*res);

        for (auto &s : tbl.idents) {
            if (s == selected) {
                break;
            }
            selectedIdx++;
        }

        // no matching headers, just return selected list
        if (selectedIdx == tbl.idents.size()) {
            return std::move(std::make_unique<TableResult>(selectList));
        }

        // get all unique values corresponding to the selected ident
        for (auto &row : tbl.rows) {
            resultSet.insert(
                *(std::next(row.begin(), selectedIdx)));
        }

        // populate table
        for (auto &elem : resultSet) {
            std::list<std::string> row;
            row.push_back(elem);
            rows.push_back(row);
        }

        return std::move(std::make_unique<TableResult>(resultTableHeaders, rows));
    }
}
