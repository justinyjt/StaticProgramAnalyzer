#include "catch.hpp"

#include "../TestHelper.h"
#include "pkb/PKB.h"
#include "pkb/PKBWriter.h"
#include "pkb/PKBReader.h"
#include "sp/SourceProcessor.h"
#include "qps/QPS.h"


TEST_CASE("Integration test for wiki sample code 4") {
    PKB pkb = PKB();
    SourceProcessor sp = SourceProcessor(std::make_unique<PKBWriter>(pkb));
    PKBReader reader = PKBReader(pkb);
    QPS qps = QPS(&reader);
    std::string sourceCode =

            "procedure main { flag = 0; call computeCentroid; call printResults;}"
            "procedure readPoint { read x; read y;"
            "    }"
            "    procedure printResults {"
            " print flag;"
            "print cenX;"
            " print cenY;"
            "print normSq;"
            "    }"
            "    procedure computeCentroid {"
            "count = 0;"
            "cenX = 0;"
            "cenY = 0;"
            " call readPoint;"
            "while ((x != 0) && (y != 0)) {"
            "count = count + 1;"
            "cenX = cenX + x;"
            "cenY = cenY + y;"
            "call readPoint;"
            "        }"
            "if (count == 0) then {"
            "flag = 1;"
            "        } else {"
            "cenX = cenX / count;"
            "cenY = cenY / count;"
            "        }"
            "normSq = cenX * cenX + cenY * cenY;"
            "    }";
    requireTrue(sp.process(sourceCode));

    SECTION("Follows") {
        SECTION("Follows select stmt") {
            SECTION("stmt s; Select s such that Follows(1, s)") {
                std::list<std::string> answer;
                std::list<std::string> expected({"2"});
                std::string query = "stmt s; Select s such that Follows(1, s)";
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("stmt s; Select s such that Follows(19, s)") {
                std::list<std::string> answer;
                std::list<std::string> expected({"23"});
                std::string query = "stmt s; Select s such that Follows(19, s)";
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("stmt s; Select s such that Follows(s, 1)") {
                std::list<std::string> answer;
                std::list<std::string> expected;
                std::string query = "stmt s; Select s such that Follows(s, 1)";
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("stmt s; Select s such that Follows(s, 22)") {
                std::list<std::string> answer;
                std::list<std::string> expected({"21"});
                std::string query = "stmt s; Select s such that Follows(s, 22)";
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("stmt s; Select s such that Follows(s, 23)") {
                std::list<std::string> answer;
                std::list<std::string> expected({"19"});
                std::string query = "stmt s; Select s such that Follows(s, 23)";
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }
        }

        SECTION("Follows select read") {
            SECTION("read r; Select r such that Follows(4, r)") {
                std::list<std::string> answer;
                std::string query = "read r; Select r such that Follows(4, r)";
                std::list<std::string> expected({"5"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("read r; Select r such that Follows(r, 5)") {
                std::list<std::string> answer;
                std::string query = "read r; Select r such that Follows(r, 5)";
                std::list<std::string> expected({"4"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }
        }

        SECTION("Follows select print") {
            SECTION("print p; Select p such that Follows(p, 7)") {
                std::list<std::string> answer;
                std::string query = "print p; Select p such that Follows(p, 7)";
                std::list<std::string> expected({"6"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("print p; Select p such that Follows(6, p)") {
                std::list<std::string> answer;
                std::string query = "print p; Select p such that Follows(6, p)";
                std::list<std::string> expected({"7"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("print p; Select p such that Follows(p, 7)") {
                std::list<std::string> answer;
                std::string query = "print p; Select p such that Follows(p, 7)";
                std::list<std::string> expected({"6"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("print p; Select p such that Follows(7, p)") {
                std::list<std::string> answer;
                std::string query = "print p; Select p such that Follows(7, p)";
                std::list<std::string> expected({"8"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("print p; Select p such that Follows(p, 8)") {
                std::list<std::string> answer;
                std::string query = "print p; Select p such that Follows(p, 8)";
                std::list<std::string> expected({"7"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("print p; Select p such that Follows(8, p)") {
                std::list<std::string> answer;
                std::string query = "print p; Select p such that Follows(8, p)";
                std::list<std::string> expected({"9"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("print p; Select p such that Follows(p, 9)") {
                std::list<std::string> answer;
                std::string query = "print p; Select p such that Follows(p, 9)";
                std::list<std::string> expected({"8"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("print p; Select p such that Follows(9, p)") {
                std::list<std::string> answer;
                std::string query = "print p; Select p such that Follows(9, p)";
                std::list<std::string> expected;
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }
        }

        SECTION("Follows select while") {
            SECTION("while w; Select w such that Follows(13, w)") {
                std::list<std::string> answer;
                std::string query = "while w; Select w such that Follows(13, w)";
                std::list<std::string> expected({"14"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("while w; Select w such that Follows(14, w)") {
                std::list<std::string> answer;
                std::string query = "while w; Select w such that Follows(14, w)";
                std::list<std::string> expected;
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }
        }

        SECTION("Follows select if") {
            SECTION("if ifs; Select ifs such that Follows(14, ifs)") {
                std::list<std::string> answer;
                std::string query = "if ifs; Select ifs such that Follows(14, ifs)";
                std::list<std::string> expected({"19"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("if ifs; Select ifs such that Follows(19, ifs)") {
                std::list<std::string> answer;
                std::string query = "if ifs; Select ifs such that Follows(19, ifs)";
                std::list<std::string> expected;
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }
        }

        SECTION("Follows(assign)") {
            SECTION("assign a; Select a such that Follows(a, 11)") {
                std::list<std::string> answer;
                std::string query = "assign a; Select a such that Follows(a, 11)";
                std::list<std::string> expected({"10"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("assign a; Select a such that Follows(a, 12)") {
                std::list<std::string> answer;
                std::string query = "assign a; Select a such that Follows(a, 12)";
                std::list<std::string> expected({"11"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("assign a; Select a such that Follows(a, 21)") {
                std::list<std::string> answer;
                std::string query = "assign a; Select a such that Follows(a, 21)";
                std::list<std::string> expected;
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("assign a; Select a such that Follows(19, a)") {
                std::list<std::string> answer;
                std::string query = "assign a; Select a such that Follows(19, a)";
                std::list<std::string> expected({"23"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("assign a; Select a such that Follows(20, a)") {
                std::list<std::string> answer;
                std::string query = "assign a; Select a such that Follows(20, a)";
                std::list<std::string> expected;
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("assign a; Select a such that Follows(a, 2)") {
                std::list<std::string> answer;
                std::string query = "assign a; Select a such that Follows(a, 2)";
                std::list<std::string> expected({"1"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("assign a; Select a such that Follows(a, 3)") {
                std::list<std::string> answer;
                std::string query = "assign a; Select a such that Follows(a, 3)";
                std::list<std::string> expected;
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }
        }

        SECTION("Follows select call") {
            SECTION("call c, c1; Select c such that Follows(c, c1)") {
                std::list<std::string> answer;
                std::string query = "call c, c1; Select c such that Follows(c, c1)";
                std::list<std::string> expected({"2"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("call c, c1; Select c1 such that Follows(c, c1)") {
                std::list<std::string> answer;
                std::string query = "call c, c1; Select c1 such that Follows(c, c1)";
                std::list<std::string> expected({"3"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("call c; Select c such that Follows(17, c)") {
                std::list<std::string> answer;
                std::string query = "call c; Select c such that Follows(17, c)";
                std::list<std::string> expected({"18"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("call c; Select c such that Follows(c, 2)") {
                std::list<std::string> answer;
                std::string query = "call c; Select c such that Follows(c, 2)";
                std::list<std::string> expected;
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("call c; Select c such that Follows(c, 3)") {
                std::list<std::string> answer;
                std::string query = "call c; Select c such that Follows(c, 3)";
                std::list<std::string> expected({"2"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }
        }

        SECTION("Follows mixed") {
            SECTION("assign a; call cl; Select a such that Follows(a, cl) with cl.stmt# = 2") {
                std::list<std::string> answer;
                std::string query = "assign a; call cl; Select a such that Follows(a, cl) with cl.stmt# = 2";
                std::list<std::string> expected({"1"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("assign a; call cl; Select cl.procName such that Follows(a, cl) with cl.stmt# = 2") {
                std::list<std::string> answer;
                std::string query = "assign a; call cl; Select cl.procName such that Follows(a, cl) with cl.stmt# = 2";
                std::list<std::string> expected({"computeCentroid"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("assign a; call cl; Select cl.procName such that Follows(a, cl) with a.stmt# = 12") {
                std::list<std::string> answer;
                std::string query = "assign a; call cl; Select cl.procName such that Follows(a, cl) with a.stmt# = 12";
                std::list<std::string> expected({"readPoint"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("call cl; while w; Select w such that Follows(cl, w) with cl.procName = \"readPoint\"") {
                std::list<std::string> answer;
                std::string query =
                        "call cl; while w; Select w such that Follows(cl, w) with cl.procName = \"readPoint\"";
                std::list<std::string> expected({"14"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("Follows(mix): Test 5") {
                std::list<std::string> answer;
                std::string query =
                        "if ifs; while w; Select ifs such that Follows(w, ifs) with w.stmt# = 14";
                std::list<std::string> expected({"19"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }
        }

        SECTION("Follows with wildcard") {
            SECTION("stmt s; Select s such that Follows(s, _)") {
                std::list<std::string> answer;
                std::string query = "stmt s; Select s such that Follows(s, _)";
                std::list<std::string> expected({"1", "10", "11", "12", "13", "14", "15", "16", "17",
                                                 "19", "2", "21", "4", "6", "7", "8"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("print p; Select p such that Follows(p, _)") {
                std::list<std::string> answer;
                std::string query = "print p; Select p such that Follows(p, _)";
                std::list<std::string> expected({"6", "7", "8"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("call cl; Select cl such that Follows(cl, _)") {
                std::list<std::string> answer;
                std::string query =
                        "call cl; Select cl such that Follows(cl, _)";
                std::list<std::string> expected({"13", "2"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("read r; Select r such that Follows(r, _)") {
                std::list<std::string> answer;
                std::string query = "read r; Select r such that Follows(r, _)";
                std::list<std::string> expected({"4"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("if ifs; Select ifs such that Follows(ifs, _)") {
                std::list<std::string> answer;
                std::string query =
                        "if ifs; Select ifs such that Follows(ifs, _)";
                std::list<std::string> expected({"19"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("while w; Select w such that Follows(w, _)") {
                std::list<std::string> answer;
                std::string query = "while w; Select w such that Follows(w, _)";
                std::list<std::string> expected({"14"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("assign a; Select a such that Follows(a, _)") {
                std::list<std::string> answer;
                std::string query =
                        "assign a; Select a such that Follows(a, _)";
                std::list<std::string> expected({"1", "10", "11", "12",
                                                 "15", "17", "16", "21"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("stmt s; Select s such that Follows(18, _)") {
                std::list<std::string> answer;
                std::string query = "stmt s; Select s such that Follows(18, _)";
                std::list<std::string> expected;
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("stmt s; Select s such that Follows(17, _)") {
                std::list<std::string> answer;
                std::string query = "stmt s; Select s such that Follows(17, _)";
                std::list<std::string> expected({"1", "10", "11", "12", "13", "14", "15", "16",
                                                 "17", "18", "19", "2", "20", "21", "22", "23",
                                                 "3", "4", "5", "6", "7", "8", "9"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }
        }
    }

    SECTION("Follows*") {
        SECTION("Follows* select stmt") {
            SECTION("stmt s; Select s such that Follows*(1, s)") {
                std::list<std::string> answer;
                std::string query = "stmt s; Select s such that Follows*(1, s)";
                std::list<std::string> expected({"2", "3"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("stmt s; Select s such that Follows*(19, s)") {
                std::list<std::string> answer;
                std::string query = "stmt s; Select s such that Follows*(19, s)";
                std::list<std::string> expected({"23"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("stmt s; Select s such that Follows*(s, 1)") {
                std::list<std::string> answer;
                std::string query = "stmt s; Select s such that Follows*(s, 1)";
                std::list<std::string> expected;
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("stmt s; Select s such that Follows*(s, 22)") {
                std::list<std::string> answer;
                std::string query = "stmt s; Select s such that Follows*(s, 22)";
                std::list<std::string> expected({"21"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("stmt s; Select s such that Follows*(s, 23)") {
                std::list<std::string> answer;
                std::string query = "stmt s; Select s such that Follows*(s, 23)";
                std::list<std::string> expected({"10", "11", "12",
                                                 "13", "14", "19"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }
        }

        SECTION("Follows* select read") {
            SECTION("read r; Select r such that Follows*(4, r)") {
                std::list<std::string> answer;
                std::string query = "read r; Select r such that Follows*(4, r)";
                std::list<std::string> expected({"5"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("read r; Select r such that Follows*(r, 5)") {
                std::list<std::string> answer;
                std::string query = "read r; Select r such that Follows*(r, 5)";
                std::list<std::string> expected({"4"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }
        }

        SECTION("Follows* select print)") {
            SECTION("print p; Select p such that Follows*(p, 7)") {
                std::list<std::string> answer;
                std::string query = "print p; Select p such that Follows*(p, 7)";
                std::list<std::string> expected({"6"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("print p; Select p such that Follows*(6, p)") {
                std::list<std::string> answer;
                std::string query = "print p; Select p such that Follows*(6, p)";
                std::list<std::string> expected({"7", "8", "9"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("print p; Select p such that Follows*(p, 7)") {
                std::list<std::string> answer;
                std::string query = "print p; Select p such that Follows*(p, 7)";
                std::list<std::string> expected({"6"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("print p; Select p such that Follows*(7, p)") {
                std::list<std::string> answer;
                std::string query = "print p; Select p such that Follows*(7, p)";
                std::list<std::string> expected({"8", "9"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("print p; Select p such that Follows*(p, 8)") {
                std::list<std::string> answer;
                std::string query = "print p; Select p such that Follows*(p, 8)";
                std::list<std::string> expected({"6", "7"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("print p; Select p such that Follows*(8, p") {
                std::list<std::string> answer;
                std::string query = "print p; Select p such that Follows*(8, p)";
                std::list<std::string> expected({"9"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("print p; Select p such that Follows*(p, 9)") {
                std::list<std::string> answer;
                std::string query = "print p; Select p such that Follows*(p, 9)";
                std::list<std::string> expected({"6", "7", "8"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("print p; Select p such that Follows*(9, p)") {
                std::list<std::string> answer;
                std::string query = "print p; Select p such that Follows*(9, p)";
                std::list<std::string> expected;
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }
        }

        SECTION("Follows* select while") {
            SECTION("while w; Select w such that Follows*(13, w)") {
                std::list<std::string> answer;
                std::string query = "while w; Select w such that Follows*(13, w)";
                std::list<std::string> expected({"14"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("while w; Select w such that Follows*(14, w)") {
                std::list<std::string> answer;
                std::string query = "while w; Select w such that Follows*(14, w)";
                std::list<std::string> expected;
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }
        }

        SECTION("Follows* select if") {
            SECTION("if ifs; Select ifs such that Follows*(14, ifs)") {
                std::list<std::string> answer;
                std::string query = "if ifs; Select ifs such that Follows*(14, ifs)";
                std::list<std::string> expected({"19"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("if ifs; Select ifs such that Follows*(19, ifs)") {
                std::list<std::string> answer;
                std::string query = "if ifs; Select ifs such that Follows*(19, ifs)";
                std::list<std::string> expected;
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }
        }

        SECTION("Follows* select assign") {
            SECTION("assign a; Select a such that Follows*(a, 11)") {
                std::list<std::string> answer;
                std::string query = "assign a; Select a such that Follows*(a, 11)";
                std::list<std::string> expected({"10"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("assign a; Select a such that Follows*(a, 12)") {
                std::list<std::string> answer;
                std::string query = "assign a; Select a such that Follows*(a, 12)";
                std::list<std::string> expected({"10", "11"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("assign a; Select a such that Follows*(a, 21)") {
                std::list<std::string> answer;
                std::string query = "assign a; Select a such that Follows*(a, 21)";
                std::list<std::string> expected;
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("assign a; Select a such that Follows*(19, a)") {
                std::list<std::string> answer;
                std::string query = "assign a; Select a such that Follows*(19, a)";
                std::list<std::string> expected({"23"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("assign a; Select a such that Follows*(20, a)") {
                std::list<std::string> answer;
                std::string query = "assign a; Select a such that Follows*(20, a)";
                std::list<std::string> expected;
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("assign a; Select a such that Follows*(a, 2)") {
                std::list<std::string> answer;
                std::string query = "assign a; Select a such that Follows*(a, 2)";
                std::list<std::string> expected({"1"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("assign a; Select a such that Follows*(a, 3)") {
                std::list<std::string> answer;
                std::string query = "assign a; Select a such that Follows*(a, 3)";
                std::list<std::string> expected({"1"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }
        }

        SECTION("Follows* select call") {
            SECTION("call c, c1; Select c such that Follows*(c, c1)") {
                std::list<std::string> answer;
                std::string query = "call c, c1; Select c such that Follows*(c, c1)";
                std::list<std::string> expected({"2"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("call c, c1; Select c1 such that Follows*(c, c1)") {
                std::list<std::string> answer;
                std::string query = "call c, c1; Select c1 such that Follows*(c, c1)";
                std::list<std::string> expected({"3"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("call c; Select c such that Follows*(17, c)") {
                std::list<std::string> answer;
                std::string query = "call c; Select c such that Follows*(17, c)";
                std::list<std::string> expected({"18"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("Follows*(calls): Test 4") {
                std::list<std::string> answer;
                std::string query = "call c; Select c such that Follows*(c, 2)";
                std::list<std::string> expected;
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("Follows*(calls): Test 5") {
                std::list<std::string> answer;
                std::string query = "call c; Select c such that Follows*(c, 3)";
                std::list<std::string> expected({"2"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }
        }

        SECTION("Follows* mixed") {
            SECTION("assign a; call cl; Select a such that Follows*(a, cl) with cl.stmt# = 2") {
                std::list<std::string> answer;
                std::string query = "assign a; call cl; Select a such that Follows*(a, cl) with cl.stmt# = 2";
                std::list<std::string> expected({"1"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("assign a; call cl; Select cl.procName such that Follows*(a, cl) with cl.stmt# = 2") {
                std::list<std::string> answer;
                std::string query = "assign a; call cl; Select cl.procName such that Follows*(a, cl) with cl.stmt# = 2";
                std::list<std::string> expected({"computeCentroid"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("assign a; call cl; Select cl.procName such that Follows*(a, cl) with a.stmt# = 12") {
                std::list<std::string> answer;
                std::string query = "assign a; call cl; Select cl.procName such that Follows*(a, cl) with a.stmt# = 12";
                std::list<std::string> expected({"readPoint"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("call cl; while w; Select w such that Follows*(cl, w) with cl.procName = \"readPoint\"") {
                std::list<std::string> answer;
                std::string query = "call cl; while w; Select w such that Follows*(cl, w)"
                                    "with cl.procName = \"readPoint\"";
                std::list<std::string> expected({"14"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("if ifs; while w; Select ifs such that Follows*(w, ifs) with w.stmt# = 14") {
                std::list<std::string> answer;
                std::string query = "if ifs; while w; Select ifs such that Follows*(w, ifs) with w.stmt# = 14";
                std::list<std::string> expected({"19"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }
        }

        SECTION("Follows* with wildcard") {
            SECTION("stmt s; Select s such that Follows*(s, _)") {
                std::list<std::string> answer;
                std::string query = "stmt s; Select s such that Follows*(s, _)";
                std::list<std::string> expected({"1", "10", "11", "12", "13", "14", "15", "17",
                                                 "19", "2", "16", "21", "4", "6", "7", "8"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("print p; Select p such that Follows*(p, _)") {
                std::list<std::string> answer;
                std::string query = "print p; Select p such that Follows*(p, _)";
                std::list<std::string> expected({"6", "7", "8"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("call cl; Select cl such that Follows*(cl, _)") {
                std::list<std::string> answer;
                std::string query = "call cl; Select cl such that Follows*(cl, _)";
                std::list<std::string> expected({"13", "2"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("read r; Select r such that Follows*(r, _)") {
                std::list<std::string> answer;
                std::string query = "read r; Select r such that Follows*(r, _)";
                std::list<std::string> expected({"4"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("if ifs; Select ifs such that Follows*(ifs, _)") {
                std::list<std::string> answer;
                std::string query = "if ifs; Select ifs such that Follows*(ifs, _)";
                std::list<std::string> expected({"19"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("while w; Select w such that Follows*(w, _)") {
                std::list<std::string> answer;
                std::string query = "while w; Select w such that Follows*(w, _)";
                std::list<std::string> expected({"14"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("assign a; Select a such that Follows*(a, _)") {
                std::list<std::string> answer;
                std::string query = "assign a; Select a such that Follows*(a, _)";
                std::list<std::string> expected({"1", "10", "11", "12",
                                                 "15", "17", "16", "21"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("stmt s; Select s such that Follows*(18, _)") {
                std::list<std::string> answer;
                std::string query = "stmt s; Select s such that Follows*(18, _)";
                std::list<std::string> expected;
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("stmt s; Select s such that Follows*(17, _)") {
                std::list<std::string> answer;
                std::string query = "stmt s; Select s such that Follows*(17, _)";
                std::list<std::string> expected({"1", "10", "11", "12", "13", "14", "15", "16",
                                                 "17", "18", "19", "2", "20", "21", "22", "23",
                                                 "3", "4", "5", "6", "7", "8", "9"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }
        }
    }

    SECTION("Select Attribute Values") {
        SECTION("Select procedure.procName") {
            std::list<std::string> answer;
            std::string query = "procedure p; Select p.procName";
            std::list<std::string> expected({"main", "printResults", "readPoint", "computeCentroid"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("Select call.procName") {
            std::list<std::string> answer;
            std::string query = "call cl; Select cl.procName";
            // might need to remove duplicate
            std::list<std::string> expected({"printResults", "readPoint", "computeCentroid"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("Select variable.varName") {
            std::list<std::string> answer;
            std::string query = "variable v; Select v.varName";
            std::list<std::string> expected({"cenX", "cenY", "count", "flag", "normSq", "x", "y"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("Select read.varName") {
            std::list<std::string> answer;
            std::string query = "read r; Select r.varName";
            std::list<std::string> expected({"x", "y"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("Select print.varName") {
            std::list<std::string> answer;
            std::string query = "print p; Select p.varName";
            std::list<std::string> expected({"cenX", "cenY", "flag", "normSq"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("Select constant.value") {
            std::list<std::string> answer;
            std::string query = "constant c; Select c.value";
            std::list<std::string> expected({"0", "1"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("Select stmt.stmt#") {
            std::list<std::string> answer;
            std::string query = "stmt s; Select s.stmt#";
            std::list<std::string> expected({"1", "2", "3", "4", "5", "6", "7", "8",
                                             "9", "10", "11", "12", "13", "14", "15", "16",
                                             "17", "18", "19", "20", "21", "22", "23"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("Select read.stmt#") {
            std::list<std::string> answer;
            std::string query = "read r; Select r.stmt#";
            std::list<std::string> expected({"4", "5"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("Select print.stmt#") {
            std::list<std::string> answer;
            std::string query = "print p; Select p.stmt#";
            std::list<std::string> expected({"6", "7", "8", "9"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("Select call.stmt#") {
            std::list<std::string> answer;
            std::string query = "call c; Select c.stmt#";
            std::list<std::string> expected({"13", "18", "2", "3"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("Select while.stmt#") {
            std::list<std::string> answer;
            std::string query = "while w; Select w.stmt#";
            std::list<std::string> expected({"14"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("Select if.stmt#") {
            std::list<std::string> answer;
            std::string query = "if ifs; Select ifs.stmt#";
            std::list<std::string> expected({"19"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("Select assign.stmt#") {
            std::list<std::string> answer;
            std::string query = "assign a; Select a.stmt#";
            std::list<std::string> expected({"1", "10", "11", "12",
                                             "15", "16", "17", "20",
                                             "21", "22", "23"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }
    }

    SECTION("Parent") {
        SECTION("Parent select stmt") {
            SECTION("stmt s; Select s such that Parent(1, s)") {
                std::list<std::string> answer;
                std::string query = "stmt s; Select s such that Parent(1, s)";
                std::list<std::string> expected;
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("stmt s; Select s such that Parent(19, s)") {
                std::list<std::string> answer;
                std::string query = "stmt s; Select s such that Parent(19, s)";
                std::list<std::string> expected({"20", "21", "22"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("stmt s; Select s such that Parent(s, 1)") {
                std::list<std::string> answer;
                std::string query = "stmt s; Select s such that Parent(s, 1)";
                std::list<std::string> expected;
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("stmt s; Select s such that Parent(s, 22)") {
                std::list<std::string> answer;
                std::string query = "stmt s; Select s such that Parent(s, 22)";
                std::list<std::string> expected({"19"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("stmt s; Select s such that Parent(s, 23)") {
                std::list<std::string> answer;
                std::string query = "stmt s; Select s such that Parent(s, 23)";
                std::list<std::string> expected;
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }
        }

        SECTION("Parent select read") {
            SECTION("read r; Select r such that Parent(4, r)") {
                std::list<std::string> answer;
                std::string query = "read r; Select r such that Parent(4, r)";
                std::list<std::string> expected;
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("read r; Select r such that Parent(r, 5)") {
                std::list<std::string> answer;
                std::string query = "read r; Select r such that Parent(r, 5)";
                std::list<std::string> expected;
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }
        }

        SECTION("Parent select print)") {
            SECTION("print p; Select p such that Parent(p, 7)") {
                std::list<std::string> answer;
                std::string query = "print p; Select p such that Parent(p, 7)";
                std::list<std::string> expected;
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("print p; Select p such that Parent(8, p)") {
                std::list<std::string> answer;
                std::string query = "print p; Select p such that Parent(8, p)";
                std::list<std::string> expected;
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }
        }

        SECTION("Parent select while") {
            SECTION("while w; Select w such that Parent(w, 15)") {
                std::list<std::string> answer;
                std::string query = "while w; Select w such that Parent(w, 15)";
                std::list<std::string> expected({"14"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("while w; Select w such that Parent(14, w)") {
                std::list<std::string> answer;
                std::string query = "while w; Select w such that Parent(14, w)";
                std::list<std::string> expected;
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("while w; Select w such that Parent(w, 16)") {
                std::list<std::string> answer;
                std::string query = "while w; Select w such that Parent(w, 16)";
                std::list<std::string> expected({"14"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("while w; Select w such that Parent(w, 17)") {
                std::list<std::string> answer;
                std::string query = "while w; Select w such that Parent(w, 17)";
                std::list<std::string> expected({"14"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("while w; Select w such that Parent(w, 18)") {
                std::list<std::string> answer;
                std::string query = "while w; Select w such that Parent(w, 18)";
                std::list<std::string> expected({"14"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }
        }

        SECTION("Parent select if") {
            SECTION("if ifs; Select ifs such that Parent(14, ifs)") {
                std::list<std::string> answer;
                std::string query = "if ifs; Select ifs such that Parent(14, ifs)";
                std::list<std::string> expected;
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("if ifs; Select ifs such that Parent(19, ifs)") {
                std::list<std::string> answer;
                std::string query = "if ifs; Select ifs such that Parent(19, ifs)";
                std::list<std::string> expected;
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("if ifs; Select ifs such that Parent(ifs, 20)") {
                std::list<std::string> answer;
                std::string query = "if ifs; Select ifs such that Parent(ifs, 20)";
                std::list<std::string> expected({"19"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("if ifs; Select ifs such that Parent(ifs, 21)") {
                std::list<std::string> answer;
                std::string query = "if ifs; Select ifs such that Parent(ifs, 21)";
                std::list<std::string> expected({"19"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("if ifs; Select ifs such that Parent(ifs, 22)") {
                std::list<std::string> answer;
                std::string query = "if ifs; Select ifs such that Parent(ifs, 22)";
                std::list<std::string> expected({"19"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }
        }

        SECTION("Parent select assign") {
            SECTION("assign a; Select a such that Parent(a, 11)") {
                std::list<std::string> answer;
                std::string query = "assign a; Select a such that Parent(a, 11)";
                std::list<std::string> expected;
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("assign a; Select a such that Parent(14, a)") {
                std::list<std::string> answer;
                std::string query = "assign a; Select a such that Parent(14, a)";
                std::list<std::string> expected({"15", "16", "17"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("assign a; Select a such that Parent(19, a)") {
                std::list<std::string> answer;
                std::string query = "assign a; Select a such that Parent(19, a)";
                std::list<std::string> expected({"20", "21", "22"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }
        }

        SECTION("Parent select call)") {
            SECTION("call c; Select c such that Parent(14, c)") {
                std::list<std::string> answer;
                std::string query = "call c; Select c such that Parent(14, c)";
                std::list<std::string> expected({"18"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("call c; Select c such that Parent(c, 2)") {
                std::list<std::string> answer;
                std::string query = "call c; Select c such that Parent(c, 2)";
                std::list<std::string> expected;
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("Parent(calls): Test 3") {
                std::list<std::string> answer;
                std::string query = "call c; Select c such that Parent(19, c)";
                std::list<std::string> expected;
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }
        }

        SECTION("Parent mixed") {
            SECTION("while w; call cl; Select w such that Parent(w, cl)") {
                std::list<std::string> answer;
                std::string query = "while w; call cl; Select w such that Parent(w, cl)";
                std::list<std::string> expected({"14"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("while w; call cl; Select cl.procName such that Parent(w, cl)") {
                std::list<std::string> answer;
                std::string query = "while w; call cl; Select cl.procName such that Parent(w, cl)";
                std::list<std::string> expected({"readPoint"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("assign a; if ifs; Select a such that Parent(ifs, a)") {
                std::list<std::string> answer;
                std::string query = "assign a; if ifs; Select a such that Parent(ifs, a)";
                std::list<std::string> expected({"20", "21", "22"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("assign a; while w; Select a such that Parent(w, a)") {
                std::list<std::string> answer;
                std::string query = "assign a; while w; Select a such that Parent(w, a)";
                std::list<std::string> expected({"15", "16", "17"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("assign a; while w; Select a.stmt# such that Parent(w, a)") {
                std::list<std::string> answer;
                std::string query = "assign a; while w; Select a.stmt# such that Parent(w, a)";
                std::list<std::string> expected({"15", "16", "17"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("assign a; if ifs; Select a.stmt# such that Parent(ifs, a)") {
                std::list<std::string> answer;
                std::string query = "assign a; if ifs; Select a.stmt# such that Parent(ifs, a)";
                std::list<std::string> expected({"20", "21", "22"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("assign a; if ifs; Select ifs such that Parent(ifs, a)") {
                std::list<std::string> answer;
                std::string query = "assign a; if ifs; Select ifs such that Parent(ifs, a)";
                std::list<std::string> expected({"19"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }
        }

        SECTION("Parent with wildcard") {
            SECTION("stmt s; Select s such that Parent(s, _)") {
                std::list<std::string> answer;
                std::string query = "stmt s; Select s such that Parent(s, _)";
                std::list<std::string> expected({"14", "19"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("print p; Select p such that Parent(_, p)") {
                std::list<std::string> answer;
                std::string query = "print p; Select p such that Parent(_, p)";
                std::list<std::string> expected;
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("call cl; Select cl such that Parent(_, cl)") {
                std::list<std::string> answer;
                std::string query = "call cl; Select cl such that Parent(_, cl)";
                std::list<std::string> expected({"18"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("read r; Select r such that Parent(_, r)") {
                std::list<std::string> answer;
                std::string query = "read r; Select r such that Parent(_, r)";
                std::list<std::string> expected;
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("if ifs; Select ifs such that Parent(ifs, _)") {
                std::list<std::string> answer;
                std::string query = "if ifs; Select ifs such that Parent(ifs, _)";
                std::list<std::string> expected({"19"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("if ifs; Select ifs such that Parent(_, ifs)") {
                std::list<std::string> answer;
                std::string query = "if ifs; Select ifs such that Parent(_, ifs)";
                std::list<std::string> expected;
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("while w; Select w such that Parent(w, _)") {
                std::list<std::string> answer;
                std::string query = "while w; Select w such that Parent(w, _)";
                std::list<std::string> expected({"14"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("while w; Select w such that Parent(_, w)") {
                std::list<std::string> answer;
                std::string query = "while w; Select w such that Parent(_, w)";
                std::list<std::string> expected;
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("assign a; Select a such that Parent(_, a)") {
                std::list<std::string> answer;
                std::string query = "assign a; Select a such that Parent(_, a)";
                std::list<std::string> expected({"15", "16", "17",
                                                 "20", "21", "22"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("stmt s; Select s such that Parent(12, _)") {
                std::list<std::string> answer;
                std::string query = "stmt s; Select s such that Parent(12, _)";
                std::list<std::string> expected;
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("stmt s; Select s such that Parent(14, _)") {
                std::list<std::string> answer;
                std::string query = "stmt s; Select s such that Parent(14, _)";
                std::list<std::string> expected({"1", "10", "11", "12", "13", "14", "15", "16",
                                                 "17", "18", "19", "2", "20", "21", "22", "23",
                                                 "3", "4", "5", "6", "7", "8", "9"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }
        }
    }SECTION("Parent*") {
        SECTION("Parent* select stmt") {
            SECTION("stmt s; Select s such that Parent*(1, s)") {
                std::list<std::string> answer;
                std::string query = "stmt s; Select s such that Parent*(1, s)";
                std::list<std::string> expected;
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("stmt s; Select s such that Parent*(19, s)") {
                std::list<std::string> answer;
                std::string query = "stmt s; Select s such that Parent*(19, s)";
                std::list<std::string> expected({"20", "21", "22"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("stmt s; Select s such that Parent*(s, 1)") {
                std::list<std::string> answer;
                std::string query = "stmt s; Select s such that Parent*(s, 1)";
                std::list<std::string> expected;
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("stmt s; Select s such that Parent*(s, 22)") {
                std::list<std::string> answer;
                std::string query = "stmt s; Select s such that Parent*(s, 22)";
                std::list<std::string> expected({"19"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("stmt s; Select s such that Parent*(s, 23)") {
                std::list<std::string> answer;
                std::string query = "stmt s; Select s such that Parent*(s, 23)";
                std::list<std::string> expected;
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }
        }

        SECTION("Parent* select read") {
            SECTION("read r; Select r such that Parent*(4, r)") {
                std::list<std::string> answer;
                std::string query = "read r; Select r such that Parent*(4, r)";
                std::list<std::string> expected;
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("read r; Select r such that Parent*(r, 5)") {
                std::list<std::string> answer;
                std::string query = "read r; Select r such that Parent*(r, 5)";
                std::list<std::string> expected;
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }
        }SECTION("Parent* select print") {
            SECTION("print p; Select p such that Parent*(p, 7)") {
                std::list<std::string> answer;
                std::string query = "print p; Select p such that Parent*(p, 7)";
                std::list<std::string> expected;
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("print p; Select p such that Parent*(8, p)") {
                std::list<std::string> answer;
                std::string query = "print p; Select p such that Parent*(8, p)";
                std::list<std::string> expected;
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }
        }

        SECTION("Parent* select while") {
            SECTION("while w; Select w such that Parent*(w, 15)") {
                std::list<std::string> answer;
                std::string query = "while w; Select w such that Parent*(w, 15)";
                std::list<std::string> expected({"14"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("while w; Select w such that Parent*(14, w)") {
                std::list<std::string> answer;
                std::string query = "while w; Select w such that Parent*(14, w)";
                std::list<std::string> expected;
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("while w; Select w such that Parent*(w, 16)") {
                std::list<std::string> answer;
                std::string query = "while w; Select w such that Parent*(w, 16)";
                std::list<std::string> expected({"14"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("while w; Select w such that Parent*(w, 17)") {
                std::list<std::string> answer;
                std::string query = "while w; Select w such that Parent*(w, 17)";
                std::list<std::string> expected({"14"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("while w; Select w such that Parent*(w, 18)") {
                std::list<std::string> answer;
                std::string query = "while w; Select w such that Parent*(w, 18)";
                std::list<std::string> expected({"14"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }
        }SECTION("Parent* select if") {
            SECTION("if ifs; Select ifs such that Parent*(14, ifs)") {
                std::list<std::string> answer;
                std::string query = "if ifs; Select ifs such that Parent*(14, ifs)";
                std::list<std::string> expected;
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("if ifs; Select ifs such that Parent*(19, ifs)") {
                std::list<std::string> answer;
                std::string query = "if ifs; Select ifs such that Parent*(19, ifs)";
                std::list<std::string> expected;
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("if ifs; Select ifs such that Parent*(ifs, 20)") {
                std::list<std::string> answer;
                std::string query = "if ifs; Select ifs such that Parent*(ifs, 20)";
                std::list<std::string> expected({"19"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("if ifs; Select ifs such that Parent*(ifs, 21)") {
                std::list<std::string> answer;
                std::string query = "if ifs; Select ifs such that Parent*(ifs, 21)";
                std::list<std::string> expected({"19"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("if ifs; Select ifs such that Parent*(ifs, 22)") {
                std::list<std::string> answer;
                std::string query = "if ifs; Select ifs such that Parent*(ifs, 22)";
                std::list<std::string> expected({"19"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }
        }

        SECTION("Parent* select call)") {
            SECTION("call c; Select c such that Parent*(14, c)") {
                std::list<std::string> answer;
                std::string query = "call c; Select c such that Parent*(14, c)";
                std::list<std::string> expected({"18"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("call c; Select c such that Parent*(c, 2)") {
                std::list<std::string> answer;
                std::string query = "call c; Select c such that Parent*(c, 2)";
                std::list<std::string> expected;
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("call c; Select c such that Parent*(19, c)") {
                std::list<std::string> answer;
                std::string query = "call c; Select c such that Parent*(19, c)";
                std::list<std::string> expected;
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }
        }SECTION("Parent* mixed") {
            SECTION("while w; call cl; Select w such that Parent*(w, cl)") {
                std::list<std::string> answer;
                std::string query = "while w; call cl; Select w such that Parent*(w, cl)";
                std::list<std::string> expected({"14"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("while w; call cl; Select cl.procName such that Parent*(w, cl)") {
                std::list<std::string> answer;
                std::string query = "while w; call cl; Select cl.procName such that Parent*(w, cl)";
                std::list<std::string> expected({"readPoint"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("assign a; if ifs; Select a such that Parent*(ifs, a)") {
                std::list<std::string> answer;
                std::string query = "assign a; if ifs; Select a such that Parent*(ifs, a)";
                std::list<std::string> expected({"20", "21", "22"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("assign a; while w; Select a such that Parent*(w, a)") {
                std::list<std::string> answer;
                std::string query = "assign a; while w; Select a such that Parent*(w, a)";
                std::list<std::string> expected({"15", "16", "17"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("assign a; while w; Select a.stmt# such that Parent*(w, a)") {
                std::list<std::string> answer;
                std::string query = "assign a; while w; Select a.stmt# such that Parent*(w, a)";
                std::list<std::string> expected({"15", "16", "17"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("assign a; if ifs; Select a.stmt# such that Parent*(ifs, a)") {
                std::list<std::string> answer;
                std::string query = "assign a; if ifs; Select a.stmt# such that Parent*(ifs, a)";
                std::list<std::string> expected({"20", "21", "22"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }

            SECTION("assign a; if ifs; Select ifs such that Parent*(ifs, a)") {
                std::list<std::string> answer;
                std::string query = "assign a; if ifs; Select ifs such that Parent*(ifs, a)";
                std::list<std::string> expected({"19"});
                qps.executeQuery(query, answer);
                requireSameListNoOrder(expected, answer);
            }
        }
    }

    SECTION("Modifies") {
        SECTION("call c; variable v; Select c such that Modifies(c, v)") {
            std::list<std::string> answer;
            std::string query = "call c; variable v; Select c such that Modifies(c, v)";
            std::list<std::string> expected({"13", "2", "18"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("call c; variable v; Select v such that Modifies(c, v)") {
            std::list<std::string> answer;
            std::string query = "call c; variable v; Select v such that Modifies(c, v)";
            std::list<std::string> expected({"cenX", "cenY", "count", "flag",
                                             "normSq", "x", "y"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("call c; Select c such that Modifies(c, \"flag\")") {
            std::list<std::string> answer;
            std::string query = "call c; Select c such that Modifies(c, \"flag\")";
            std::list<std::string> expected({"2"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("call c; Select c such that Modifies(c, \"cenX\")") {
            std::list<std::string> answer;
            std::string query = "call c; Select c such that Modifies(c, \"cenX\")";
            std::list<std::string> expected({"2"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("call c; Select c such that Modifies(c, \"x\")") {
            std::list<std::string> answer;
            std::string query = "call c; Select c such that Modifies(c, \"x\")";
            std::list<std::string> expected({"13", "18", "2"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("call c; Select c such that Modifies(c, _)") {
            std::list<std::string> answer;
            std::string query = "call c; Select c such that Modifies(c, _)";
            std::list<std::string> expected({"13", "18", "2"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("variable v; Select v such that Modifies(20, v)") {
            std::list<std::string> answer;
            std::string query = "variable v; Select v such that Modifies(20, v)";
            std::list<std::string> expected({"flag"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("variable v; Select v such that Modifies(10, v)") {
            std::list<std::string> answer;
            std::string query = "variable v; Select v such that Modifies(10, v)";
            std::list<std::string> expected({"count"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("variable v; Select v such that Modifies(3, v)") {
            std::list<std::string> answer;
            std::string query = "variable v; Select v such that Modifies(3, v)";
            std::list<std::string> expected;
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("print p; Select p such that Modifies(2, _)") {
            std::list<std::string> answer;
            std::string query = "print p; Select p such that Modifies(2, _)";
            std::list<std::string> expected({"6", "7", "8", "9"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("procedure p; Select p such that Modifies(17, _)") {
            std::list<std::string> answer;
            std::string query = "procedure p; Select p such that Modifies(17, _)";
            std::list<std::string> expected({"main", "readPoint", "computeCentroid", "printResults"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("call c; variable v; Select c.procName such that Modifies(c, \"flag\")") {
            std::list<std::string> answer;
            std::string query = "call c; variable v; Select c.procName such that Modifies(c, \"flag\")";
            std::list<std::string> expected({"computeCentroid"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("procedure p; variable v; Select p.procName such that Modifies(p, \"flag\")") {
            std::list<std::string> answer;
            std::string query = "procedure p; variable v; Select p.procName such that Modifies(p, \"flag\")";
            std::list<std::string> expected({"main", "computeCentroid"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("procedure p; variable v; Select p.procName such that Modifies(p, \"flag\")") {
            std::list<std::string> answer;
            std::string query = "procedure p; variable v; Select p.procName such that Modifies(p, \"flag\")";
            std::list<std::string> expected({"main", "computeCentroid"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("procedure p; variable v; Select v.varName such that Modifies(\"computeCentroid\", v)") {
            std::list<std::string> answer;
            std::string query = "procedure p; variable v; Select v.varName such that Modifies(\"computeCentroid\", v)";
            std::list<std::string> expected({"cenX", "cenY", "count", "flag", "normSq", "x", "y"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("while w; Select w such that Modifies(17, _)") {
            std::list<std::string> answer;
            std::string query = "while w; Select w such that Modifies(17, _)";
            std::list<std::string> expected({"14"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("if ifs; Select ifs such that Modifies(1, _)") {
            std::list<std::string> answer;
            std::string query = "if ifs; Select ifs such that Modifies(1, _)";
            std::list<std::string> expected({"19"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("read r; Select r such that Modifies(2, _)") {
            std::list<std::string> answer;
            std::string query = "read r; Select r such that Modifies(2, _)";
            std::list<std::string> expected({"4", "5"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("print p; Select p such that Modifies(16, _)") {
            std::list<std::string> answer;
            std::string query = "print p; Select p such that Modifies(16, _)";
            std::list<std::string> expected({"6", "7", "8", "9"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("stmt s; Select s such that Modifies(s, \"cenX\")") {
            std::list<std::string> answer;
            std::string query = "stmt s; Select s such that Modifies(s, \"cenX\")";
            std::list<std::string> expected({"11", "14", "16",
                                             "19", "2", "21"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("stmt s; Select s such that Modifies(s, \"cenY\")") {
            std::list<std::string> answer;
            std::string query = "stmt s; Select s such that Modifies(s, \"cenY\")";
            std::list<std::string> expected({"12", "14", "17",
                                             "19", "2", "22"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("assign a; Select a such that Modifies(a, _)") {
            std::list<std::string> answer;
            std::string query = "assign a; Select a such that Modifies(a, _)";
            std::list<std::string> expected({"1", "10", "11", "12", "15", "16", "17", "20", "21", "22", "23"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("procedure p; Select p such that Modifies(p, _)") {
            std::list<std::string> answer;
            std::string query = "procedure p; Select p such that Modifies(p, _)";
            std::list<std::string> expected({"main", "readPoint", "computeCentroid"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

//        SECTION("variable v; Select v such that Modifies(_, v)") {
//            std::list<std::string> answer;
//            std::string query = " procedure p; variable v; Select <p, v> such that Modifies(p, v)";
//            std::list<std::string> expected({"x", "y", "flag", "cenX", "cenY", "count", "normSq"});
//            qps.executeQuery(query, answer);
//            requireSameListNoOrder(expected, answer);
//        }
    }

    SECTION("Uses") {
        SECTION("call c; variable v; Select c such that Uses(c, v)") {
            std::list<std::string> answer;
            std::string query = "call c; variable v; Select c such that Uses(c, v)";
            std::list<std::string> expected({"2", "3"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("call c; variable v; Select v such that Uses(c, v)") {
            std::list<std::string> answer;
            std::string query = "call c; variable v; Select v such that Uses(c, v)";
            std::list<std::string> expected({"cenX", "cenY", "count", "flag",
                                             "normSq", "x", "y"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("call c; Select c such that Uses(c, \"flag\")") {
            std::list<std::string> answer;
            std::string query = "call c; Select c such that Uses(c, \"flag\")";
            std::list<std::string> expected({"3"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("call c; Select c such that Uses(c, \"cenX\")") {
            std::list<std::string> answer;
            std::string query = "call c; Select c such that Uses(c, \"cenX\")";
            std::list<std::string> expected({"2", "3"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("call c; Select c such that Uses(c, \"x\")") {
            std::list<std::string> answer;
            std::string query = "call c; Select c such that Uses(c, \"x\")";
            std::list<std::string> expected({"2"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("call c; Select c such that Uses(c, _)") {
            std::list<std::string> answer;
            std::string query = "call c; Select c such that Uses(c, _)";
            std::list<std::string> expected({"2", "3"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("variable v; Select v such that Uses(14, v)") {
            std::list<std::string> answer;
            std::string query = "variable v; Select v such that Uses(14, v)";
            std::list<std::string> expected({"cenX", "cenY", "count", "x", "y"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("variable v; Select v such that Uses(10, v)") {
            std::list<std::string> answer;
            std::string query = "variable v; Select v such that Uses(10, v)";
            std::list<std::string> expected;
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("variable v; Select v such that Uses(3, v)") {
            std::list<std::string> answer;
            std::string query = "variable v; Select v such that Uses(3, v)";
            std::list<std::string> expected({"cenX", "cenY", "flag", "normSq"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("variable v; Select v such that Uses(19, v)") {
            std::list<std::string> answer;
            std::string query = "variable v; Select v such that Uses(19, v)";
            std::list<std::string> expected({"cenX", "cenY", "count"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("print p; Select p such that Uses(2, _)") {
            std::list<std::string> answer;
            std::string query = "print p; Select p such that Uses(2, _)";
            std::list<std::string> expected({"6", "7", "8", "9"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("procedure p; Select p such that Uses(17, _)") {
            std::list<std::string> answer;
            std::string query = "procedure p; Select p such that Uses(17, _)";
            std::list<std::string> expected({"main", "readPoint", "computeCentroid", "printResults"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("procedure p; variable v; Select p.procName such that Uses(p, \"flag\")") {
            std::list<std::string> answer;
            std::string query = "procedure p; variable v; Select p.procName such that Uses(p, \"flag\")";
            std::list<std::string> expected({"main", "printResults"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("procedure p; variable v; Select p.procName such that Uses(p, \"flag\")") {
            std::list<std::string> answer;
            std::string query = "procedure p; variable v; Select p.procName such that Uses(p, \"flag\")";
            std::list<std::string> expected({"main", "printResults"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("procedure p; variable v; Select v.varName such that  Uses(\"computeCentroid\", v)") {
            std::list<std::string> answer;
            std::string query = "procedure p; variable v; Select v.varName such that  Uses(\"computeCentroid\", v)";
            std::list<std::string> expected({"cenX", "cenY", "count", "x", "y"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("while w; variable v; Select v such that Uses(w, v)") {
            std::list<std::string> answer;
            std::string query = "while w; variable v; Select v such that Uses(w, v)";
            std::list<std::string> expected({"cenX", "cenY", "count", "x", "y"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("if ifs; variable v; Select v such that Uses(ifs, v)") {
            std::list<std::string> answer;
            std::string query = "if ifs; variable v; Select v such that Uses(ifs, v)";
            std::list<std::string> expected({"cenX", "cenY", "count"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("read r; Select r such that Uses(2, _)") {
            std::list<std::string> answer;
            std::string query = "read r; Select r such that Uses(2, _)";
            std::list<std::string> expected({"4", "5"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("print p; Select p such that Uses(16, _)") {
            std::list<std::string> answer;
            std::string query = "print p; Select p such that Uses(16, _)";
            std::list<std::string> expected({"6", "7", "8", "9"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("stmt s; Select s such that Uses(s, \"cenX\")") {
            std::list<std::string> answer;
            std::string query = "stmt s; Select s such that Uses(s, \"cenX\")";
            std::list<std::string> expected({"14", "16", "19", "2",
                                             "23", "21", "3", "7"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("stmt s; Select s such that Uses(s, \"cenY\")") {
            std::list<std::string> answer;
            std::string query = "stmt s; Select s such that Uses(s, \"cenY\")";
            std::list<std::string> expected({"14", "17", "19", "2", "22",
                                             "23", "3", "8"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("assign a; Select a such that Uses(a, _)") {
            std::list<std::string> answer;
            std::string query = "assign a; Select a such that Uses(a, _)";
            std::list<std::string> expected({"15", "16", "17", "21", "22", "23"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }
    }

    SECTION("Relationship with same synonyms") {
        SECTION("Parent") {
            std::list<std::string> answer;
            std::string query = "stmt s; Select s such that Parent(s, s)";
            std::list<std::string> expected;
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("Parent*") {
            std::list<std::string> answer;
            std::string query = "stmt s; Select s such that Parent*(s, s)";
            std::list<std::string> expected;
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("Follows") {
            std::list<std::string> answer;
            std::string query = "stmt s; Select s such that Follows(s, s)";
            std::list<std::string> expected;
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("Follows*") {
            std::list<std::string> answer;
            std::string query = "stmt s; Select s such that Follows*(s, s)";
            std::list<std::string> expected;
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("Calls") {
            std::list<std::string> answer;
            std::string query = "procedure p; Select p such that Calls(p, p)";
            std::list<std::string> expected;
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }
    }

    SECTION("Pattern") {
        SECTION("assign a; Select a pattern a(_, _)") {
            std::list<std::string> answer;
            std::string query = "assign a; Select a pattern a(_, _)";
            std::list<std::string> expected({"1", "10", "11", "12",
                                             "15", "16", "17", "20",
                                             "21", "22", "23"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("assign a; Select a pattern a(_, \"count + 1\")") {
            std::list<std::string> answer;
            std::string query = "assign a; Select a pattern a(_, \"count + 1\")";
            std::list<std::string> expected({"15"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("assign a; Select a pattern a(_, \"count\")") {
            std::list<std::string> answer;
            std::string query = "assign a; Select a pattern a(_, \"count\")";
            std::list<std::string> expected({});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("assign a, variable v; Select <a, v> pattern a(v, \"count + 1\")") {
            std::list<std::string> answer;
            std::string query = "assign a; variable v; Select <a, v> pattern a(v, \"count + 1\")";
            std::list<std::string> expected({"15 count"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("assign a, variable v; Select <a, v> pattern a(v, _\"count\"_)") {
            std::list<std::string> answer;
            std::string query = "assign a; variable v; Select <a, v> pattern a(v, _\"count\"_)";
            std::list<std::string> expected({"15 count", "21 cenX", "22 cenY"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("assign a, variable v; Select a pattern a(\"count\", _\"count\"_)") {
            std::list<std::string> answer;
            std::string query = "assign a; variable v; Select a pattern a(\"count\", _\"count\"_)";
            std::list<std::string> expected({"15"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("assign a, variable v; Select a pattern a(\"count\", _\"count + 1\"_)") {
            std::list<std::string> answer;
            std::string query = "assign a; variable v; Select a pattern a(\"count\", \"count + 1\")";
            std::list<std::string> expected({"15"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }
    }

    SECTION("affects") {
        SECTION("Select BOOLEAN such that Affects(1,3)") {
            std::list<std::string> answer;
            std::string query = "Select BOOLEAN such that Affects(1,3)";
            std::list<std::string> expected({"FALSE"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("Select BOOLEAN such that Affects(1,2)") {
            std::list<std::string> answer;
            std::string query = "Select BOOLEAN such that Affects(1,2)";
            std::list<std::string> expected({"FALSE"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("stmt s; Select s such that Affects(s,17)") {
            std::list<std::string> answer;
            std::string query = "stmt s; Select s such that Affects(s,17)";
            std::list<std::string> expected({"12", "17"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("stmt s; Select s such that Affects(17,s)") {
            std::list<std::string> answer;
            std::string query = "stmt s; Select s such that Affects(17,s)";
            std::list<std::string> expected({"17", "22", "23"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("stmt s; Select s such that Affects(s, _)") {
            std::list<std::string> answer;
            std::string query = "stmt s; Select s such that Affects(s, _)";
            std::list<std::string> expected({"10", "11", "12", "15", "16", "17", "22", "21"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("stmt s; Select s such that Affects(_, s)") {
            std::list<std::string> answer;
            std::string query = "stmt s; Select s such that Affects(_, s)";
            std::list<std::string> expected({"23", "15", "16", "17", "22", "21"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("stmt s; Select s such that Affects(s, s)") {
            std::list<std::string> answer;
            std::string query = "stmt s; Select s such that Affects(s, s)";
            std::list<std::string> expected({"15", "16", "17"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("stmt s, s1; Select <s, s1> such that Affects(s, s1)") {
            std::list<std::string> answer;
            std::string query = "stmt s, s1; Select <s, s1> such that Affects(s, s1)";
            std::list<std::string> expected({"10 15", "10 21", "10 22", "11 16", "11 21", "11 23",
                                             "12 22", "12 23", "15 15", "15 21", "15 22", "16 16",
                                             "16 21", "16 23", "17 17", "17 22", "17 23", "22 23",
                                             "21 23", "12 17"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("Select BOOLEAN such that Affects(_, _)") {
            std::list<std::string> answer;
            std::string query = "Select BOOLEAN such that Affects(_, _)";
            std::list<std::string> expected({"TRUE"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }
    }

    SECTION("affects*") {
        SECTION("Select BOOLEAN such that Affects*(16,16)") {
            std::list<std::string> answer;
            std::string query = "Select BOOLEAN such that Affects(16,16)";
            std::list<std::string> expected({"TRUE"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("Select BOOLEAN such that Affects*(1,2)") {
            std::list<std::string> answer;
            std::string query = "Select BOOLEAN such that Affects*(1,2)";
            std::list<std::string> expected({"FALSE"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("Select BOOLEAN such that Affects*(16,16)") {
            std::list<std::string> answer;
            std::string query = "Select BOOLEAN such that Affects*(16,16)";
            std::list<std::string> expected({"TRUE"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("stmt s; Select s such that Affects*(s,17)") {
            std::list<std::string> answer;
            std::string query = "stmt s; Select s such that Affects*(s,17)";
            std::list<std::string> expected({"12", "17"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("stmt s; Select s such that Affects*(17,s)") {
            std::list<std::string> answer;
            std::string query = "stmt s; Select s such that Affects*(17,s)";
            std::list<std::string> expected({"17", "22", "23"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("stmt s; Select s such that Affects*(s, _)") {
            std::list<std::string> answer;
            std::string query = "stmt s; Select s such that Affects*(s, _)";
            std::list<std::string> expected({"10", "11", "12", "15", "16", "17", "22", "21"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("stmt s; Select s such that Affects*(_, s)") {
            std::list<std::string> answer;
            std::string query = "stmt s; Select s such that Affects*(_, s)";
            std::list<std::string> expected({"23", "15", "16", "17", "22", "21"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("Select BOOLEAN such that Affects*(_, _)") {
            std::list<std::string> answer;
            std::string query = "Select BOOLEAN such that Affects*(_, _)";
            std::list<std::string> expected({"TRUE"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }

        SECTION("stmt s, s1; Select <s, s1> such that Affects*(s, s1)") {
            std::list<std::string> answer;
            std::string query = "stmt s, s1; Select <s, s1> such that Affects*(s, s1)";
            std::list<std::string> expected({"10 15", "10 21", "10 22", "11 16", "11 21", "11 23",
                                             "12 22", "12 23", "15 15", "15 21", "15 22", "16 16",
                                             "16 21", "16 23", "17 17", "17 22", "17 23", "22 23",
                                             "21 23", "12 17", "10 23", "15 23"});
            qps.executeQuery(query, answer);
            requireSameListNoOrder(expected, answer);
        }
    }
}
