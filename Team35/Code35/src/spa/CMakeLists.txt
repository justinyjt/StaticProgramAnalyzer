file(GLOB_RECURSE srcs "${CMAKE_CURRENT_SOURCE_DIR}/src" *.cpp)
file(GLOB_RECURSE headers "${CMAKE_CURRENT_SOURCE_DIR}/src" *.h *.hpp)

add_library(spa ${srcs} ${headers} src/qps/query_parser/clause_parser/pattern_clause_parser/PatternClauseParser.h src/qps/query_parser/clause_parser/pattern_clause_parser/PatternClauseParser.cpp src/qps/pql/Term.cpp)
# this makes the headers accessible for other projects which uses spa lib

target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
