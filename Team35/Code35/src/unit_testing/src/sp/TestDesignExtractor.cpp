#include "catch.hpp"
#include "sp/DesignExtractor.h"
#include "commons/ASTNode.h"
#include "../TestHelper.h"

TEST_CASE("DE can extract assign pattern correctly", "[Design Extractor]") {
    SECTION("DE can extract correct pattern: x = 3") {
        std::shared_ptr<ASTNode> root =
                std::make_shared<ASTNode>(ASTNode::SyntaxType::Program, std::nullopt);
        std::shared_ptr<ASTNode> proc =
                std::make_shared<ASTNode>(ASTNode::SyntaxType::Procedure, "main");
        std::shared_ptr<ASTNode> stmtLst =
                std::make_shared<ASTNode>(ASTNode::SyntaxType::StmtLst, std::nullopt);
        std::shared_ptr<ExprNode> assign =
                std::make_shared<ExprNode>(ASTNode::SyntaxType::Assign, std::nullopt);
        std::shared_ptr<ExprNode> lVar =
                std::make_shared<ExprNode>(ASTNode::SyntaxType::Variable, "x");
        std::shared_ptr<ExprNode> rConst =
                std::make_shared<ExprNode>(ASTNode::SyntaxType::Constant, "3");

        assign->addChild(lVar);
        assign->addChild(rConst);
        stmtLst->addChild(assign);
        proc->addChild(stmtLst);
        root->addChild(proc);

        PKB pkb;
        DesignExtractor de = DesignExtractor(std::make_unique<PKBWriter>(pkb));
        de.extractProgram(std::move(root));

        std::string ans;
        for (const auto &itr : de.getAssignPatMap()) {
            ans.append(std::to_string(itr.first) + "\t" +
                       itr.second.first + "=" + itr.second.second->toString() + "\n");
        }
        std::string expected = "1\tx=3\n";

        requireEqual(ans, expected);
    }

    SECTION("DE can extract correct pattern: x =  y * 3") {
        std::shared_ptr<ASTNode> root =
                std::make_shared<ASTNode>(ASTNode::SyntaxType::Program, std::nullopt);
        std::shared_ptr<ASTNode> proc =
                std::make_shared<ASTNode>(ASTNode::SyntaxType::Procedure, "main");
        std::shared_ptr<ASTNode> stmtLst =
                std::make_shared<ASTNode>(ASTNode::SyntaxType::StmtLst, std::nullopt);
        std::shared_ptr<ExprNode> assign =
                std::make_shared<ExprNode>(ASTNode::SyntaxType::Assign, std::nullopt);
        std::shared_ptr<ExprNode> lVar =
                std::make_shared<ExprNode>(ASTNode::SyntaxType::Variable, "x");
        std::shared_ptr<ExprNode> rVar =
                std::make_shared<ExprNode>(ASTNode::SyntaxType::Variable, "y");
        std::shared_ptr<ExprNode> mul =
                std::make_shared<ExprNode>(ASTNode::SyntaxType::Multiply, "*");
        std::shared_ptr<ExprNode> rConst =
                std::make_shared<ExprNode>(ASTNode::SyntaxType::Constant, "3");

        mul->addChild(rVar);
        mul->addChild(rConst);
        assign->addChild(lVar);
        assign->addChild(mul);
        stmtLst->addChild(assign);
        proc->addChild(stmtLst);
        root->addChild(proc);

        PKB pkb;
        DesignExtractor de = DesignExtractor(std::make_unique<PKBWriter>(pkb));
        de.extractProgram(std::move(root));

        std::string ans;
        for (const auto &itr : de.getAssignPatMap()) {
            ans.append(std::to_string(itr.first) + "\t" +
                       itr.second.first + "=" + itr.second.second->toString() + "\n");
        }

        std::string expected = "1\tx=y*3\n";
        requireEqual(ans, expected);
    }

    SECTION("DE can extract correct pattern: x =  1 + y * 3") {
        std::shared_ptr<ASTNode> root =
                std::make_shared<ASTNode>(ASTNode::SyntaxType::Program, std::nullopt);
        std::shared_ptr<ASTNode> proc =
                std::make_shared<ASTNode>(ASTNode::SyntaxType::Procedure, "main");
        std::shared_ptr<ASTNode> stmtLst =
                std::make_shared<ASTNode>(ASTNode::SyntaxType::StmtLst, std::nullopt);
        std::shared_ptr<ExprNode> assign =
                std::make_shared<ExprNode>(ASTNode::SyntaxType::Assign, std::nullopt);
        std::shared_ptr<ExprNode> lVar =
                std::make_shared<ExprNode>(ASTNode::SyntaxType::Variable, "x");
        std::shared_ptr<ExprNode> rConst1 =
                std::make_shared<ExprNode>(ASTNode::SyntaxType::Constant, "1");
        std::shared_ptr<ExprNode> plus =
                std::make_shared<ExprNode>(ASTNode::SyntaxType::Plus, "+");
        std::shared_ptr<ExprNode> rVar2 =
                std::make_shared<ExprNode>(ASTNode::SyntaxType::Variable, "y");
        std::shared_ptr<ExprNode> mul =
                std::make_shared<ExprNode>(ASTNode::SyntaxType::Multiply, "*");
        std::shared_ptr<ExprNode> rConst3 =
                std::make_shared<ExprNode>(ASTNode::SyntaxType::Constant, "3");

        mul->addChild(std::move(rVar2));
        mul->addChild(std::move(rConst3));
        plus->addChild(std::move(rConst1));
        plus->addChild(std::move(mul));
        assign->addChild(std::move(lVar));
        assign->addChild(std::move(plus));
        stmtLst->addChild(std::move(assign));
        proc->addChild(std::move(stmtLst));
        root->addChild(std::move(proc));

        PKB pkb;
        DesignExtractor de = DesignExtractor(std::make_unique<PKBWriter>(pkb));
        de.extractProgram(std::move(root));

        std::string ans;
        for (const auto &itr : de.getAssignPatMap()) {
            ans.append(std::to_string(itr.first) + "\t" +
                       itr.second.first + "=" + itr.second.second->toString() + "\n");
        }

        std::string expected = "1\tx=1+y*3\n";
        requireEqual(ans, expected);
    }
}
