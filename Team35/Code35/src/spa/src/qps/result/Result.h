#pragma once
#include <string>
#include <list>
#include <vector>
#include "commons/types.h"
/*
represents result of clause eval or query eval (multiple clause evals)
*/
class Result {
  public:
    virtual void output(std::list<std::string>&);
};

class StrResult : public Result {
  std::vector<std::string> results;
  
  public:
    StrResult(std::unordered_set<std::string> set) {
      for (auto& elem : set) {
        results.push_back(elem);
      }
    }

    void output(std::list<std::string>& list) {
      for (std::string& elem: results) {
        list.push_back(elem);
      }
    }
};

class IntResult : public Result {
  std::vector<int> results;

  public:
    IntResult(std::unordered_set<int> set) {
      for (auto& elem : set) {
        results.push_back(elem);
      }
    }

    void output(std::list<std::string>& list) {
      for (int elem: results) {
        list.push_back(std::to_string(elem));
      }
    }
};
