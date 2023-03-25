#include "SelectResult.h"

#include <iterator>

// general constructor for n-cols
SelectResult::SelectResult(std::vector<std::string> &_idents,
                           const std::vector<std::vector<std::string>> &_cols,
                           const std::unordered_map<std::string, std::unordered_map<std::string, std::string>> &_synToAttrRefMap) :
        Result(Tag::SELECT) {
    idents.insert(idents.end(), _idents.begin(), _idents.end());
    cols.insert(cols.end(), _cols.begin(), _cols.end());
    for (auto const &synMapPair : _synToAttrRefMap) {
        synToAttrRefMap.insert({synMapPair.first, synMapPair.second});
    }
}

std::unique_ptr<Result> SelectResult::join(Result &) {
    throw std::runtime_error("");
}

//std::vector<std::vector<std::string>> SelectResult::getColumn(int colNum) {
//    std::vector<std::vector<std::string>> resultCol;
//    std::vector<std::string> currRow;
//    for (int i = 0; i < cols.size(); i++) {
//        currRow = cols[i];
//        resultCol.push_back(std::vector<std::string>({currRow[colNum]}));
//    }
//    return resultCol;
//}

std::unique_ptr<Result> SelectResult::crossProductHeaders(std::vector<std::string> headersToCP) {
    std::unique_ptr<Result> crossProduct;
    int currHeader = 0;
    bool isFirstHeader = true;
    for (int i = 0; i < idents.size(); i++) {
        if (headersToCP[currHeader] == idents[i]) {
            if (isFirstHeader) { // set first column
                isFirstHeader = false;
                crossProduct = std::make_unique<TableResult>(std::vector<std::string>({idents[i]}),
                                                             std::vector<std::vector<std::string>>({cols[i]}));
            } else {
                std::unique_ptr<Result> t = std::make_unique<TableResult>(
                    TableResult(std::vector<std::string>({idents[i]}),
                                std::vector<std::vector<std::string>>({cols[i]})));
                crossProduct = crossProduct->join(*t);
            }
            currHeader++;
        }
    }
    return crossProduct;
}

std::pair<std::vector<std::string>, std::vector<std::string>> SelectResult::classifyHeaders(std::vector<std::string> intermediateIdents) {
    std::vector<std::string> overlappingHeaders;
    std::vector<std::string> nonOverlappingHeaders;

    std::unordered_set<std::string> intermediateCols(
        intermediateIdents.begin(), intermediateIdents.end());

    for (int i = 0; i < idents.size(); i++) {
        if (intermediateCols.find(idents[i]) != intermediateCols.end()) {
            overlappingHeaders.push_back(idents[i]);
        } else {
            nonOverlappingHeaders.push_back(idents[i]);
        }
    }

    return std::make_pair(overlappingHeaders, nonOverlappingHeaders);
}

//TableResult SelectResult::projectColumns(TableResult intermediateResultTable, std::vector<std::string> projectedColumns) {
//    std::vector<std::string> projectedIdents;
//    std::vector<std::vector<std::string>> projectedRows;
//
//    for (auto const &header : projectedColumns) {
//        // get index of header
//        int index = std::distance(intermediateResultTable.idents.begin(),
//                                  std::find(intermediateResultTable.idents.begin(),
//                                            intermediateResultTable.idents.end(), header));
//
//        projectedIdents.emplace_back(intermediateResultTable.idents[index]);
//        std::vector<std::string> currCol;
//        for (auto const &row : intermediateResultTable.rows) {
//            currCol.emplace_back(row[index]);
//        }
//        projectedRows.emplace_back(currCol);
//    }
//
//    return TableResult(projectedIdents, projectedRows);
//}

std::unique_ptr<Result> SelectResult::mapSynValues(std::unique_ptr<Result> finalResultTable) {
    TableResult finalRes = dynamic_cast<TableResult&>(*finalResultTable);

    for (int i = 0; i < finalRes.idents.size(); i++) {
        for (int j = 0; j < finalRes.rows[0].size(); j++) {
            finalRes.rows[i][j] = synToAttrRefMap[idents[i]][cols[i][j]];
        }
    }

    return std::make_unique<TableResult>(finalRes);
}

