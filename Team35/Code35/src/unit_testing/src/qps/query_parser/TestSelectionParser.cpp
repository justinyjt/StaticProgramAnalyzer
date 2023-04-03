#include "catch.hpp"
#include "../../TestHelper.h"
#include "qps/query_parser/selection_parser/SelectionParser.h"
#include "commons/lexer/LexerFactory.h"
#include "qps/clause/select_clause/MultipleSynonymSelectClause.h"
#include "qps/query_parser/QuerySyntaxValidator.h"
#include "qps/pql/AttrRef.h"
#include "qps/query_parser/helper.h"
#include "qps/clause/select_clause/BooleanSelectClause.h"
#include "qps/query_exceptions/SemanticException.h"

class setUpSp {
 public:
    setUpSp() {
        // Set up code
        query = "";
        declarationList.insert({"p", Synonym::DesignEntity::PROCEDURE});
        declarationList.insert({"p1", Synonym::DesignEntity::PROCEDURE});
        declarationList.insert({"s", Synonym::DesignEntity::STMT});
        declarationList.insert({"s1", Synonym::DesignEntity::STMT});
        declarationList.insert({"r", Synonym::DesignEntity::READ});
        declarationList.insert({"pn", Synonym::DesignEntity::PRINT});
        declarationList.insert({"a", Synonym::DesignEntity::ASSIGN});
        declarationList.insert({"cl", Synonym::DesignEntity::CALL});
        declarationList.insert({"w", Synonym::DesignEntity::WHILE});
        declarationList.insert({"r", Synonym::DesignEntity::READ});
        declarationList.insert({"ifs", Synonym::DesignEntity::IF});
        declarationList.insert({"v", Synonym::DesignEntity::VARIABLE});
        declarationList.insert({"c", Synonym::DesignEntity::CONSTANT});

        synonymProcedure = std::make_unique<Synonym>(Synonym::DesignEntity::PROCEDURE, "p");
        synonymStatement1 = std::make_unique<Synonym>(Synonym::DesignEntity::STMT, "s");
        synonymStatement2 = std::make_unique<Synonym>(Synonym::DesignEntity::STMT, "s");
        synonymRead = std::make_unique<Synonym>(Synonym::DesignEntity::READ, "r");
        synonymPrint = std::make_unique<Synonym>(Synonym::DesignEntity::PRINT, "pn");
        synonymAssign = std::make_unique<Synonym>(Synonym::DesignEntity::ASSIGN, "a");
        synonymCall = std::make_unique<Synonym>(Synonym::DesignEntity::CALL, "cl");
        synonymWhile = std::make_unique<Synonym>(Synonym::DesignEntity::WHILE, "w");
        synonymIf = std::make_unique<Synonym>(Synonym::DesignEntity::IF, "ifs");
        synonymVariable = std::make_unique<Synonym>(Synonym::DesignEntity::VARIABLE, "v");
        synonymConstant = std::make_unique<Synonym>(Synonym::DesignEntity::CONSTANT, "c");
    }

    ~setUpSp() {
        // Tear down code
    }

    // Declarations
    std::string query;
    std::unordered_map<std::string, Synonym::DesignEntity> declarationList;

    std::unique_ptr<Synonym> synonymProcedure;
    std::unique_ptr<Synonym> synonymPrint;
    std::unique_ptr<Synonym> synonymCall;
    std::unique_ptr<Synonym> synonymRead;
    std::unique_ptr<Synonym> synonymStatement1;
    std::unique_ptr<Synonym> synonymStatement2;
    std::unique_ptr<Synonym> synonymAssign;
    std::unique_ptr<Synonym> synonymWhile;
    std::unique_ptr<Synonym> synonymIf;
    std::unique_ptr<Synonym> synonymVariable;
    std::unique_ptr<Synonym> synonymConstant;


    std::unique_ptr<Clause> clause;
    std::unique_ptr<BooleanSelectClause> booleanSelectClause;
    std::unique_ptr<MultipleSynonymSelectClause> multipleSynonymSelectClause;
    std::unique_ptr<ILexer> lexer;
};

