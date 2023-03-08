#include "DeclarationParser.h"
#include "qps/query_exceptions/SyntaxException.h"
#include "qps/query_exceptions/SemanticException.h"

DeclarationParser::DeclarationParser(PQLTokenScanner &pqlTokenScanner, std::vector<Synonym>& synonyms) :
        pqlTokenScanner(pqlTokenScanner), synonyms(synonyms) {}

std::vector<Synonym> DeclarationParser::parse() {

    // check for next declaration
    while (true) {
        if (pqlTokenScanner.peek(Token::Tag::Select) || pqlTokenScanner.peek(Token::Tag::EndOfFile)) {
            break;
        }

        Synonym::DesignEntity de = parseDesignEntity();

        // check if there are multiple declarations of the same type
        while (true) {
            std::unique_ptr<Synonym> synonym = parseSynonym(de);

            validateAndAddSynonym(std::move(synonym));

            if (pqlTokenScanner.match(Token::Tag::SemiColon)) {
                break;
            } else if (pqlTokenScanner.match(Token::Tag::Comma)) {
                continue;
            } else {
                throw SyntaxException();
            }
        }
    }
    return synonyms;
}

// convert Token tag to Synonym design entity
Synonym::DesignEntity DeclarationParser::parseDesignEntity() {
    if (pqlTokenScanner.peekDesignEntity()) {
        Synonym::DesignEntity de;
        switch (pqlTokenScanner.peekTag()) {
            case Token::Tag::Statement:
                de = Synonym::DesignEntity::STMT;
                break;
            case Token::Tag::Read:
                de = Synonym::DesignEntity::READ;
                break;
            case Token::Tag::Print:
                de = Synonym::DesignEntity::PRINT;
                break;
            case Token::Tag::Call:
                de = Synonym::DesignEntity::CALL;
                break;
            case Token::Tag::While:
                de = Synonym::DesignEntity::WHILE;
                break;
            case Token::Tag::If:
                de = Synonym::DesignEntity::IF;
                break;
            case Token::Tag::Assign:
                de = Synonym::DesignEntity::ASSIGN;
                break;
            case Token::Tag::Variable:
                de = Synonym::DesignEntity::VARIABLE;
                break;
            case Token::Tag::Constant:
                de = Synonym::DesignEntity::CONSTANT;
                break;
            case Token::Tag::Procedure:
                de = Synonym::DesignEntity::PROCEDURE;
                break;
            default:
                assert(false);
        }
        pqlTokenScanner.next();
        return de;
    } else {
        throw SyntaxException();
    }
}

std::unique_ptr<Synonym> DeclarationParser::parseSynonym(Synonym::DesignEntity de) {
    std::unique_ptr<Synonym> synonym;
    if (isValidName(pqlTokenScanner.peekLexeme())) {
        synonym = std::make_unique<Synonym>(de, pqlTokenScanner.peekLexeme());
        pqlTokenScanner.next();
        return synonym;
    } else {
        throw SyntaxException();
    }
}

bool DeclarationParser::isValidName(std::string input) {

    // check syntax validity
    if (!isalpha(input[0])) {
        return false;
    }
    for (char c : input) {
        if (!isalpha(c) && !isdigit(c)) {
            return false;
        }
    }
    return true;
}

void DeclarationParser::validateAndAddSynonym(std::unique_ptr<Synonym> synonym) {
    bool isDeclared = false;
    for (const Synonym& s : synonyms) {
        if (synonym->str() == s.str()) {
            isDeclared = true;
        }
    }
    if (!isDeclared) {
        synonyms.emplace_back(*synonym);
    } else {
        throw SemanticException();
    }
}
