#include "SourceProcessor.h"

#include <memory>
#include <utility>

#include "commons/lexer/exception/LexerException.h"
#include "commons/lexer/LexerFactory.h"
#include "commons/token_scanner/SimpleTokenScanner.h"
#include "Parser.h"
#include "SourceValidator.h"

SourceProcessor::SourceProcessor(std::unique_ptr<PKBWriter> pkb) : pkb_(std::move(pkb)) {}

bool SourceProcessor::process(const std::string &source) {
    try {
        std::unique_ptr<ILexer> lex = std::move(LexerFactory::createLexer(source, LexerFactory::LexerType::Simple));
        SimpleTokenScanner scanner(std::move(lex));
        SourceValidator sv(scanner);
        if (!sv.validate()) {
            return false;
        }
        scanner.reset();
        std::unique_ptr<IParser> parser = std::make_unique<Parser>(scanner);
        DesignExtractor designExtractor(std::move(pkb_));
        designExtractor.extractProgram(parser->parse());
        return true;
    } catch (LexerException &e) {
        return false;
    }
}