TEST_CASE_METHOD(setUpSp, "Select synonym") {
    query = "Select s";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SelectionParser sp(pqlTokenScanner, declarationList);
    clause = sp.parse();
    std::vector<std::unique_ptr<Synonym>> synonyms;
    synonyms.push_back(std::move(synonymStatement1));
    requireTrue(*clause == *std::make_unique<MultipleSynonymSelectClause>(std::move(synonyms)));
}

TEST_CASE("Select BOOLEAN declared as synonym") {
    std::string query = "Select BOOLEAN";
    std::unordered_map<std::string, Synonym::DesignEntity> declarationList;
    declarationList.insert({"BOOLEAN", Synonym::DesignEntity::STMT});
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SelectionParser sp(pqlTokenScanner, declarationList);
    std::unique_ptr<SelectClause> sc = sp.parse();
    std::vector<std::unique_ptr<Synonym>> synonyms;
    std::unique_ptr<Synonym> s = std::make_unique<Synonym>(Synonym::DesignEntity::STMT, "BOOLEAN");
    synonyms.push_back(std::move(s));
    requireTrue(*sc == *std::make_unique<MultipleSynonymSelectClause>(std::move(synonyms)));
}

TEST_CASE_METHOD(setUpSp, "Select BOOLEAN") {
    std::string query = "Select BOOLEAN";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SelectionParser sp(pqlTokenScanner, declarationList);
    clause = sp.parse();
    requireTrue(*clause == *std::make_unique<BooleanSelectClause>());
}

TEST_CASE_METHOD(setUpSp, "Select attrRef") {
    std::string query = "Select s.stmt#";
    std::unordered_map<std::string, Synonym::DesignEntity> declarationList;
    declarationList.insert({"s", Synonym::DesignEntity::STMT});
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SelectionParser sp(pqlTokenScanner, declarationList);
    std::unique_ptr<SelectClause> sc = sp.parse();
    std::vector<std::unique_ptr<Synonym>> synonyms;
    std::unique_ptr<AttrRef> attrRef = std::make_unique<AttrRef>(Synonym::DesignEntity::STMT, "s", STMT_KEYWORD);
    synonyms.push_back(std::move(attrRef));
    requireTrue(*sc == *std::make_unique<MultipleSynonymSelectClause>(std::move(synonyms)));
}

TEST_CASE_METHOD(setUpSp, "Select tuple, attrRef and synonym") {
    query = "Select <s.stmt#, v>";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SelectionParser sp(pqlTokenScanner, declarationList);
    clause = sp.parse();
    std::vector<std::unique_ptr<Synonym>> synonyms;
    std::unique_ptr<AttrRef> attrRef = std::make_unique<AttrRef>(Synonym::DesignEntity::STMT, "s", STMT_KEYWORD);
    synonyms.push_back(std::move(attrRef));
    synonyms.push_back(std::move(synonymVariable));
    requireTrue(*clause == *std::make_unique<MultipleSynonymSelectClause>(std::move(synonyms)));
}

TEST_CASE_METHOD(setUpSp, "Select tuple, with BOOLEAN as synonym") {
    query = "Select <s.stmt#, BOOLEAN>";
    declarationList.insert({"BOOLEAN", Synonym::DesignEntity::VARIABLE});
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SelectionParser sp(pqlTokenScanner, declarationList);
    clause = sp.parse();
    std::vector<std::unique_ptr<Synonym>> synonyms;
    std::unique_ptr<AttrRef> attrRef = std::make_unique<AttrRef>(Synonym::DesignEntity::STMT, "s", STMT_KEYWORD);
    std::unique_ptr<Synonym> synonym = std::make_unique<Synonym>(Synonym::DesignEntity::VARIABLE, "BOOLEAN");
    synonyms.push_back(std::move(attrRef));
    synonyms.push_back(std::move(synonym));
    requireTrue(*clause == *std::make_unique<MultipleSynonymSelectClause>(std::move(synonyms)));
}

