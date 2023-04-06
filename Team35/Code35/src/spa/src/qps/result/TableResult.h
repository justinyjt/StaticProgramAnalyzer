#pragma once

#include <cstdint>
#include <iostream>
#include <iterator>
#include <list>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

#include "Result.h"
#include "commons/util/StringUtil.h"

template<typename T>
using TableCell = T;

typedef std::vector<TableCell<std::string>> TableRow;
typedef std::vector<TableRow> TableRows;
typedef std::string TableHeader;
typedef std::vector<TableHeader> TableHeaders;

// n-col result
class TableResult : public Result {
 public:
    // constructor for empty table
    TableResult();

    // general constructor for n-cols
    TableResult(TableHeaders headers, TableRows rows);

    // constructor for SelectResult Output
    TableResult(const TableResult &tableRes, const std::vector<uint32_t> &order);

    template<typename T>
    TableResult(const TableHeader &header, const std::unordered_set<T> &set) : Result(Tag::TABLE) {
        table_headers_.push_back(header);
        for (auto &p : set) {
            rows_.emplace_back(TableRow{StringUtil::toString(p)});
        }
    }

    template<typename S, typename T>
    TableResult(const std::string &header1, const std::string &header2, const PairSet<S, T> &set) : Result(Tag::TABLE) {
        if (header1 == header2) {
            table_headers_.push_back(header1);
            for (auto &p : set) {
                rows_.emplace_back(TableRow{StringUtil::toString(p.first)});
            }
        } else {
            table_headers_.push_back(header1);
            table_headers_.push_back(header2);
            for (auto &p : set) {
                rows_.emplace_back(TableRow{StringUtil::toString(p.first), StringUtil::toString(p.second)});
            }
        }
    }

    std::unique_ptr<Result> join(Result &rhs) override;

    void output(std::list<std::string> &result) const override;

    bool operator==(const Result &rhs) const;

    [[nodiscard]] std::unique_ptr<TableResult> projectColumns(
            const std::unordered_set<TableHeader> &projectedColumns) const;

    [[nodiscard]] bool isNull() const override;

    [[nodiscard]] const TableHeaders &getTableHeaders() const;

 private:
    TableHeaders table_headers_;
    std::vector<TableRow> rows_;
    std::optional<std::vector<uint32_t>> order_;  // order in which to output result
};
