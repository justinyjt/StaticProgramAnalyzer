#pragma once

#include "DesignExtractor.h"
#include "IParser.h"

class SourceProcessor {
 public:
    explicit SourceProcessor(DesignExtractor &designExtractor);
    void process(std::string source);
 private:
    DesignExtractor &design_extractor_;
};


