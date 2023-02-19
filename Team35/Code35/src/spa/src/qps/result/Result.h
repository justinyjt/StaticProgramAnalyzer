#pragma once
#include <string>
#include <list>
#include <memory>
#include <vector>
#include <iostream>
#include <unordered_set>
#include "commons/types.h"

/*
represents result of single clause evaluation 
or query evaluation (multiple clause evaluations)
*/

class Result {
 public:
    enum class Tag { BOOL, TABLE };
    explicit Result(Tag);
    virtual ~Result() = default;
    virtual void output(std::list<std::string> &, std::string &selected) = 0;
    static std::unique_ptr<Result> join(Result *, Result *);
    bool equal(const Result &rhs) const;
 private:
    Tag tag;
    static std::unique_ptr<Result> tableJoin(Result *, Result *);
};
