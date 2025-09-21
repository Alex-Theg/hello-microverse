# cmake/dependencies.cmake
# FetchContent-based third-party dependencies for the greeting-platform monorepo.
# CMake ≥ 3.16 recommended.

include(FetchContent)

# ---------------------------------------------------------------------------
# Global FetchContent behavior
# ---------------------------------------------------------------------------
# Don’t re-check remotes unless versions/tags change (faster, reproducible).
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

# Avoid noisy builds; flip to OFF while debugging dependency builds.
set(FETCHCONTENT_QUIET ON)

# Helper: set a CMake cache option cleanly
function(_dep_option NAME DEFAULT DOCSTRING)
  if(NOT DEFINED ${NAME})
    set(${NAME} ${DEFAULT} CACHE BOOL "${DOCSTRING}")
  endif()
endfunction()

# ---------------------------------------------------------------------------
# nlohmann/json (header-only) — JSON parsing
# Target: nlohmann_json::nlohmann_json
# ---------------------------------------------------------------------------
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG        v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)

# ---------------------------------------------------------------------------
# fmt (formatting) — used by spdlog when externalized
# Target: fmt::fmt
# ---------------------------------------------------------------------------
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG        10.2.1
)
FetchContent_MakeAvailable(fmt)

# ---------------------------------------------------------------------------
# spdlog — structured logging
# Target: spdlog::spdlog
# Use external fmt to keep versions explicit.
# ---------------------------------------------------------------------------
_dep_option(SPDLOG_FMT_EXTERNAL ON "Use external fmt with spdlog")
_dep_option(SPDLOG_BUILD_SHARED OFF "Build spdlog as static lib")
_dep_option(SPDLOG_ENABLE_PCH   OFF "Disable PCH for portability")

FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG        v1.13.0
)
FetchContent_MakeAvailable(spdlog)

# ---------------------------------------------------------------------------
# yaml-cpp — parse OpenAPI YAML in the DIY gateway
# Target: yaml-cpp
# Temporarily disabled due to CMake compatibility issues
# ---------------------------------------------------------------------------
# _dep_option(YAML_CPP_BUILD_TESTS     OFF "yaml-cpp: no tests")
# _dep_option(YAML_CPP_BUILD_TOOLS     OFF "yaml-cpp: no tools")
# _dep_option(YAML_CPP_BUILD_CONTRIB   OFF "yaml-cpp: no contrib")
# _dep_option(YAML_BUILD_SHARED_LIBS   OFF "yaml-cpp: build static")

# FetchContent_Declare(
#   yaml_cpp
#   GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
#   GIT_TAG        0.8.0
# )
# FetchContent_MakeAvailable(yaml_cpp)

# ---------------------------------------------------------------------------
# cpp-httplib — lightweight HTTP (client/server) for quick DIY gateway work
# Target: httplib::httplib
# Note: Header-only; ships with a simple CMakeLists exporting target.
# ---------------------------------------------------------------------------
_dep_option(HTTPLIB_ENABLE_OPENSSL    OFF "Disable SSL right now (no TLS phase)")
_dep_option(HTTPLIB_COMPILE           OFF "Header-only build")

FetchContent_Declare(
  cpp_httplib
  GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
  GIT_TAG        v0.15.3
)
FetchContent_MakeAvailable(cpp_httplib)

# Some versions don't export an imported target consistently; ensure it exists.
if(NOT TARGET httplib::httplib)
  add_library(httplib::httplib INTERFACE IMPORTED)
  target_include_directories(httplib::httplib INTERFACE
    ${cpp_httplib_SOURCE_DIR})
endif()

# ---------------------------------------------------------------------------
# Paho MQTT (C then C++) — MQTT client for Greeter/Audit
# Temporarily disabled due to CMake compatibility issues
# Targets:
#   PahoMqttC::paho-mqtt3a (async, no TLS)
#   PahoMqttCpp::paho-mqttpp3
# TLS is OFF for now per project phase; flip ON later.
# ---------------------------------------------------------------------------

# # paho.mqtt.c
# set(PAHO_BUILD_STATIC       ON  CACHE BOOL "" FORCE)
# set(PAHO_BUILD_SHARED       OFF CACHE BOOL "" FORCE)
# set(PAHO_WITH_SSL           OFF CACHE BOOL "" FORCE)
# set(PAHO_BUILD_SAMPLES      OFF CACHE BOOL "" FORCE)
# set(PAHO_BUILD_DOCUMENTATION OFF CACHE BOOL "" FORCE)
# set(PAHO_ENABLE_TESTING     OFF CACHE BOOL "" FORCE)

# FetchContent_Declare(
#   paho_mqtt_c
#   GIT_REPOSITORY https://github.com/eclipse/paho.mqtt.c.git
#   GIT_TAG        v1.3.13
# )
# FetchContent_MakeAvailable(paho_mqtt_c)

# # paho.mqtt.cpp
# set(PAHO_MQTT_C_DIR "${paho_mqtt_c_SOURCE_DIR}" CACHE PATH "" FORCE)
# set(PAHO_MQTT_C_LIB "${paho_mqtt_c_BINARY_DIR}" CACHE PATH "" FORCE)
# set(PAHO_WITH_SSL OFF CACHE BOOL "" FORCE)
# set(PAHO_BUILD_STATIC ON CACHE BOOL "" FORCE)

# FetchContent_Declare(
#   paho_mqtt_cpp
#   GIT_REPOSITORY https://github.com/eclipse/paho.mqtt.cpp.git
#   GIT_TAG        v1.3.2
# )
# FetchContent_MakeAvailable(paho_mqtt_cpp)

# # Convenience alias (some versions don't export nice namespace targets)
# if(NOT TARGET PahoMqttCpp::paho-mqttpp3 AND TARGET paho-mqttpp3)
#   add_library(PahoMqttCpp::paho-mqttpp3 ALIAS paho-mqttpp3)
# endif()
# if(NOT TARGET PahoMqttC::paho-mqtt3a AND TARGET paho-mqtt3a)
#   add_library(PahoMqttC::paho-mqtt3a ALIAS paho-mqtt3a)
# endif()

# ---------------------------------------------------------------------------
# Catch2 — unit testing (optional)
# Target: Catch2::Catch2WithMain
# ---------------------------------------------------------------------------
_dep_option(ENABLE_TESTING ON "Enable unit tests with Catch2")
if(ENABLE_TESTING)
  include(CTest)
  FetchContent_Declare(
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.5.2
  )
  FetchContent_MakeAvailable(Catch2)
endif()

# ---------------------------------------------------------------------------
# Summary (nice to have in configure logs)
# ---------------------------------------------------------------------------
message(STATUS "Third-party dependencies configured:")
message(STATUS "  - nlohmann_json::nlohmann_json")
message(STATUS "  - fmt::fmt")
message(STATUS "  - spdlog::spdlog (external fmt)")
# message(STATUS "  - yaml-cpp")
message(STATUS "  - httplib::httplib")
if(TARGET cpr::cpr)
  message(STATUS "  - cpr::cpr")
endif()
# message(STATUS "  - PahoMqttC::paho-mqtt3a")
# message(STATUS "  - PahoMqttCpp::paho-mqttpp3")
if(TARGET Catch2::Catch2WithMain)
  message(STATUS "  - Catch2::Catch2WithMain")
endif()
