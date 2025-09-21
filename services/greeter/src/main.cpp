#include <httplib.h>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include "openapi_annotations.h"

int main() {
    spdlog::info("Starting Greeter service...");
    
    // Set up OpenAPI service information
    OPENAPI_SERVICE("Greeter Service", "1.0.0", 
        "The Greeter service provides personalized greeting generation functionality. "
        "It creates custom greetings for users based on their preferences and context.");
    
    httplib::Server server;
    
    // Health check endpoint
    nlohmann::json health_response = {
        {"type", "string"},
        {"example", "OK"}
    };
    
    OPENAPI_GET_SIMPLE("/healthz", 
        "Health check endpoint",
        "Returns the health status of the greeter service",
        {"Health"},
        health_response,
        "healthCheck");
    
    server.Get("/healthz", [](const httplib::Request&, httplib::Response& res) {
        spdlog::debug("Health check requested");
        res.set_content("OK", "text/plain");
    });
    
    // Greetings endpoint
    nlohmann::json greetings_response = {
        {"type", "object"},
        {"properties", {
            {"message", {
                {"type", "string"},
                {"example", "Hello from Greeter service!"}
            }},
            {"service", {
                {"type", "string"},
                {"example", "greeter"}
            }},
            {"version", {
                {"type", "string"},
                {"example", "1.0.0"}
            }}
        }},
        {"required", {"message", "service", "version"}}
    };
    
    OPENAPI_GET_SIMPLE("/greetings",
        "Get general greeting",
        "Returns a general greeting message from the greeter service",
        {"Greetings"},
        greetings_response,
        "getGeneralGreeting");
    
    server.Get("/greetings", [](const httplib::Request&, httplib::Response& res) {
        spdlog::info("Greetings endpoint called");
        nlohmann::json response = {
            {"message", "Hello from Greeter service!"},
            {"service", "greeter"},
            {"version", "1.0.0"}
        };
        res.set_content(response.dump(), "application/json");
    });
    
    // Get greeting by ID
    nlohmann::json personalized_response = {
        {"type", "object"},
        {"properties", {
            {"id", {
                {"type", "string"},
                {"example", "World"}
            }},
            {"message", {
                {"type", "string"},
                {"example", "Hello, World!"}
            }},
            {"service", {
                {"type", "string"},
                {"example", "greeter"}
            }}
        }},
        {"required", {"id", "message", "service"}}
    };
    
    OPENAPI_GET_SIMPLE("/greetings/{id}",
        "Get personalized greeting",
        "Returns a personalized greeting message for a specific user or entity",
        {"Greetings"},
        personalized_response,
        "getPersonalizedGreeting");
    
    server.Get("/greetings/(.*)", [](const httplib::Request& req, httplib::Response& res) {
        std::string id = req.matches[1];
        spdlog::info("Greeting requested for ID: {}", id);
        nlohmann::json response = {
            {"id", id},
            {"message", "Hello, " + id + "!"},
            {"service", "greeter"}
        };
        res.set_content(response.dump(), "application/json");
    });
    
    spdlog::info("Greeter service starting on port 8081");
    server.listen("0.0.0.0", 8081);
    
    return 0;
}
