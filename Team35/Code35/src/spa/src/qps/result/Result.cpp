#include "Result.h"
#include "BoolResult.h"
#include "TableResult.h"
#include <utility>

Result::Result(Result::Tag _tag) : tag(_tag) {}

Result* Result::join(Result* lhs, Result* rhs) {
  // case bool <-> bool:
  if (lhs->tag == Tag::BOOL && rhs->tag == Tag::BOOL) {
    BoolResult* l = dynamic_cast<BoolResult*>(lhs);
    BoolResult* r = dynamic_cast<BoolResult*>(rhs);
    Result* result = new BoolResult(l->b && r->b);
  }

  // case table <-> table:
  if (lhs->tag == Tag::TABLE && rhs->tag == Tag::TABLE) {
    return Result::tableJoin(lhs, rhs);
  }

  // case table <-> bool or bool <-> table:
  BoolResult* br = dynamic_cast<BoolResult*>(rhs);
  TableResult* tr = dynamic_cast<TableResult*>(rhs);
  Result* result;

  if (!br->b) {  // empty set
    std::vector<std::list<std::string>> rows;
    result = new TableResult(tr->idents, rows);
  } else {  // make a copy
    result = new TableResult(*tr);
  }

  return result;
}

Result* Result::tableJoin(Result* lhs, Result* rhs) {
  TableResult* t1 = dynamic_cast<TableResult*>(lhs);
  TableResult* t2 = dynamic_cast<TableResult*>(rhs);

  std::vector<std::string> headers1(t1->idents.begin(), t1->idents.end());
  std::vector<std::string> headers2(t2->idents.begin(), t2->idents.end());
  std::vector<int> commonHeaders1;
  std::vector<int> commonHeaders2;
  std::vector<int> nonCommonHeaders1;
  std::vector<int> nonCommonHeaders2;
  std::list<std::string> outputHeaders;
  std::vector<std::list<std::string>> outputColumns;

  // find common headers for table 1 and 2
  for (int i = 0; i < headers1.size(); i++) {
    for (int j = 0; j < headers2.size(); j++) {
      if (headers1.at(i) == headers2.at(j)) {
        commonHeaders1.push_back(i);
        commonHeaders2.push_back(j);
      }
    }
  }

  // find non common headers for table 1
  for (int i = 0; i < headers1.size(); i++) {
    if (std::find(commonHeaders1.begin(), commonHeaders1.end(), i) != commonHeaders1.end()) {
    } else {
      // i is not in common headers, add to values
      nonCommonHeaders1.push_back(i);
    }
  }

  // find non common headers for table 2
  for (int i = 0; i < headers2.size(); i++) {
    if (std::find(commonHeaders2.begin(), commonHeaders2.end(), i) != commonHeaders2.end()) {
    } else {
      // i is not in common headers, add to values
      nonCommonHeaders2.push_back(i);
    }
  }

  // if there are matching column headers, merge
  if (commonHeaders1.size() > 0) {
    // produce output headers
    for (int commonHeader : commonHeaders1) {
      outputHeaders.push_back(headers1.at(commonHeader));
    }

    for (int nonCommonHeader : nonCommonHeaders1) {
      outputHeaders.push_back(headers1.at(nonCommonHeader));
    }

    for (int nonCommonHeader : nonCommonHeaders2) {
      outputHeaders.push_back(headers2.at(nonCommonHeader));
    }

    // separate rows in table into keys and values

    std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>> hashmap1;
    for (std::list<std::string> row : t1->rows) {
      std::vector<std::string> rowVec(row.begin(), row.end());
      std::vector<std::string> keys;
      std::vector<std::string> values;
      for (int i = 0; i < row.size(); i++) {
        if (std::find(commonHeaders1.begin(), commonHeaders1.end(), i) != commonHeaders1.end()) {
          keys.push_back(rowVec.at(i));
        } else {
          values.push_back(rowVec.at(i));
        }
      }

      hashmap1.push_back(std::make_pair(keys, values));
    }


    std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>> hashmap2;
    for (std::list<std::string> row : t2->rows) {
      std::vector<std::string> rowVec(row.begin(), row.end());
      std::vector<std::string> keys;
      std::vector<std::string> values;
      for (int i = 0; i < row.size(); i++) {
        if (std::find(commonHeaders2.begin(), commonHeaders2.end(), i) != commonHeaders2.end()) {
          // i is in common headers, add to keys
          keys.push_back(rowVec.at(i));
        } else {
          // i is not in common headers, add to values
          values.push_back(rowVec.at(i));
        }
      }
      hashmap2.push_back(std::make_pair(keys, values));
    }

    // generate new table

    for (auto kv : hashmap1) {
      std::vector<std::string> key1 = kv.first;
      std::vector<std::string> value1 = kv.second;

      // Look up the key in the second table
      for (auto kv2 : hashmap2) {
        std::vector<std::string> key2 = kv2.first;
        std::vector<std::string> value2 = kv2.second;
        if (key1 == key2) {
          // append values from t1 and t2
          std::list<std::string> res;
          for (std::string s : key1) {
              res.push_back(s);
          }
          for (std::string s : value1) {
              res.push_back(s);
          }
          for (std::string s : value2) {
              res.push_back(s);
          }
          outputColumns.push_back(res);
        }
      }
    }

  } else {  // when there are no common headers
      std::vector<std::list<std::string>> rows1 = t1->rows;
      std::vector<std::list<std::string>> rows2 = t2->rows;

      // append all headers
      for (std::string header : headers1) {
        outputHeaders.push_back(header);
      }

      for (std::string header : headers2) {
        outputHeaders.push_back(header);
      }

      for (std::list<std::string> row1 : rows1) {
        for (std::list<std::string> row2 : rows2) {
          std::list<std::string> concat;
          for (std::string s : row1) {
            concat.push_back(s);
          }
          for (std::string s : row2) {
            concat.push_back(s);
          }

          outputColumns.push_back(concat);
        }
      }
  }

  if (outputColumns.size() == 0) {
    BoolResult *boolResult = new BoolResult(false);
    return boolResult;
  }

  TableResult* tableResult1 = new TableResult(outputHeaders, outputColumns);
  return tableResult1;
}
