cmake_minimum_required(VERSION 3.16)
project(preferences LANGUAGES CXX)
add_executable(preferences src/main.cpp)
target_include_directories(preferences PRIVATE include)
target_link_libraries(preferences 
    httplib::httplib
    nlohmann_json::nlohmann_json
    spdlog::spdlog
)
