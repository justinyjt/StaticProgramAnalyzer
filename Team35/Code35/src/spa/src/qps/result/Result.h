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
    enum class Tag { BOOL, TABLE, SELECT };
    explicit Result(Tag);

    virtual ~Result() = default;
    virtual void output(std::list<std::string> &) = 0;
    virtual std::unique_ptr<Result> join(Result &) = 0;
    static std::unique_ptr<Result> selectJoin(Result &, Result &);
    static std::unique_ptr<Result> tableJoin(Result &, Result &);
//    std::unique_ptr<Result> projectColumns()
    Tag tag;
};
