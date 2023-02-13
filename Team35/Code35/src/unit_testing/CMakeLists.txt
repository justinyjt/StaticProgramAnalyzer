file(GLOB_RECURSE srcs "${CMAKE_CURRENT_SOURCE_DIR}/src" *.cpp)
file(GLOB_RECURSE headers "${CMAKE_CURRENT_SOURCE_DIR}/src" *.h *.hpp)

add_executable(unit_testing ${srcs} src/qps/query_parser/TestDeclarationParser.cpp src/qps/query_parser/TestSelectionParser.cpp src/qps/query_parser/TestClauseParser.cpp)

target_link_libraries(unit_testing spa)
