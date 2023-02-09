#include "DeclarationParser.h"

#include <string>

#include "commons/token/Token.h"

std::vector<Synonym> DeclarationParser::parse(const std::unique_ptr<ILexer> &lexer) {
    std::vector<Synonym> declarationList;
    while (1) {
        std::unique_ptr<Token> checkSelect = lexer->scan();
        if (checkSelect->getLexeme() == "Select") {
            std::string s = checkSelect->getLexeme();
            break;
        } else if (checkSelect->getLexeme() == "variable") {
            std::unique_ptr<Token> nextToken = lexer->scan();
            Synonym s(Synonym::DesignEntity::VARIABLE, nextToken->getLexeme());
            declarationList.push_back(s);
            lexer->scan();
        } else if (checkSelect->getLexeme() == "assign") {
            std::unique_ptr<Token> nextToken = lexer->scan();
            Synonym s(Synonym::DesignEntity::ASSIGN, nextToken->getLexeme());
            declarationList.push_back(s);
            lexer->scan();
        }
    }
    return declarationList;
}
