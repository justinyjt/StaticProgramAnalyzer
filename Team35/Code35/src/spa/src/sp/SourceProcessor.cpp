#include "SourceProcessor.h"
#include "Parser.h"
#include <string>
#include <memory>

SourceProcessor::SourceProcessor(DesignExtractor &de) : design_extractor_(de) {}

void SourceProcessor::process(std::string source) {
    std::unique_ptr<IParser> parser = std::make_unique<Parser>(source, design_extractor_);
    parser->Parse();
}
