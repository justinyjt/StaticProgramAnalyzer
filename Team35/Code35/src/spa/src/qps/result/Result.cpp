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
  std::unordered_set<int> commonHeaders1;
  std::unordered_set<int> commonHeaders2;
  std::list<int> nonCommonHeaders1;
  std::list<int> nonCommonHeaders2;
  std::list<std::string> outputHeaders;
  std::vector<std::list<std::string>> outputColumns;

  // find common headers for table 1 and 2
  for (int i = 0; i < headers1.size(); i++) {
    for (int j = 0; j < headers2.size(); j++) {
      if (headers1[i] == headers2[j]) {
        commonHeaders1.insert(i);
        commonHeaders2.insert(j);
      }
    }
  }

  // find non common headers for table 1
  for (int i = 0; i < headers1.size(); i++) {
    if (commonHeaders1.find(i) == commonHeaders1.end()) {
      nonCommonHeaders1.push_back(i);
    }
  }

  // find non common headers for table 2
  for (int i = 0; i < headers2.size(); i++) {
    if (commonHeaders2.find(i) == commonHeaders2.end()) {
      nonCommonHeaders2.push_back(i);
    }
  }

  // if no matching headers
  if (commonHeaders1.size() == 0) {
      // append all headers
      outputHeaders.insert(outputHeaders.end(), headers1.begin(), headers1.end());
      outputHeaders.insert(outputHeaders.end(), headers2.begin(), headers2.end());

      for (std::list<std::string> row1 : t1->rows) {
        for (std::list<std::string> row2 : t2->rows) {
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
    for (std::list<std::string>& row : t1->rows) {
      std::vector<std::string> keys;
      std::vector<std::string> values;
      int i = 0;
      for (std::string& val : row) {
        if (commonHeaders1.find(i) != commonHeaders1.end()) {
          keys.push_back(val);
        } else {
          values.push_back(val);
        }
        i++;
      }
      hashmap1.push_back(std::make_pair(keys, values));
    }


    std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>> hashmap2;
    for (std::list<std::string> row : t2->rows) {
      std::vector<std::string> keys;
      std::vector<std::string> values;
      int i = 0;
      for (std::string& val : row) {
        if (commonHeaders2.find(i) != commonHeaders2.end()) {
          keys.push_back(val);
        } else {
          values.push_back(val);
        }
        i++;
      }
      hashmap2.push_back(std::make_pair(keys, values));
    }

    // generate new table
    for (auto kv : hashmap1) {
      std::vector<std::string>& key1 = kv.first;
      std::vector<std::string>& value1 = kv.second;

      // Look up the key in the second table
      for (auto kv2 : hashmap2) {
        std::vector<std::string>& key2 = kv2.first;
        std::vector<std::string>& value2 = kv2.second;
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

  if (outputColumns.size() == 0) {
    BoolResult *boolResult = new BoolResult(false);
    return boolResult;
  }

  TableResult* tableResult = new TableResult(outputHeaders, outputColumns);
  return tableResult;
}
