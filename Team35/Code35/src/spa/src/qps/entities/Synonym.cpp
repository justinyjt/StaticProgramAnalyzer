#include "Synonym.h"
#include <utility>

Synonym::Synonym(DesignEntity synonym, std::string declaration) : Term::Term(Term::Tag::Synonym),
        synonym(synonym), declaration(std::move(declaration)) {}

Synonym::DesignEntity Synonym::getDesignEntity() {
    return synonym;
}

std::string Synonym::getDeclaration() {
    return declaration;
}
