#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <regex>
#include <spdlog/spdlog.h>
#include "openapi_annotations.h"

class OpenAPIExtractor {
private:
    std::string service_name;
    std::string service_version;
    std::string service_description;
    
public:
    OpenAPIExtractor(const std::string& name) : service_name(name), service_version("1.0.0") {
        service_description = "Generated from C++ code annotations for " + name + " service";
    }
    
    void extractFromFile(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            spdlog::error("Could not open file: {}", filepath);
            return;
        }
        
        std::string content((std::istreambuf_iterator<char>(file)),
                           std::istreambuf_iterator<char>());
        file.close();
        
        // Extract service info
        extractServiceInfo(content);
        
        // Extract endpoints
        extractEndpoints(content);
    }
    
    // Getter methods
    const std::string& getServiceName() const { return service_name; }
    const std::string& getServiceVersion() const { return service_version; }
    const std::string& getServiceDescription() const { return service_description; }
    
private:
    void extractServiceInfo(const std::string& content) {
        // Look for OPENAPI_SERVICE macro
        std::regex service_regex("OPENAPI_SERVICE\\s*\\(\\s*\"([^\"]+)\"\\s*,\\s*\"([^\"]+)\"\\s*,\\s*\"([^\"]+)\"\\s*\\)");
        std::smatch match;
        
        if (std::regex_search(content, match, service_regex)) {
            service_name = match[1].str();
            service_version = match[2].str();
            service_description = match[3].str();
            spdlog::info("Found service info: {} v{}", service_name, service_version);
        }
    }
    
    void extractEndpoints(const std::string& content) {
        // Look for OPENAPI_GET_SIMPLE macro calls
        std::regex get_regex("OPENAPI_GET_SIMPLE\\s*\\(\\s*\"([^\"]+)\"\\s*,\\s*\"([^\"]+)\"\\s*,\\s*\"([^\"]+)\"\\s*,\\s*\\{([^}]+)\\}\\s*,\\s*([^,]+)\\s*,\\s*\"([^\"]+)\"\\s*\\)");
        std::sregex_iterator iter(content.begin(), content.end(), get_regex);
        std::sregex_iterator end;
        
        for (; iter != end; ++iter) {
            const std::smatch& match = *iter;
            std::string path = match[1].str();
            std::string summary = match[2].str();
            std::string description = match[3].str();
            std::string tags_str = match[4].str();
            std::string operation_id = match[6].str();
            
            // Parse tags
            std::vector<std::string> tags;
            std::regex tag_regex("\"([^\"]+)\"");
            std::sregex_iterator tag_iter(tags_str.begin(), tags_str.end(), tag_regex);
            for (; tag_iter != std::sregex_iterator(); ++tag_iter) {
                tags.push_back((*tag_iter)[1].str());
            }
            
            // Create response schema based on path
            nlohmann::json response_schema = createResponseSchema(path);
            
            // Register endpoint
            openapi::EndpointInfo endpoint(path, "GET", summary, description, tags, 
                                         nlohmann::json::object(), response_schema, operation_id, false);
            openapi::OpenAPIRegistry::registerEndpoint(endpoint);
            
            spdlog::info("Found GET endpoint: {} - {}", path, summary);
        }
    }
    
    nlohmann::json createResponseSchema(const std::string& path) {
        if (path == "/healthz") {
            return nlohmann::json{
                {"type", "string"},
                {"example", "OK"}
            };
        } else if (path == "/greetings") {
            return nlohmann::json{
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
        } else if (path.find("/greetings/{") != std::string::npos) {
            return nlohmann::json{
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
        }
        
        // Default response schema
        return nlohmann::json{
            {"type", "object"},
            {"properties", {
                {"message", {
                    {"type", "string"},
                    {"example", "Response from " + service_name}
                }},
                {"service", {
                    {"type", "string"},
                    {"example", service_name}
                }}
            }},
            {"required", {"message", "service"}}
        };
    }
};

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <service_name> <output_path> [source_file]" << std::endl;
        std::cerr << "Example: " << argv[0] << " greeter services/greeter/openapi/greeter.v1.yml services/greeter/src/main.cpp" << std::endl;
        return 1;
    }
    
    std::string service_name = argv[1];
    std::string output_path = argv[2];
    std::string source_file = argc > 3 ? argv[3] : "";
    
    spdlog::info("Generating OpenAPI spec for {} service...", service_name);
    
    // Clear any previous registrations
    openapi::OpenAPIRegistry::clear();
    
    // Create extractor and process source file if provided
    OpenAPIExtractor extractor(service_name);
    if (!source_file.empty()) {
        extractor.extractFromFile(source_file);
    }
    
    // Set service info
    openapi::OpenAPIRegistry::setServiceInfo(
        extractor.getServiceName(),
        extractor.getServiceVersion(),
        extractor.getServiceDescription()
    );
    
    spdlog::info("Service {} has {} registered endpoints", 
                 service_name, 
                 openapi::OpenAPIRegistry::getEndpoints().size());
    
    // Generate and save the spec
    openapi::OpenAPIRegistry::saveOpenAPISpec(output_path);
    
    spdlog::info("OpenAPI spec generated successfully: {}", output_path);
    
    return 0;
}
