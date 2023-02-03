#include <string>
#include "DeclarationParser.h"

std::vector<std::string> DeclarationParser::parse(Lexer &lexer) {

    std::vector<std::string> declarationList;
    while(true) {
        Token checkSelect = lexer.Scan();
        if (checkSelect.GetLexeme() == "Select") {
            break;
        }
        Token nextToken = lexer.Scan();
        declarationList.push_back(nextToken.GetLexeme());
        lexer.Scan();

        lexer.Scan();
    }
    return declarationList;
}
