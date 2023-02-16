#include "SourceProcessor.h"

#include <memory>
#include <utility>

#include "Parser.h"
#include "SyntaxValidator.h"

SourceProcessor::SourceProcessor(std::unique_ptr<PKBWriter> pkb) : pkb_(std::move(pkb)) {}

void SourceProcessor::process(std::string source) {
    std::unique_ptr<ILexer> lex =
            std::move(LexerFactory::createLexer(source, LexerFactory::LexerType::Simple));
    std::unique_ptr<SyntaxValidator> sv = std::make_unique<SyntaxValidator>(std::move(lex));
    sv->validateProgram();
    std::unique_ptr<IParser> parser =
        std::make_unique<Parser>(source, std::move(pkb_), sv->getTokenLst());
    parser->Parse();
}
