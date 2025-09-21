cmake_minimum_required(VERSION 3.16)
project(hello-microverse LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
include(cmake/common.cmake)
include(cmake/dependencies.cmake)

add_subdirectory(gateway)
add_subdirectory(services/greeter)
add_subdirectory(services/identity)
add_subdirectory(services/preferences)
add_subdirectory(services/audit)

# Custom targets for running services
# Best practice: Use add_custom_target for non-file targets
add_custom_target(run-gateway
    COMMAND gateway
    DEPENDS gateway
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Starting API Gateway on port 8080"
)

add_custom_target(run-greeter
    COMMAND greeter
    DEPENDS greeter
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/services/greeter
    COMMENT "Starting Greeter service on port 8081"
)

add_custom_target(run-identity
    COMMAND identity
    DEPENDS identity
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/services/identity
    COMMENT "Starting Identity service on port 8082"
)

add_custom_target(run-preferences
    COMMAND preferences
    DEPENDS preferences
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/services/preferences
    COMMENT "Starting Preferences service on port 8083"
)

add_custom_target(run-audit
    COMMAND audit
    DEPENDS audit
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/services/audit
    COMMENT "Starting Audit service on port 8084"
)

# Default run target (runs gateway)
add_custom_target(run
    COMMAND gateway
    DEPENDS gateway
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Starting API Gateway (default service)"
)

# Test target for health checks
add_custom_target(test-services
    COMMAND ${CMAKE_COMMAND} -E echo "Testing service health endpoints..."
    COMMAND ${CMAKE_COMMAND} -E echo "Gateway: http://localhost:8080/healthz"
    COMMAND ${CMAKE_COMMAND} -E echo "Greeter: http://localhost:8081/healthz"
    COMMAND ${CMAKE_COMMAND} -E echo "Identity: http://localhost:8082/healthz"
    COMMAND ${CMAKE_COMMAND} -E echo "Preferences: http://localhost:8083/healthz"
    COMMAND ${CMAKE_COMMAND} -E echo "Audit: http://localhost:8084/healthz"
    COMMENT "Service health check endpoints"
)

# Help target to show available commands
add_custom_target(help-services
    COMMAND ${CMAKE_COMMAND} -E echo "Available service targets:"
    COMMAND ${CMAKE_COMMAND} -E echo "  cmake --build . --target run"
    COMMAND ${CMAKE_COMMAND} -E echo "  cmake --build . --target run-gateway"
    COMMAND ${CMAKE_COMMAND} -E echo "  cmake --build . --target run-greeter"
    COMMAND ${CMAKE_COMMAND} -E echo "  cmake --build . --target run-identity"
    COMMAND ${CMAKE_COMMAND} -E echo "  cmake --build . --target run-preferences"
    COMMAND ${CMAKE_COMMAND} -E echo "  cmake --build . --target run-audit"
    COMMAND ${CMAKE_COMMAND} -E echo "  cmake --build . --target test-services"
    COMMENT "Available service targets"
)
