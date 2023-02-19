#pragma once

#include <string>
#include <memory>

#include "DesignExtractor.h"
#include "IParser.h"

class SourceProcessor {
 public:
    explicit SourceProcessor(std::unique_ptr<PKBWriter> pkb);
    bool process(std::string source);
 private:
    std::unique_ptr<PKBWriter> pkb_;
};


