#include <string>
#include "DeclarationParser.h"

std::vector<Synonym> DeclarationParser::parse(const std::unique_ptr<Lexer> &lexer) {
    std::vector<Synonym> declarationList;
    while (1) {
        Token checkSelect = lexer->scan();
        if (checkSelect.getLexeme() == "Select") {
            std::string s = checkSelect.getLexeme();
            break;
        } else if (checkSelect.getLexeme() == "variable") {
            Token nextToken = lexer->scan();
            Synonym s(Synonym::DesignEntity::VARIABLE, nextToken.getLexeme());
            declarationList.push_back(s);
            lexer->scan();
        } else if (checkSelect.getLexeme() == "assign") {
            Token nextToken = lexer->scan();
            Synonym s(Synonym::DesignEntity::ASSIGN, nextToken.getLexeme());
            declarationList.push_back(s);
            lexer->scan();
        }
    }
    return declarationList;
}
