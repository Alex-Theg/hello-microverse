cmake_minimum_required(VERSION 3.16)
project(hello-microverse LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
include(cmake/common.cmake)

add_subdirectory(gateway)
add_subdirectory(services/greeter)
add_subdirectory(services/identity)
add_subdirectory(services/preferences)
add_subdirectory(services/audit)
