#include "TableResult.h"

#include <iterator>

// general constructor for n-cols
TableResult::TableResult(std::list<std::string> &_idents,
                         const std::vector<std::list<std::string>> &_rows) : Result(Tag::TABLE) {
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
            std::initializer_list<std::string>
                {std::to_string(p.first), p.second});
    }
}

// for 2 cols with STMT_STMT_SET
TableResult::TableResult(const std::string &ident1, const std::string &ident2,
                         STMT_STMT_SET &set) : Result(Tag::TABLE) {
    idents.push_back(ident1);
    idents.push_back(ident2);
    for (auto &p : set) {
        rows.emplace_back(
            std::initializer_list<std::string>
                {std::to_string(p.first), std::to_string(p.second)});
    }
}

// for 2 cols with ENT_ENT_SET
TableResult::TableResult(const std::string &ident1, const std::string &ident2,
                         ENT_ENT_SET &set) : Result(Tag::TABLE) {
    idents.push_back(ident1);
    idents.push_back(ident2);
    for (auto &p : set) {
        rows.emplace_back(
                std::initializer_list<std::string>
                        {p.first, p.second});
    }
}

// for 2 cols with vector<list<string>>
TableResult::TableResult(const std::string &ident1, const std::string &ident2,
                         const std::vector<std::list<std::string>> &vec) : Result(Tag::TABLE) {
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
            std::initializer_list<std::string>{std::to_string(elem)});
}

void TableResult::output(std::list<std::string> &list) {
    // results already finalised from selectJoin, rows/tuples assumed to be a single element for now
    for (auto &elem : rows) {
        list.push_back(elem.front());
    }
}

bool TableResult::operator==(const Result &rhs) const {
    const TableResult *pRhs = dynamic_cast<const TableResult *>(&rhs);
    return pRhs != nullptr && idents == pRhs->idents && rows == pRhs->rows;
}
