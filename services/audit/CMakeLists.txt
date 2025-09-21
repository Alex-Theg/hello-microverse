cmake_minimum_required(VERSION 3.16)
project(audit LANGUAGES CXX)
add_executable(audit src/main.cpp)
target_include_directories(audit PRIVATE include)
target_link_libraries(audit 
    httplib::httplib
    nlohmann_json::nlohmann_json
    spdlog::spdlog
)
