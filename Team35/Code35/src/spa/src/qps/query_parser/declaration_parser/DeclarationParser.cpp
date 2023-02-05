#include <string>
#include "DeclarationParser.h"

std::vector<Synonym> DeclarationParser::parse(std::unique_ptr<Lexer> lexer) {
    std::vector<Synonym> declarationList;
    while (1) {
        Token checkSelect = lexer->Scan();
        if (checkSelect.GetLexeme() == "Select") {
            break;
        } else if (checkSelect.GetLexeme() == "Variable") {
            Token nextToken = lexer->Scan();
            Synonym s(Synonym::DesignEntity::Variable, nextToken.GetLexeme());
            declarationList.push_back(s);
            lexer->Scan();
        } else if (checkSelect.GetLexeme() == "Assign") {
            Token nextToken = lexer->Scan();
            Synonym s(Synonym::DesignEntity::Assign, nextToken.GetLexeme());
            declarationList.push_back(s);
            lexer->Scan();
        }
    }
    return declarationList;
}
