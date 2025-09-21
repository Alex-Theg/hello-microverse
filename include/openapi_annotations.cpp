#include "openapi_annotations.h"
#include <fstream>
#include <spdlog/spdlog.h>

namespace openapi {

// Static member definitions
std::vector<EndpointInfo> OpenAPIRegistry::endpoints;
std::string OpenAPIRegistry::service_name;
std::string OpenAPIRegistry::service_version;
std::string OpenAPIRegistry::service_description;

nlohmann::json OpenAPIRegistry::generateOpenAPISpec() {
    nlohmann::json spec;
    
    // OpenAPI version and info
    spec["openapi"] = "3.0.3";
    spec["info"] = {
        {"title", service_name + " API"},
        {"description", service_description},
        {"version", service_version},
        {"contact", {
            {"name", "Hello Microverse Team"},
            {"email", "support@hellomicroverse.com"}
        }},
        {"license", {
            {"name", "MIT"},
            {"url", "https://opensource.org/licenses/MIT"}
        }}
    };
    
    // Servers
    spec["servers"] = {
        {
            {"url", "http://localhost:8080"},
            {"description", "Local development server"}
        }
    };
    
    // Paths
    spec["paths"] = nlohmann::json::object();
    
    for (const auto& endpoint : endpoints) {
        if (spec["paths"][endpoint.path].is_null()) {
            spec["paths"][endpoint.path] = nlohmann::json::object();
        }
        
        nlohmann::json operation;
        operation["summary"] = endpoint.summary;
        operation["description"] = endpoint.description;
        operation["operationId"] = endpoint.operation_id.empty() ? 
            endpoint.method + "_" + endpoint.path.substr(1) : endpoint.operation_id;
        
        if (!endpoint.tags.empty()) {
            operation["tags"] = endpoint.tags;
        }
        
        // Request body for POST/PUT
        if ((endpoint.method == "POST" || endpoint.method == "PUT") && 
            !endpoint.request_schema.is_null() && !endpoint.request_schema.empty()) {
            operation["requestBody"] = {
                {"required", true},
                {"content", {
                    {"application/json", {
                        {"schema", endpoint.request_schema}
                    }}
                }}
            };
        }
        
        // Response
        operation["responses"] = {
            {"200", {
                {"description", "Successful response"},
                {"content", {
                    {"application/json", {
                        {"schema", endpoint.response_schema}
                    }}
                }}
            }}
        };
        
        // Security
        if (endpoint.auth_required) {
            operation["security"] = {
                {{"bearerAuth", {}}}
            };
        }
        
        spec["paths"][endpoint.path][endpoint.method] = operation;
    }
    
    // Components
    spec["components"] = nlohmann::json::object();
    
    // Security schemes
    spec["components"]["securitySchemes"] = {
        {"bearerAuth", {
            {"type", "http"},
            {"scheme", "bearer"},
            {"bearerFormat", "JWT"}
        }}
    };
    
    // Common schemas
    spec["components"]["schemas"] = {
        {"Error", {
            {"type", "object"},
            {"properties", {
                {"error", {
                    {"type", "string"},
                    {"description", "Error message"}
                }},
                {"code", {
                    {"type", "integer"},
                    {"description", "Error code"}
                }},
                {"service", {
                    {"type", "string"},
                    {"description", "Service that generated the error"}
                }}
            }},
            {"required", {"error", "service"}}
        }}
    };
    
    return spec;
}

void OpenAPIRegistry::saveOpenAPISpec(const std::string& outputPath) {
    try {
        nlohmann::json spec = generateOpenAPISpec();
        
        std::ofstream file(outputPath);
        if (!file.is_open()) {
            spdlog::error("Failed to open file for writing: {}", outputPath);
            return;
        }
        
        // Convert to YAML-like format (simplified)
        file << "openapi: 3.0.3\n";
        file << "info:\n";
        file << "  title: " << spec["info"]["title"].get<std::string>() << "\n";
        file << "  description: |\n";
        file << "    " << spec["info"]["description"].get<std::string>() << "\n";
        file << "  version: " << spec["info"]["version"].get<std::string>() << "\n";
        file << "  contact:\n";
        file << "    name: " << spec["info"]["contact"]["name"].get<std::string>() << "\n";
        file << "    email: " << spec["info"]["contact"]["email"].get<std::string>() << "\n";
        file << "  license:\n";
        file << "    name: " << spec["info"]["license"]["name"].get<std::string>() << "\n";
        file << "    url: " << spec["info"]["license"]["url"].get<std::string>() << "\n";
        file << "\n";
        
        file << "servers:\n";
        for (const auto& server : spec["servers"]) {
            file << "  - url: " << server["url"].get<std::string>() << "\n";
            file << "    description: " << server["description"].get<std::string>() << "\n";
        }
        file << "\n";
        
        file << "paths:\n";
        for (auto& [path, pathItem] : spec["paths"].items()) {
            file << "  " << path << ":\n";
            for (auto& [method, operation] : pathItem.items()) {
                file << "    " << method << ":\n";
                file << "      summary: " << operation["summary"].get<std::string>() << "\n";
                file << "      description: " << operation["description"].get<std::string>() << "\n";
                file << "      operationId: " << operation["operationId"].get<std::string>() << "\n";
                
                if (operation.contains("tags")) {
                    file << "      tags:\n";
                    for (const auto& tag : operation["tags"]) {
                        file << "        - " << tag.get<std::string>() << "\n";
                    }
                }
                
                if (operation.contains("requestBody")) {
                    file << "      requestBody:\n";
                    file << "        required: true\n";
                    file << "        content:\n";
                    file << "          application/json:\n";
                    file << "            schema:\n";
                    file << "              type: object\n";
                }
                
                file << "      responses:\n";
                file << "        '200':\n";
                file << "          description: Successful response\n";
                file << "          content:\n";
                file << "            application/json:\n";
                file << "              schema:\n";
                file << "                type: object\n";
                
                if (operation.contains("security")) {
                    file << "      security:\n";
                    file << "        - bearerAuth: []\n";
                }
            }
        }
        
        file << "\ncomponents:\n";
        file << "  securitySchemes:\n";
        file << "    bearerAuth:\n";
        file << "      type: http\n";
        file << "      scheme: bearer\n";
        file << "      bearerFormat: JWT\n";
        
        file << "  schemas:\n";
        file << "    Error:\n";
        file << "      type: object\n";
        file << "      properties:\n";
        file << "        error:\n";
        file << "          type: string\n";
        file << "          description: Error message\n";
        file << "        code:\n";
        file << "          type: integer\n";
        file << "          description: Error code\n";
        file << "        service:\n";
        file << "          type: string\n";
        file << "          description: Service that generated the error\n";
        file << "      required:\n";
        file << "        - error\n";
        file << "        - service\n";
        
        file.close();
        spdlog::info("OpenAPI spec saved to: {}", outputPath);
        
    } catch (const std::exception& e) {
        spdlog::error("Error generating OpenAPI spec: {}", e.what());
    }
}

} // namespace openapi
