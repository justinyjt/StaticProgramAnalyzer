#include <cassert>

#include "WithNumClause.h"

WithNumClause::WithNumClause(std::unique_ptr<PQLToken> first, std::unique_ptr<PQLToken> second) :
        TwoArgClause(std::move(first), std::move(second)) {}

STMT_SET WithNumClause::getStmtNumsFromSyn(Synonym &syn, PKBReader *db) {
    switch (syn.de) {
        case Synonym::DesignEntity::STMT:
            return db->getStatements(StmtType::None);
        case Synonym::DesignEntity::READ:
            return db->getStatements(StmtType::Read);
        case Synonym::DesignEntity::PRINT:
            return db->getStatements(StmtType::Print);
        case Synonym::DesignEntity::CALL:
            return db->getStatements(StmtType::Call);
        case Synonym::DesignEntity::WHILE:
            return db->getStatements(StmtType::While);
        case Synonym::DesignEntity::IF:
            return db->getStatements(StmtType::If);
        case Synonym::DesignEntity::ASSIGN:
            return db->getStatements(StmtType::Assign);
        default:
            throw std::runtime_error("");
    }
}

std::unique_ptr<Result> WithNumClause::handleSynNumCase(PKBReader *db, Synonym &syn, const std::string &num) {
    if (syn.de == Synonym::DesignEntity::CONSTANT) {
        return handleOneConstCaseNum(db, syn.str(), num);
    }
    return handleNoConstCaseNum(db, syn, std::stoi(num));
}

std::unique_ptr<Result> WithNumClause::handleSynSynCase(PKBReader *db, Synonym &syn1, Synonym &syn2) {
    if (syn1.str() == syn2.str()) {
        return handleSameSynCase(db, syn1);
    }

    if (syn1.de == Synonym::DesignEntity::CONSTANT && syn2.de == Synonym::DesignEntity::CONSTANT) {
        return handleTwoConstCase(db, syn1.str(), syn2.str());
    } else if (syn1.de == Synonym::DesignEntity::CONSTANT) {
        return handleOneConstCaseSyn(db, syn1.str(), syn2);
    } else if (syn2.de == Synonym::DesignEntity::CONSTANT) {
        return handleOneConstCaseSyn(db, syn2.str(), syn1);
    }
    return handleNoConstCaseSyn(db, syn1, syn2);
}

std::unique_ptr<Result> WithNumClause::handleTwoConstCase(
        PKBReader *db, const std::string &syn1, const std::string &syn2) {
    ENT_SET constValues = db->getEntities(Entity::Constant);
    ENT_ENT_SET resultSet;
    for (auto const &constant : constValues) {
        resultSet.emplace(ENT_ENT(constant, constant));
    }
    std::unique_ptr<Result> res = std::make_unique<TableResult>(syn1, syn2, resultSet);
    return std::move(res);
}

std::unique_ptr<Result> WithNumClause::handleOneConstCaseSyn(
        PKBReader *db, const std::string &constSyn, Synonym &nonConstSyn) {
    ENT_SET constValues = db->getEntities(Entity::Constant);
    STMT_SET stmtValues = getStmtNumsFromSyn(nonConstSyn, db);
    ENT_ENT_SET resultSet;
    for (auto const &stmt : stmtValues) {
        std::string stmtStr = std::to_string(stmt);
        if (constValues.find(stmtStr) != constValues.end()) {
            resultSet.emplace(ENT_ENT(stmtStr, stmtStr));
        }
    }
    std::unique_ptr<Result> res = std::make_unique<TableResult>(constSyn, nonConstSyn.str(), resultSet);
    return std::move(res);
}

