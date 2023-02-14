#pragma once

#include <string>
#include <list>
#include <vector>
#include <iostream>
#include <unordered_set>
#include "commons/types.h"
/*
represents result of clause eval or query eval (multiple clause evals)
*/
class Result {
 public:
    virtual void output(std::list<std::string>&) = 0;
};

class StrResult : public Result {
    std::vector<std::string> results;

 public:
    explicit StrResult(const std::unordered_set<std::string>& set) {
      for (auto& elem : set) {
        results.push_back(elem);
      }
    }

    void output(std::list<std::string>& list) override {
      for (std::string& elem : results) {
        list.push_back(elem);
      }
    }
};

class IntResult : public Result {
  std::vector<int> results;

 public:
    explicit IntResult(const std::unordered_set<int>& set) {
      for (auto& elem : set) {
        results.push_back(elem);
      }
    }

    void output(std::list<std::string>& list) override {
      for (int elem : results) {
        list.push_back(std::to_string(elem));
      }
    }
};

class BoolResult : public Result {
  bool b;

 public:
    explicit BoolResult(bool b) : b(b) { }

    void output(std::list<std::string>& list) override {
      list.push_back(b ? "true" : "false");
    }
};
