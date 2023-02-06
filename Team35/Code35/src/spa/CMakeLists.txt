file(GLOB_RECURSE srcs "${CMAKE_CURRENT_SOURCE_DIR}/src" *.cpp)
file(GLOB_RECURSE headers "${CMAKE_CURRENT_SOURCE_DIR}/src" *.h *.hpp)

add_library(spa ${srcs} ${headers} src/qps/clause/pattern/Pattern.cpp src/qps/clause/relationship/Relationship.cpp src/qps/clause/Clause.cpp src/qps/pql/Synonym.cpp src/qps/pql/ExpressionStr.cpp src/qps/pql/Wildcard.cpp src/qps/pql/Tok.cpp src/qps/pql/StatementNumber.cpp src/qps/pql/IdentStr.cpp) #src/qps/query_parser/clause_parser/pattern_clause_parser/PatternClauseParser.h src/qps/query_parser/clause_parser/pattern_clause_parser/PatternClauseParser.cpp src/qps/pql/Term.cpp)
# this makes the headers accessible for other projects which uses spa lib

target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