TEST_CASE("Select tuple, with BOOLEAN undeclared, semantic error") {
    std::string query = "Select <s.stmt#, BOOLEAN>";
    std::unordered_map<std::string, Synonym::DesignEntity> declarationList;
    declarationList.insert({"s", Synonym::DesignEntity::STMT});
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SelectionParser sp(pqlTokenScanner, declarationList);
    requireThrowAs<SemanticException>([&sp]() {
        std::unique_ptr<SelectClause> sc = sp.parse();
    });
}

TEST_CASE_METHOD(setUpSp, "Select tuple, NAME types") {
    query = "Select <p.procName, cl.procName, v.varName, r.varName, pn.varName>";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SelectionParser sp(pqlTokenScanner, declarationList);
    clause = sp.parse();
    std::vector<std::unique_ptr<Synonym>> synonyms;
    std::unique_ptr<AttrRef> attrRefProc = std::make_unique<AttrRef>(Synonym::DesignEntity::PROCEDURE,
                                                                     "p", PROCNAME_KEYWORD);
    std::unique_ptr<AttrRef> attrRefCall = std::make_unique<AttrRef>(Synonym::DesignEntity::CALL, "cl",
                                                                     PROCNAME_KEYWORD);
    std::unique_ptr<AttrRef> attrRefVar = std::make_unique<AttrRef>(Synonym::DesignEntity::VARIABLE, "v",
                                                                    VARNAME_KEYWORD);
    std::unique_ptr<AttrRef> attrRefRead = std::make_unique<AttrRef>(Synonym::DesignEntity::READ, "r",
                                                                     VARNAME_KEYWORD);
    std::unique_ptr<AttrRef> attrRefPrint = std::make_unique<AttrRef>(Synonym::DesignEntity::PRINT, "pn",
                                                                      VARNAME_KEYWORD);
    synonyms.push_back(std::move(attrRefProc));
    synonyms.push_back(std::move(attrRefCall));
    synonyms.push_back(std::move(attrRefVar));
    synonyms.push_back(std::move(attrRefRead));
    synonyms.push_back(std::move(attrRefPrint));
    requireTrue(*clause == *std::make_unique<MultipleSynonymSelectClause>(std::move(synonyms)));
}

TEST_CASE_METHOD(setUpSp, "Select tuple, INTEGER types") {
    query = "Select <s.stmt#, r.stmt#, pn.stmt#, cl.stmt#, w.stmt#, ifs.stmt#, a.stmt#, c.value>";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SelectionParser sp(pqlTokenScanner, declarationList);
    clause = sp.parse();
    std::vector<std::unique_ptr<Synonym>> synonyms;
    std::unique_ptr<AttrRef> attrRefStmt = std::make_unique<AttrRef>(Synonym::DesignEntity::STMT, "s",
                                                                     STMT_KEYWORD);
    std::unique_ptr<AttrRef> attrRefRead = std::make_unique<AttrRef>(Synonym::DesignEntity::READ, "r",
                                                                     STMT_KEYWORD);
    std::unique_ptr<AttrRef> attrRefPrint = std::make_unique<AttrRef>(Synonym::DesignEntity::PRINT, "pn",
                                                                      STMT_KEYWORD);
    std::unique_ptr<AttrRef> attrRefCall = std::make_unique<AttrRef>(Synonym::DesignEntity::CALL, "cl",
                                                                     STMT_KEYWORD);
    std::unique_ptr<AttrRef> attrRefWhile = std::make_unique<AttrRef>(Synonym::DesignEntity::WHILE, "w",
                                                                      STMT_KEYWORD);
    std::unique_ptr<AttrRef> attrRefIf = std::make_unique<AttrRef>(Synonym::DesignEntity::IF, "ifs",
                                                                   STMT_KEYWORD);
    std::unique_ptr<AttrRef> attrRefAssign = std::make_unique<AttrRef>(Synonym::DesignEntity::ASSIGN, "a",
                                                                       STMT_KEYWORD);
    std::unique_ptr<AttrRef> attrRefConst = std::make_unique<AttrRef>(Synonym::DesignEntity::CONSTANT, "c",
                                                                      VALUE_KEYWORD);
    synonyms.push_back(std::move(attrRefStmt));
    synonyms.push_back(std::move(attrRefRead));
    synonyms.push_back(std::move(attrRefPrint));
    synonyms.push_back(std::move(attrRefCall));
    synonyms.push_back(std::move(attrRefWhile));
    synonyms.push_back(std::move(attrRefIf));
    synonyms.push_back(std::move(attrRefAssign));
    synonyms.push_back(std::move(attrRefConst));
    requireTrue(*clause == *std::make_unique<MultipleSynonymSelectClause>(std::move(synonyms)));
}

