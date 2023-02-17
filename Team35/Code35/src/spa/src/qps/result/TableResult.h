#pragma once
#include "qps/result/Result.h"

// n-col result
class TableResult : public Result {
 public:
    std::unordered_set<std::string> idents;  // eg { s1, s2 }
    std::vector<std::list<std::string>> rows;  // eg vector<list<1,"x">, list<2,"y">, ...>

    // general constructor for n-cols
    TableResult(std::unordered_set<std::string>& _idents,
          const std::vector<std::list<std::string>>& _rows) : Result(Tag::TABLE) {
        idents.insert(_idents.begin(), _idents.end());
        rows.insert(rows.end(), _rows.begin(), _rows.end());
    }

    // constructor for empty table
    TableResult() : Result(Tag::TABLE) {};

    // for 2 cols with STMT_ENT_SET
    TableResult(const std::string& ident1, const std::string& ident2,
          STMT_ENT_SET& set) : Result(Tag::TABLE) {
      idents.insert(ident1);
      idents.insert(ident2);
      for (auto& p : set) {
        rows.emplace_back(
          std::initializer_list<std::string>
              {std::to_string(p.first), p.second});
      }
    }

    // for 2 cols with STMT_STMT_SET
    TableResult(const std::string& ident1, const std::string& ident2,
          STMT_STMT_SET& set) : Result(Tag::TABLE) {
      idents.insert(ident1);
      idents.insert(ident2);
      for (auto& p : set) {
        rows.emplace_back(
          std::initializer_list<std::string>
            {std::to_string(p.first), std::to_string(p.second)});
      }
    }

    // for 2 cols with vector<list<string>> 
    TableResult(const std::string& ident1, const std::string& ident2,
          const std::vector<std::list<std::string>>& vec) : Result(Tag::TABLE) {
      idents.insert(ident1);
      idents.insert(ident2);
      rows.insert(rows.end(), vec.begin(), vec.end());
    }

    // for 1 col with ENT_SET
    TableResult(const std::string& ident, ENT_SET& set) : Result(Tag::TABLE) {
      idents.insert(ident);
      for (auto& elem : set)
        rows.emplace_back(
          std::initializer_list<std::string>{elem});
    }

    // for 1 col with STMT_SET
    TableResult(const std::string& ident, STMT_SET& set) : Result(Tag::TABLE) {
      idents.insert(ident);
      for (auto& elem : set)
        rows.emplace_back(
          std::initializer_list<std::string>{std::to_string(elem)});
    }

    void output(std::list<std::string>& list) override {
      if (idents.size() > 1) {
        throw std::runtime_error("Multi row table");
      }

      for (auto& elem : rows) {
        list.push_back(elem.front());
      }
    }
};
