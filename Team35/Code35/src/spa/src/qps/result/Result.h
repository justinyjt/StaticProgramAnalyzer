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
};

// scalar result
class BoolResult : public Result {
  bool b;

 public:
    explicit BoolResult(bool b) : b(b) { }

    void output(std::list<std::string>& list) override {
      list.push_back(b ? "true" : "false");
    }
};

// 2-col result
template <class T, class U>
class TwoColResult : public Result {
    std::vector<std::pair<T, U>> rows;

 public:
    const std::string ident1;
    const std::string ident2;
    explicit TwoColResult(std::string ident1, std::string ident2,
      const std::vector<std::pair<T, U>>& vec) : ident1(ident1), ident2(ident2) {
        rows.insert(rows.end(), vec.begin(), vec.end());
    }

    explicit TwoColResult(std::string ident1, std::string ident2,
      const std::unordered_set<std::pair<T, U>>& set) : ident1(ident1), ident2(ident2) {
        for (auto& p : set) {
          rows.push_back(p);
        }
    }

    void output(std::list<std::string>& list) override {
      throw std::runtime_error("");
    }
};

// 1-col result
template <class V>
class OneColResult : public Result {
    std::vector<V> rows;

 public:
    std::string ident;
    explicit OneColResult(std::string ident, const std::unordered_set<V>& set):
      ident(ident) {
      for (auto& elem : set)
        rows.push_back(elem);
    }

    explicit OneColResult(std::string ident, const std::vector<V>& vec):
    ident(ident) {
      rows.insert(rows.end(), vec.begin(), vec.end());
    }

    void output(std::list<std::string>& list) override {
      for (auto& elem : rows) {
        if constexpr (std::is_same<V, int>::value) {
          list.push_back(std::to_string(elem));
        }               if constexpr (std::is_same<V, std::string>::value) {
          list.push_back(elem);
        }
      }
    }
};