TEST_CASE_METHOD(setUpSp, "Select attrRef stmt#, invalid synonym procedure") {
    query = "Select p.stmt#";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SelectionParser sp(pqlTokenScanner, declarationList);
    requireThrowAs<SemanticException>([&sp]() {
        sp.parse();
    });
}

TEST_CASE_METHOD(setUpSp, "Select attrRef stmt#, invalid synonym variable") {
    query = "Select v.stmt#";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SelectionParser sp(pqlTokenScanner, declarationList);
    requireThrowAs<SemanticException>([&sp]() {
        sp.parse();
    });
}

TEST_CASE_METHOD(setUpSp, "Select attrRef stmt#, invalid synonym constant") {
    query = "Select c.stmt#";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SelectionParser sp(pqlTokenScanner, declarationList);
    requireThrowAs<SemanticException>([&sp]() {
        sp.parse();
    });
}

TEST_CASE_METHOD(setUpSp, "Select attrRef procName, invalid synonym variable") {
    query = "Select v.procName";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SelectionParser sp(pqlTokenScanner, declarationList);
    requireThrowAs<SemanticException>([&sp]() {
        sp.parse();
    });
}

TEST_CASE_METHOD(setUpSp, "Select attrRef procName, invalid synonym read") {
    query = "Select r.procName";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SelectionParser sp(pqlTokenScanner, declarationList);
    requireThrowAs<SemanticException>([&sp]() {
        sp.parse();
    });
}

TEST_CASE_METHOD(setUpSp, "Select attrRef procName, invalid synonym print") {
    query = "Select pn.procName";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SelectionParser sp(pqlTokenScanner, declarationList);
    requireThrowAs<SemanticException>([&sp]() {
        sp.parse();
    });
}

TEST_CASE_METHOD(setUpSp, "Select attrRef procName, invalid synonym constant") {
    query = "Select c.procName";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SelectionParser sp(pqlTokenScanner, declarationList);
    requireThrowAs<SemanticException>([&sp]() {
        sp.parse();
    });
}

TEST_CASE_METHOD(setUpSp, "Select attrRef procName, invalid synonym stmt") {
    query = "Select s.procName";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SelectionParser sp(pqlTokenScanner, declarationList);
    requireThrowAs<SemanticException>([&sp]() {
        sp.parse();
    });
}

TEST_CASE_METHOD(setUpSp, "Select attrRef procName, invalid synonym whike") {
    query = "Select w.procName";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SelectionParser sp(pqlTokenScanner, declarationList);
    requireThrowAs<SemanticException>([&sp]() {
        sp.parse();
    });
}

TEST_CASE_METHOD(setUpSp, "Select attrRef procName, invalid synonym if") {
    query = "Select ifs.procName";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SelectionParser sp(pqlTokenScanner, declarationList);
    requireThrowAs<SemanticException>([&sp]() {
        sp.parse();
    });
}

TEST_CASE_METHOD(setUpSp, "Select attrRef procName, invalid synonym assign") {
    query = "Select a.procName";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SelectionParser sp(pqlTokenScanner, declarationList);
    requireThrowAs<SemanticException>([&sp]() {
        sp.parse();
    });
}

TEST_CASE_METHOD(setUpSp, "Select attrRef value, invalid synonym procedure") {
    query = "Select p.value";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SelectionParser sp(pqlTokenScanner, declarationList);
    requireThrowAs<SemanticException>([&sp]() {
        sp.parse();
    });
}

TEST_CASE_METHOD(setUpSp, "Select attrRef value, invalid synonym call") {
    query = "Select cl.value";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SelectionParser sp(pqlTokenScanner, declarationList);
    requireThrowAs<SemanticException>([&sp]() {
        sp.parse();
    });
}

