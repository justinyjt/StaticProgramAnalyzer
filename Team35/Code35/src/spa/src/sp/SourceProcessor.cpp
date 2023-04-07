#include "SourceProcessor.h"

#include <memory>
#include <utility>

#include "commons/lexer/exception/LexerException.h"
#include "Parser.h"
#include "SourceValidator.h"

SourceProcessor::SourceProcessor(std::unique_ptr<PKBWriter> pkb) : pkb_(std::move(pkb)) {}

bool SourceProcessor::process(const std::string &source) {
    try {
        std::unique_ptr<ILexer> lex =
                std::move(LexerFactory::createLexer(source, LexerFactory::LexerType::Simple));
        std::unique_ptr<SourceValidator> sv = std::make_unique<SourceValidator>(std::move(lex));
        if (!sv->validateProgram()) {
            return false;
        }
        std::unique_ptr<IParser> parser = std::make_unique<Parser>(sv->getTokenLst());
        DesignExtractor designExtractor(std::move(pkb_));
        designExtractor.extractProgram(parser->Parse());
        return true;
    } catch (LexerException &e) {
        return false;
    }
}
