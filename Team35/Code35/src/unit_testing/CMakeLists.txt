file(GLOB_RECURSE srcs "${CMAKE_CURRENT_SOURCE_DIR}/src" *.cpp)
file(GLOB_RECURSE headers "${CMAKE_CURRENT_SOURCE_DIR}/src" *.h *.hpp)

add_executable(unit_testing ${srcs} src/commons/query_parser/TestQueryParser.cpp)

target_link_libraries(unit_testing spa)
