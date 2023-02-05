file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.hpp")

add_library(spa ${srcs} ${headers} src/qps/query_parser/clause_parser/pattern_clause_parser/PatternClauseParser.h src/qps/query_parser/clause_parser/pattern_clause_parser/PatternClauseParser.cpp src/qps/entities/Term.cpp)
# this makes the headers accessible for other projects which uses spa lib

target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)


