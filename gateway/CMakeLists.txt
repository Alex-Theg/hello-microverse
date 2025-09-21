cmake_minimum_required(VERSION 3.16)
project(gateway LANGUAGES CXX)
add_executable(gateway src/main.cpp)
target_include_directories(gateway PRIVATE include)
target_link_libraries(gateway 
    httplib::httplib
    nlohmann_json::nlohmann_json
    spdlog::spdlog
)
