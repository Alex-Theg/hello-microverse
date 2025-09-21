cmake_minimum_required(VERSION 3.16)
project(greeter LANGUAGES CXX)
add_executable(greeter src/main.cpp ${CMAKE_SOURCE_DIR}/include/openapi_annotations.cpp)
target_include_directories(greeter PRIVATE include ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(greeter 
    httplib::httplib
    nlohmann_json::nlohmann_json
    spdlog::spdlog
)
