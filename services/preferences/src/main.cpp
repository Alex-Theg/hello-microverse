#include <httplib.h>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

int main() {
    spdlog::info("Starting Preferences service...");
    
    httplib::Server server;
    
    // Health check endpoint
    server.Get("/healthz", [](const httplib::Request&, httplib::Response& res) {
        spdlog::debug("Health check requested");
        res.set_content("OK", "text/plain");
    });
    
    // Get user preferences
    server.Get("/preferences/(.*)", [](const httplib::Request& req, httplib::Response& res) {
        std::string userId = req.matches[1];
        spdlog::info("Preferences requested for user: {}", userId);
        nlohmann::json response = {
            {"userId", userId},
            {"preferences", {
                {"language", "en"},
                {"timezone", "UTC"},
                {"notifications", true}
            }},
            {"service", "preferences"},
            {"version", "1.0.0"}
        };
        res.set_content(response.dump(), "application/json");
    });
    
    // Update user preferences
    server.Put("/preferences/(.*)", [](const httplib::Request& req, httplib::Response& res) {
        std::string userId = req.matches[1];
        spdlog::info("Preferences update requested for user: {}", userId);
        nlohmann::json response = {
            {"userId", userId},
            {"message", "Preferences updated successfully"},
            {"service", "preferences"}
        };
        res.set_content(response.dump(), "application/json");
    });
    
    // Get all preferences
    server.Get("/preferences", [](const httplib::Request&, httplib::Response& res) {
        spdlog::info("All preferences requested");
        nlohmann::json response = {
            {"message", "Preferences service is running"},
            {"service", "preferences"},
            {"version", "1.0.0"}
        };
        res.set_content(response.dump(), "application/json");
    });
    
    spdlog::info("Preferences service starting on port 8083");
    server.listen("0.0.0.0", 8083);
    
    return 0;
}
