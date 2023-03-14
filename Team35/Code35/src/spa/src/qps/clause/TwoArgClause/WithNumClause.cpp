#include <cassert>

#include "WithNumClause.h"

WithNumClause::WithNumClause(std::unique_ptr<PQLToken> first, std::unique_ptr<PQLToken> second) : TwoArgClause(std::move(first), std::move(second)) {};

STMT_SET WithNumClause::getNumValuesFromSyn(Synonym syn, PKBReader* db) {
    switch (syn.de) {
        case Synonym::DesignEntity::CONSTANT: {
            STMT_SET s = STMT_SET();
            ENT_SET constants = db->getEntities(Entity::Constant);
            for (auto const& constant : constants) {
                s.emplace(std::stoi(constant));
            }
            return s;
        }
        case Synonym::DesignEntity::STMT: {
            return db->getStatements(StmtType::None);
        }
        case Synonym::DesignEntity::READ: {
            return db->getStatements(StmtType::Read);
        }
        case Synonym::DesignEntity::PRINT:
        {
            return db->getStatements(StmtType::Print);
        }
        case Synonym::DesignEntity::CALL:
        {
            return db->getStatements(StmtType::Call);
        }
        case Synonym::DesignEntity::WHILE:
        {
            return db->getStatements(StmtType::While);
        }
        case Synonym::DesignEntity::IF:
        {
            return db->getStatements(StmtType::If);
        }
        case Synonym::DesignEntity::ASSIGN:
        {
            return db->getStatements(StmtType::Assign);
        }
        default:
            assert(false);
    }
}

std::unique_ptr<Result> WithNumClause::evaluate(PKBReader* db) {
    /* <SYNONYM | NUM>, <SYNONYM | NUM> */

    switch (getPairEnum()) {
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::SYNONYM):  // with syn.x = syn.y -> syn_type[]
        {
            Synonym syn1 = dynamic_cast<Synonym&>(*first);
            Synonym syn2 = dynamic_cast<Synonym&>(*second);
            STMT_SET syn1Vals = getNumValuesFromSyn(syn1, db);
            STMT_SET syn2Vals = getNumValuesFromSyn(syn2, db);
            STMT_STMT_SET resultSet;
            for (auto const& syn1Val : syn1Vals) {
                for (auto const& syn2Val : syn2Vals) {
                    if (syn1Val == syn2Val) {
                        resultSet.emplace(STMT_STMT(syn1Val, syn2Val));
                    }
                }
            }
            std::unique_ptr<Result> res = std::make_unique<TableResult>(first->str(), second->str(), resultSet);
            return std::move(res);
        }
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::STMT_NUM):  // with syn.x = "x" -> syn_type[]
        {
            Synonym syn1 = dynamic_cast<Synonym&>(*first);
            STMT_NUM num = (dynamic_cast<StatementNumber&>(*second)).n;
            STMT_SET syn1Vals = getNumValuesFromSyn(syn1, db);
            STMT_SET resultSet;
            for (auto const& syn1Val : syn1Vals) {
                if (syn1Val == num) {
                    resultSet.emplace(syn1Val);
                }
            }
            std::unique_ptr<Result> result = std::make_unique<TableResult>(first->str(), resultSet);
            return std::move(result);
        }
        case pairEnum(PQLToken::Tag::STMT_NUM, PQLToken::Tag::SYNONYM):  // with syn.x = "x" -> syn_type[]
        {
            Synonym syn2 = dynamic_cast<Synonym&>(*second);
            STMT_NUM num = (dynamic_cast<StatementNumber&>(*first)).n;
            STMT_SET syn2Vals = getNumValuesFromSyn(syn2, db);
            STMT_SET resultSet;
            for (auto const& syn2Val : syn2Vals) {
                if (syn2Val == num) {
                    resultSet.emplace(syn2Val);
                }
            }
            std::unique_ptr<Result> result = std::make_unique<TableResult>(second->str(), resultSet);
            return std::move(result);
        }
        case pairEnum(PQLToken::Tag::STMT_NUM, PQLToken::Tag::STMT_NUM):  // Uses/Modifies(1, "x") -> bool
        {
            STMT_NUM num1 = (dynamic_cast<StatementNumber&>(*first)).n;
            STMT_NUM num2 = (dynamic_cast<StatementNumber&>(*second)).n;
            std::unique_ptr<Result> result = std::make_unique<BoolResult>(num1 == num2);
            return std::move(result);
        }
        default:
            throw std::runtime_error("");
    }
}

void WithNumClause::validateArgs() {}

bool WithNumClause::operator==(const Clause &rhs) const {
    const auto* pRhs = dynamic_cast<const WithNumClause*>(&rhs);
    return pRhs != nullptr && TwoArgClause::equal(*pRhs);
}
