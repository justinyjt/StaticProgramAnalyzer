#pragma once
#include <string>
#include <list>
#include <vector>
#include <iostream>
#include <utility>
#include <type_traits>
#include <unordered_set>
#include "commons/types.h"
#include "qps/pql/Synonym.h"

/*
represents result of clause eval or query eval (multiple clause evals)
*/
class Result {
 public:
    virtual void output(std::list<std::string>&) = 0;
    virtual Result* merge(Result* rhs) = 0;
};

// scalar result
class BoolResult : public Result {
  bool b;

 public:
    explicit BoolResult(bool b) : b(b) { }

    void output(std::list<std::string>& list) override {
      list.push_back(b ? "true" : "false");
    }

    Result* merge(Result* rhs) override {
      if (b) {  // true
        return rhs;
      } else {  // false
        return this;
      }
    }
};

// n-col result
class TableResult : public Result {
    std::list<std::string> idents;  // s1, s2
    std::vector<std::list<std::string>> rows;  // vec<list<1,2>, list<2,3>, ...>

 public:
    // general for n-cols
    explicit TableResult(std::list<std::string>& idents,
          const std::vector<std::list<std::string>>& vec) : idents(idents) {
        rows.insert(rows.end(), vec.begin(), vec.end());
    }

    // for 2 cols
    explicit TableResult(const std::string& ident1, const std::string& ident2,
          STMT_ENT_SET& set) {
      idents.push_back(ident1);
      idents.push_back(ident2);
      for (auto& p : set) {
        rows.push_back({std::to_string(p.first), p.second});
      }
    }

    // for 2 cols
    explicit TableResult(const std::string& ident1, const std::string& ident2,
          const std::vector<std::list<std::string>>& vec) {
      idents.push_back(ident1);
      idents.push_back(ident2);
      rows.insert(rows.end(), vec.begin(), vec.end());
    }

    // for 1 col
    explicit TableResult(const std::string& ident, ENT_SET& set) {
      idents.push_back(ident);
      for (auto& elem : set)
        rows.push_back({elem});
    }

    // for 1 col
    explicit TableResult(const std::string& ident, STMT_SET& set) {
      idents.push_back(ident);
      for (auto& elem : set)
        rows.push_back({std::to_string(elem)});
    }

    void output(std::list<std::string>& list) override {
      if (idents.size() > 1) {
        throw std::runtime_error("");
      }

      for (auto& elem : rows) {
        list.push_back(elem.front());
      }
    }

    Result* merge(Result* rhs) override {
      if (dynamic_cast<BoolResult*>(rhs) != nullptr) {
        return rhs->merge(this);
      }
      return nullptr;
    }
};