std::unique_ptr<Result> SelectResult::getFinalResult(TableResult intermediateResultTable) {
    std::pair<std::vector<std::string>,
        std::vector<std::string>> splitHeaders = classifyHeaders(intermediateResultTable.idents);
    std::vector<std::string> overlappingHeaders = splitHeaders.first;
    std::vector<std::string> nonOverlappingHeaders = splitHeaders.second;

    std::unique_ptr<Result> finalRes;
    if (overlappingHeaders.size() == 0) { // no overlapping headers
        // CP Select Headers
        return crossProductHeaders(idents);

    } else if (nonOverlappingHeaders.size() == 0) { // select headers are a subset of result headers

        finalRes = intermediateResultTable.projectColumns(overlappingHeaders);

    } else { // some select headers are missing from result headers

        std::unique_ptr<Result> overlappingIntermediate = intermediateResultTable.
            projectColumns(overlappingHeaders);
        std::unique_ptr<Result> nonOverlappingCP = crossProductHeaders(nonOverlappingHeaders);
        finalRes = overlappingIntermediate->join(*nonOverlappingCP);
        // a, b, c, d      a, b
        // get a and b intermediate table
        // for each non overlapping column
        //      for each c
        //              for each a,b pair in intermediate

    }
    return std::move(mapSynValues(std::move(finalRes)));
}


//// constructor for empty table
//SelectResult::SelectResult() : Result(Tag::SELECT) {}
//
//// for 2 cols with STMT_ENT_SET
//SelectResult::SelectResult(const std::string &ident1, const std::string &ident2,
//                         STMT_ENT_SET &set, std::string selected) : Result(Tag::SELECT), selected(selected) {
//    idents.push_back(ident1);
//    idents.push_back(ident2);
//    for (auto &p : set) {
//        cols.emplace_back(
//                std::initializer_list<std::string>
//                        {std::to_string(p.first), p.second});
//    }
//}
//
//// for 2 cols with STMT_STMT_SET
//SelectResult::SelectResult(const std::string &ident1, const std::string &ident2,
//                         STMT_STMT_SET &set, std::string selected) : Result(Tag::SELECT), selected(selected) {
//    idents.push_back(ident1);
//    idents.push_back(ident2);
//    for (auto &p : set) {
//        cols.emplace_back(
//                std::initializer_list<std::string>
//                        {std::to_string(p.first), std::to_string(p.second)});
//    }
//}
//
//// for 2 cols with ENT_ENT_SET
//SelectResult::SelectResult(const std::string &ident1, const std::string &ident2,
//                         ENT_ENT_SET &set, std::string selected) : Result(Tag::SELECT), selected(selected) {
//    idents.push_back(ident1);
//    idents.push_back(ident2);
//    for (auto &p : set) {
//        cols.emplace_back(
//                std::initializer_list<std::string>
//                        {p.first, p.second});
//    }
//}
//
//// for 2 cols with vector<list<string>>
//SelectResult::SelectResult(const std::string &ident1, const std::string &ident2,
//                           const std::vector<std::list<std::string>> &vec, std::string selected) :
//        Result(Tag::SELECT), selected(selected) {
//    idents.push_back(ident1);
//    idents.push_back(ident2);
//    cols.insert(cols.end(), vec.begin(), vec.end());
//}
//
//// for 1 col with ENT_SET
//SelectResult::SelectResult(const std::string &ident, ENT_SET &set, std::string selected)
//        : Result(Tag::SELECT), selected(selected) {
//    idents.push_back(ident);
//    for (auto &elem : set)
//        cols.emplace_back(
//                std::initializer_list<std::string>{elem});
//}
//
//// for 1 col with STMT_SET
//SelectResult::SelectResult(const std::string &ident, STMT_SET &set, std::string selected) :
//        Result(Tag::SELECT), selected(selected) {
//    idents.push_back(ident);
//    for (auto &elem : set)
//        cols.emplace_back(
//                std::initializer_list<std::string>{std::to_string(elem)});
//}

void SelectResult::output(std::list<std::string> &list) {
    // results already finalised from selectJoin, cols/tuples assumed to be a single element for now
    for (auto &elem : cols) {
        list.push_back(elem.front());
    }
}

bool SelectResult::operator==(const Result &rhs) const {
    const SelectResult *pRhs = dynamic_cast<const SelectResult *>(&rhs);
    return pRhs != nullptr && idents == pRhs->idents && cols == pRhs->cols;
}
