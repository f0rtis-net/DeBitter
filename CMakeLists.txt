cmake_minimum_required(VERSION 3.15)

project(de_bitter CXX)

add_library(project_options INTERFACE)                       
target_compile_features(project_options INTERFACE cxx_std_17)

include_directories(${PROJECT_SOURCE_DIR}/inc)

add_executable(de_bitter 
    src/main.cpp 
    src/Lexer.cpp
    src/Parser.cpp
    src/Optimizer.cpp
    src/TreePrinter.cpp
    src/Runtime.cpp
)

target_include_directories(de_bitter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})                                      
target_link_libraries(de_bitter PRIVATE project_options)