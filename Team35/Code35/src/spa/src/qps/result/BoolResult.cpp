#include "BoolResult.h"

BoolResult::BoolResult(bool b) : Result(Tag::BOOL), b(b) {}

void BoolResult::output(std::list<std::string>& list) {
  list.push_back(b ? "true" : "false");
}
