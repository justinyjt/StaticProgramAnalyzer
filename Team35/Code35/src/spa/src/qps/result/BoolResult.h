#include "qps/result/Result.h"

// scalar result
class BoolResult : public Result {
 public:
    bool b;
    explicit BoolResult(bool b) : Result(Tag::BOOL), b(b) { }

    void output(std::list<std::string>& list) override {
      list.push_back(b ? "true" : "false");
    }
};