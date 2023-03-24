#pragma once
#include <string>
#include <list>
#include <vector>
#include <iostream>
#include <iterator>
#include <unordered_map>
#include <unordered_set>

#include "Result.h"
#include "TableResult.h"

// n-col result
class SelectResult : public Result {
 public:
    std::vector<std::string> idents;  // eg list<"v", "s1">
    std::vector<std::vector<std::string>> cols;  // eg vector<list<"x",1>, list<"y",2>, ...>
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> synToAttrRefMap;
//    std::list<std::string> selected;

    // general constructor for n-cols
    SelectResult(std::vector<std::string> &_idents,
                const std::vector<std::vector<std::string>> &_cols,
                const std::unordered_map<std::string, std::unordered_map<std::string, std::string>> &_synToAttrRefMap);

//    // constructor for empty table
//    SelectResult();
//
//    // for 2 cols with STMT_ENT_SET
//    SelectResult(const std::string &ident1, const std::string &ident2,
//                STMT_ENT_SET &set, std::string selected);
//
//    // for 2 cols with STMT_STMT_SET
//    SelectResult(const std::string &ident1, const std::string &ident2,
//                STMT_STMT_SET &set, std::string selected);
//
//    // for 2 cols with ENT_ENT_SET
//    SelectResult(const std::string &ident1, const std::string &ident2,
//                ENT_ENT_SET &set, std::string selected);
//
//    // for 2 cols with vector<list<string>>
//    SelectResult(const std::string &ident1, const std::string &ident2,
//                const std::vector<std::list<std::string>> &vec, std::string selected);
//
//    // for 1 col with ENT_SET
//    SelectResult(const std::string &ident, ENT_SET &set, std::string selected);
//
//    // for 1 col with STMT_SET
//    SelectResult(const std::string &ident, STMT_SET &set, std::string selected);


//    std::vector<std::vector<std::string>> getColumn(int colNum);

    std::unique_ptr<Result> crossProductHeaders(std::vector<std::string> headersToCP);

    std::pair<std::vector<std::string>, std::vector<std::string>> classifyHeaders(std::vector<std::string> intermediateIdents);

//    TableResult projectColumns(TableResult intermediateResultTable, std::vector<std::string> projectedColumns);

    std::unique_ptr<Result> mapSynValues(std::unique_ptr<Result> finalResultTable);

    std::unique_ptr<Result> getFinalResult(TableResult intermediateResultTable);





    void output(std::list<std::string> &list) override;

    bool operator==(const Result &rhs) const;
};
