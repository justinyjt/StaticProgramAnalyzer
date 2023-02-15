#include "catch.hpp"
#include "sp/DesignExtractor.h"
#include "commons/ASTNode.h"

TEST_CASE("DE can extract assign pattern correctly", "[Design Extractor]") {
    SECTION("DE can extract correct pattern: x = 3") {
        std::unique_ptr<ASTNode> root =
                std::make_unique<ASTNode>(ASTNode(ASTNode::SyntaxType::program ,std::nullopt));
        std::unique_ptr<ASTNode> proc =
                std::make_unique<ASTNode>(ASTNode(ASTNode::SyntaxType::procedure ,std::nullopt));
        std::unique_ptr<ASTNode> stmtLst =
                std::make_unique<ASTNode>(ASTNode(ASTNode::SyntaxType::stmtLst ,std::nullopt));
        std::unique_ptr<ASTNode> assign =
                std::make_unique<ASTNode>(ASTNode(ASTNode::SyntaxType::assign ,std::nullopt));
        std::unique_ptr<ASTNode> lVar =
                std::make_unique<ASTNode>(ASTNode(ASTNode::SyntaxType::var ,"x"));
        std::unique_ptr<ASTNode> rConst =
                std::make_unique<ASTNode>(ASTNode(ASTNode::SyntaxType::constVal ,"3"));

        assign->addChild(std::move(lVar));
        assign->addChild(std::move(rConst));
        stmtLst->addChild(std::move(assign));
        proc->addChild(std::move(stmtLst));
        root->addChild(std::move(proc));

        PKB pkb;
        DesignExtractor de = DesignExtractor(std::make_unique<PKBWriter>(pkb));
        de.extractProgram(std::move(root));

        std::string ans;
        for (const auto& itr : de.getAssignPatMap()) {
            ans.append(std::to_string(itr.first) + "\t" + itr.second + "\n");
        }

        REQUIRE(ans == "1\t3\n");
    }

    SECTION("DE can extract correct pattern: x =  y * 3") {
        std::unique_ptr<ASTNode> root =
                std::make_unique<ASTNode>(ASTNode(ASTNode::SyntaxType::program ,std::nullopt));
        std::unique_ptr<ASTNode> proc =
                std::make_unique<ASTNode>(ASTNode(ASTNode::SyntaxType::procedure ,std::nullopt));
        std::unique_ptr<ASTNode> stmtLst =
                std::make_unique<ASTNode>(ASTNode(ASTNode::SyntaxType::stmtLst ,std::nullopt));
        std::unique_ptr<ASTNode> assign =
                std::make_unique<ASTNode>(ASTNode(ASTNode::SyntaxType::assign ,std::nullopt));
        std::unique_ptr<ASTNode> lVar =
                std::make_unique<ASTNode>(ASTNode(ASTNode::SyntaxType::var ,"x"));
        std::unique_ptr<ASTNode> rVar =
                std::make_unique<ASTNode>(ASTNode(ASTNode::SyntaxType::var ,"y"));
        std::unique_ptr<ASTNode> mul =
                std::make_unique<ASTNode>(ASTNode(ASTNode::SyntaxType::times ,"*"));
        std::unique_ptr<ASTNode> rConst =
                std::make_unique<ASTNode>(ASTNode(ASTNode::SyntaxType::constVal ,"3"));

        mul->addChild(std::move(rVar));
        mul->addChild(std::move(rConst));
        assign->addChild(std::move(lVar));
        assign->addChild(std::move(mul));
        stmtLst->addChild(std::move(assign));
        proc->addChild(std::move(stmtLst));
        root->addChild(std::move(proc));

        PKB pkb;
        DesignExtractor de = DesignExtractor(std::make_unique<PKBWriter>(pkb));
        de.extractProgram(std::move(root));

        std::string ans;
        for (auto itr : de.getAssignPatMap()) {
            ans.append(std::to_string(itr.first) + "\t" + itr.second + "\n");
        }

        REQUIRE(ans == "1\ty*3\n");
    }

    SECTION("DE can extract correct pattern: x =  1 + y * 3") {
        std::unique_ptr<ASTNode> root =
                std::make_unique<ASTNode>(ASTNode(ASTNode::SyntaxType::program ,std::nullopt));
        std::unique_ptr<ASTNode> proc =
                std::make_unique<ASTNode>(ASTNode(ASTNode::SyntaxType::procedure ,std::nullopt));
        std::unique_ptr<ASTNode> stmtLst =
                std::make_unique<ASTNode>(ASTNode(ASTNode::SyntaxType::stmtLst ,std::nullopt));
        std::unique_ptr<ASTNode> assign =
                std::make_unique<ASTNode>(ASTNode(ASTNode::SyntaxType::assign ,std::nullopt));
        std::unique_ptr<ASTNode> lVar =
                std::make_unique<ASTNode>(ASTNode(ASTNode::SyntaxType::var ,"x"));
        std::unique_ptr<ASTNode> rConst1 =
                std::make_unique<ASTNode>(ASTNode(ASTNode::SyntaxType::constVal ,"1"));
        std::unique_ptr<ASTNode> plus =
                std::make_unique<ASTNode>(ASTNode(ASTNode::SyntaxType::plus ,"+"));
        std::unique_ptr<ASTNode> rVar2 =
                std::make_unique<ASTNode>(ASTNode(ASTNode::SyntaxType::var ,"y"));
        std::unique_ptr<ASTNode> mul =
                std::make_unique<ASTNode>(ASTNode(ASTNode::SyntaxType::times ,"*"));
        std::unique_ptr<ASTNode> rConst3 =
                std::make_unique<ASTNode>(ASTNode(ASTNode::SyntaxType::constVal ,"3"));

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
        for (auto itr : de.getAssignPatMap()) {
            ans.append(std::to_string(itr.first) + "\t" + itr.second + "\n");
        }
        REQUIRE(ans == "1\t1+y*3\n");
    }
}
