#include <httplib.h>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

int main() {
    spdlog::info("Starting Greeter service...");
    
    httplib::Server server;
    
    // Health check endpoint
    server.Get("/healthz", [](const httplib::Request&, httplib::Response& res) {
        spdlog::debug("Health check requested");
        res.set_content("OK", "text/plain");
    });
    
    // Greetings endpoint
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
