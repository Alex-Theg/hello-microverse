#include <httplib.h>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

int main() {
    spdlog::info("Starting Identity service...");
    
    httplib::Server server;
    
    // Health check endpoint
    server.Get("/healthz", [](const httplib::Request&, httplib::Response& res) {
        spdlog::debug("Health check requested");
        res.set_content("OK", "text/plain");
    });
    
    // User authentication endpoint
    server.Post("/auth/login", [](const httplib::Request& /*req*/, httplib::Response& res) {
        spdlog::info("Login attempt");
        nlohmann::json response = {
            {"message", "Login endpoint - not implemented yet"},
            {"service", "identity"},
            {"version", "1.0.0"}
        };
        res.set_content(response.dump(), "application/json");
    });
    
    // User registration endpoint
    server.Post("/auth/register", [](const httplib::Request& /*req*/, httplib::Response& res) {
        spdlog::info("Registration attempt");
        nlohmann::json response = {
            {"message", "Registration endpoint - not implemented yet"},
            {"service", "identity"},
            {"version", "1.0.0"}
        };
        res.set_content(response.dump(), "application/json");
    });
    
    // Get user info
    server.Get("/users/(.*)", [](const httplib::Request& req, httplib::Response& res) {
        std::string userId = req.matches[1];
        spdlog::info("User info requested for ID: {}", userId);
        nlohmann::json response = {
            {"id", userId},
            {"message", "User info endpoint - not implemented yet"},
            {"service", "identity"}
        };
        res.set_content(response.dump(), "application/json");
    });
    
    spdlog::info("Identity service starting on port 8082");
    server.listen("0.0.0.0", 8082);
    
    return 0;
}
