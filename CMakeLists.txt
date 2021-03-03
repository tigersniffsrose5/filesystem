cmake_minimum_required(VERSION 3.10)

project(ffs)

set(CMAKE_CXX_STANDARD 17)                                 #使用c++ 17
set(CMAKE_CXX_STANDARD_REQUIRED ON)                        #如果将此属性设置为ON，则CXX_STANDARD目标属性被视为要求。如果此属性已OFF设置或未设置，则CXX_STANDARD目标属性被视为可选属性
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")            #设置C++编译选项

include_directories("Inc")

file(GLOB_RECURSE INCLUDES "Inc/*.h" "Inc/*.hpp")

aux_source_directory("Src" SOURCES)


set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_executable(${PROJECT_NAME} ${SOURCES} ${INCLUDES})
target_link_libraries(${PROJECT_NAME} stdc++fs)            #链接共享库 

