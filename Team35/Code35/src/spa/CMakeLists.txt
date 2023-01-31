file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.hpp")

##Commons Files
#set(srcs ${srcs}
#    src/commons/TNode.cpp)
#
#set(headers ${headers}
#        src/commons/TNode.h
#        src/commons/types.h)
#
##SP Files
#set(srcs ${srcs}
#        src/sp/DesignExtractor.cpp
#        src/sp/Parser.cpp)
#
#set(headers ${headers}
#        src/sp/Parser.h)
#
##PKB Files
#set(srcs ${srcs}
#        src/pkb/PKB.cpp)
#
#set(headers ${headers}
#        src/pkb/PKB.h
#        src/pkb/db/RelationshipTable.h)
#
#
##QPS Files
#set(srcs ${srcs}
#        src/qps/dummy.cpp) # TO BE REMOVED
#
#set(headers ${headers}
#        src/qps/dummy.h) # TO BE REMOVED


add_library(spa ${srcs} ${headers})
# this makes the headers accessible for other projects which uses spa lib

target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)


