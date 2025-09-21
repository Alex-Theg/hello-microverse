cmake_minimum_required(VERSION 3.16)
project(identity LANGUAGES CXX)
add_executable(identity src/main.cpp)
target_include_directories(identity PRIVATE include)
target_link_libraries(identity 
    httplib::httplib
    nlohmann_json::nlohmann_json
    spdlog::spdlog
)