TEST_CASE_METHOD(setUpSp, "Select attrRef value, invalid synonym variable") {
    query = "Select v.value";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SelectionParser sp(pqlTokenScanner, declarationList);
    requireThrowAs<SemanticException>([&sp]() {
        sp.parse();
    });
}

TEST_CASE_METHOD(setUpSp, "Select attrRef value, invalid synonym read") {
    query = "Select r.value";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SelectionParser sp(pqlTokenScanner, declarationList);
    requireThrowAs<SemanticException>([&sp]() {
        sp.parse();
    });
}

TEST_CASE_METHOD(setUpSp, "Select attrRef value, invalid synonym print") {
    query = "Select pn.value";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SelectionParser sp(pqlTokenScanner, declarationList);
    requireThrowAs<SemanticException>([&sp]() {
        sp.parse();
    });
}

TEST_CASE_METHOD(setUpSp, "Select attrRef value, invalid synonym stmt") {
    query = "Select s.value";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SelectionParser sp(pqlTokenScanner, declarationList);
    requireThrowAs<SemanticException>([&sp]() {
        sp.parse();
    });
}

TEST_CASE_METHOD(setUpSp, "Select attrRef value, invalid synonym while") {
    query = "Select w.value";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SelectionParser sp(pqlTokenScanner, declarationList);
    requireThrowAs<SemanticException>([&sp]() {
        sp.parse();
    });
}

TEST_CASE_METHOD(setUpSp, "Select attrRef value, invalid synonym if") {
    query = "Select ifs.value";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SelectionParser sp(pqlTokenScanner, declarationList);
    requireThrowAs<SemanticException>([&sp]() {
        sp.parse();
    });
}

TEST_CASE_METHOD(setUpSp, "Select attrRef value, invalid synonym assign") {
    query = "Select a.value";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SelectionParser sp(pqlTokenScanner, declarationList);
    requireThrowAs<SemanticException>([&sp]() {
        sp.parse();
    });
}

TEST_CASE_METHOD(setUpSp, "Select attrRef varName, invalid synonym procedure") {
    query = "Select p.varName";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SelectionParser sp(pqlTokenScanner, declarationList);
    requireThrowAs<SemanticException>([&sp]() {
        sp.parse();
    });
}

TEST_CASE_METHOD(setUpSp, "Select attrRef varName, invalid synonym call") {
    query = "Select cl.varName";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SelectionParser sp(pqlTokenScanner, declarationList);
    requireThrowAs<SemanticException>([&sp]() {
        sp.parse();
    });
}

TEST_CASE_METHOD(setUpSp, "Select attrRef varName, invalid synonym constant") {
    query = "Select c.varName";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SelectionParser sp(pqlTokenScanner, declarationList);
    requireThrowAs<SemanticException>([&sp]() {
        sp.parse();
    });
}

TEST_CASE_METHOD(setUpSp, "Select attrRef varName, invalid synonym stmt") {
    query = "Select s.varName";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SelectionParser sp(pqlTokenScanner, declarationList);
    requireThrowAs<SemanticException>([&sp]() {
        sp.parse();
    });
}

TEST_CASE_METHOD(setUpSp, "Select attrRef varName, invalid synonym while") {
    query = "Select while.varName";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SelectionParser sp(pqlTokenScanner, declarationList);
    requireThrowAs<SemanticException>([&sp]() {
        sp.parse();
    });
}

TEST_CASE_METHOD(setUpSp, "Select attrRef varName, invalid synonym if") {
    query = "Select ifs.varName";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SelectionParser sp(pqlTokenScanner, declarationList);
    requireThrowAs<SemanticException>([&sp]() {
        sp.parse();
    });
}

TEST_CASE_METHOD(setUpSp, "Select attrRef varName, invalid synonym assign") {
    query = "Select a.varName";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SelectionParser sp(pqlTokenScanner, declarationList);
    requireThrowAs<SemanticException>([&sp]() {
        sp.parse();
    });
}
