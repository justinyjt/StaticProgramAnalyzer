#include "DeclarationParser.h"

#include <string>

#include "commons/token/Token.h"
#include "qps/query_parser/clause_parser/TokenValidator.h"

std::vector<Synonym> DeclarationParser::parse(TokenValidator &tokenValidator) {
    std::vector<Synonym> declarationList;
    while (1) {
        std::unique_ptr<Token> checkSelect = tokenValidator.getNextToken();
        if (checkSelect->getLexeme() == "Select") {
            std::string s = checkSelect->getLexeme();
            break;
        } else if (checkSelect->getLexeme() == "variable") {
            std::unique_ptr<Token> nextToken = tokenValidator.getNextToken();
            Synonym s(Synonym::DesignEntity::VARIABLE, nextToken->getLexeme());
            declarationList.push_back(s);
            tokenValidator.getNextToken();
        } else if (checkSelect->getLexeme() == "assign") {
            std::unique_ptr<Token> nextToken = tokenValidator.getNextToken();
            Synonym s(Synonym::DesignEntity::ASSIGN, nextToken->getLexeme());
            declarationList.push_back(s);
            tokenValidator.getNextToken();
        }
    }
    return declarationList;
}
