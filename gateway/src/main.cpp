#include <httplib.h>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

int main() {
    spdlog::info("Starting API Gateway...");
    
    httplib::Server server;
    
    // Health check endpoint
    server.Get("/healthz", [](const httplib::Request&, httplib::Response& res) {
        spdlog::debug("Health check requested");
        res.set_content("OK", "text/plain");
    });
    
    // Gateway status endpoint
    server.Get("/gateway/status", [](const httplib::Request&, httplib::Response& res) {
        spdlog::info("Gateway status requested");
        nlohmann::json response = {
            {"message", "API Gateway is running"},
            {"service", "gateway"},
            {"version", "1.0.0"},
            {"routes", {
                {"greeter", "http://localhost:8081"},
                {"identity", "http://localhost:8082"},
                {"preferences", "http://localhost:8083"},
                {"audit", "http://localhost:8084"}
            }}
        };
        res.set_content(response.dump(), "application/json");
    });
    
    // Proxy to greeter service
    server.Get("/api/greetings", [](const httplib::Request&, httplib::Response& res) {
        spdlog::info("Proxying to greeter service");
        nlohmann::json response = {
            {"message", "Gateway proxy to greeter - not implemented yet"},
            {"service", "gateway"},
            {"target", "greeter"}
        };
        res.set_content(response.dump(), "application/json");
    });
    
    // Proxy to identity service
    server.Get("/api/auth/(.*)", [](const httplib::Request& req, httplib::Response& res) {
        std::string endpoint = req.matches[1];
        spdlog::info("Proxying to identity service: {}", endpoint);
        nlohmann::json response = {
            {"message", "Gateway proxy to identity - not implemented yet"},
            {"service", "gateway"},
            {"target", "identity"},
            {"endpoint", endpoint}
        };
        res.set_content(response.dump(), "application/json");
    });
    
    spdlog::info("API Gateway starting on port 8080");
    server.listen("0.0.0.0", 8080);
    
    return 0;
}