std::unique_ptr<Result> WithNumClause::handleNoConstCaseSyn(PKBReader *db, Synonym &syn1, Synonym &syn2) {
    STMT_SET syn1Vals = getStmtNumsFromSyn(syn1, db);
    STMT_SET syn2Vals = getStmtNumsFromSyn(syn2, db);
    STMT_STMT_SET resultSet;
    for (auto const &syn1Val : syn1Vals) {
        if (syn2Vals.find(syn1Val) != syn2Vals.end()) {
            resultSet.emplace(syn1Val, syn1Val);
        }
    }
    std::unique_ptr<Result> res = std::make_unique<TableResult>(syn1.str(), syn2.str(), resultSet);
    return std::move(res);
}

std::unique_ptr<Result> WithNumClause::handleOneConstCaseNum(
        PKBReader *db, const std::string &constSyn, const std::string &num) {
    ENT_SET constValues = db->getEntities(Entity::Constant);
    ENT_SET resultSet;
    std::string stmtStr = num;
    for (auto const &constVal : constValues) {
        if (constVal == stmtStr) {
            resultSet.emplace(constVal);
        }
    }
    std::unique_ptr<Result> res = std::make_unique<TableResult>(constSyn, resultSet);
    return std::move(res);
}

std::unique_ptr<Result> WithNumClause::handleNoConstCaseNum(PKBReader *db, Synonym &syn, const STMT_NUM &num) {
    STMT_SET synVals = getStmtNumsFromSyn(syn, db);
    STMT_SET resultSet;
    for (auto const &synVal : synVals) {
        if (synVal == num) {
            resultSet.emplace(synVal);
        }
    }
    std::unique_ptr<Result> res = std::make_unique<TableResult>(syn.str(), resultSet);
    return std::move(res);
}

std::unique_ptr<Result> WithNumClause::handleSameSynCase(PKBReader *db, Synonym &syn) {
    std::unique_ptr<Result> res;
    if (syn.de == Synonym::DesignEntity::CONSTANT) {
        ENT_SET constSet = db->getEntities(Entity::Constant);
        res = std::make_unique<TableResult>(syn.str(), constSet);
    } else {
        STMT_SET stmtSet = getStmtNumsFromSyn(syn, db);
        res = std::make_unique<TableResult>(syn.str(), stmtSet);
    }
    return std::move(res);
}

std::unique_ptr<Result> WithNumClause::evaluate(PKBReader *db) {
    /* <SYNONYM | NUM>, <SYNONYM | NUM> */

    switch (getPairEnum()) {
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::SYNONYM):  // with syn.x = syn.y -> syn_type[]
        {
            Synonym syn1 = dynamic_cast<Synonym &>(*first_);
            Synonym syn2 = dynamic_cast<Synonym &>(*second_);
            return handleSynSynCase(db, syn1, syn2);
        }
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::STMT_NUM):  // with syn.x = "x" -> syn_type[]
        {
            Synonym syn1 = dynamic_cast<Synonym &>(*first_);
            std::string num = (dynamic_cast<PQLNumber &>(*second_)).n;
            return handleSynNumCase(db, syn1, num);
        }
        case pairEnum(PQLToken::Tag::STMT_NUM, PQLToken::Tag::SYNONYM):  // with syn.x = "x" -> syn_type[]
        {
            Synonym syn2 = dynamic_cast<Synonym &>(*second_);
            std::string num = (dynamic_cast<PQLNumber &>(*first_)).n;
            return handleSynNumCase(db, syn2, num);
        }
        case pairEnum(PQLToken::Tag::STMT_NUM, PQLToken::Tag::STMT_NUM):  // Uses/Modifies(1, "x") -> bool
        {
            std::string num1 = (dynamic_cast<PQLNumber &>(*first_)).n;
            std::string num2 = (dynamic_cast<PQLNumber &>(*second_)).n;
            std::unique_ptr<Result> result = std::make_unique<BoolResult>(num1 == num2);
            return std::move(result);
        }
        default:
            throw std::runtime_error("");
    }
}

void WithNumClause::validateArgs() {}

bool WithNumClause::operator==(const Clause &rhs) const {
    const auto *pRhs = dynamic_cast<const WithNumClause *>(&rhs);
    return pRhs != nullptr && TwoArgClause::equal(*pRhs);
}
