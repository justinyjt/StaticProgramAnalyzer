#pragma once
#include <string>
#include <list>
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
    Tag tag;
    explicit Result(Tag);
    virtual void output(std::list<std::string>&) = 0;
    static Result* join(Result*, Result*);
};
