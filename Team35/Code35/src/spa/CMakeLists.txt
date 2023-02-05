file(GLOB_RECURSE srcs "${CMAKE_CURRENT_SOURCE_DIR}/src" *.cpp)
file(GLOB_RECURSE headers "${CMAKE_CURRENT_SOURCE_DIR}/src" *.h *.hpp)

add_library(spa ${srcs} ${headers} src/sp/IParser.h src/commons/ASTNode.cpp src/commons/ASTNode.h src/commons/Token.cpp src/commons/Token.h src/sp/Lexer.cpp src/sp/Lexer.h src/sp/DesignExtractor.h)
# this makes the headers accessible for other projects which uses spa lib

target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
