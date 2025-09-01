cmake_minimum_required(VERSION 3.16)
project(greeter LANGUAGES CXX)
add_executable(greeter src/main.cpp)
target_include_directories(greeter PRIVATE include)
