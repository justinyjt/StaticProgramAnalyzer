file(GLOB_RECURSE srcs "${CMAKE_CURRENT_SOURCE_DIR}/src" *.cpp)
file(GLOB_RECURSE headers "${CMAKE_CURRENT_SOURCE_DIR}/src" *.h *.hpp)

add_executable(unit_testing ${srcs} src/sp/TestParser.cpp src/sp/MockParser.cpp src/sp/MockParser.h src/sp/MockLexer.cpp src/sp/MockLexer.h src/sp/ASTPrinter.cpp src/sp/ASTPrinter.h)

target_link_libraries(unit_testing spa)
