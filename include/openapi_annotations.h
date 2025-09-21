#pragma once
#include <string>
#include <vector>
#include <map>
#include <nlohmann/json.hpp>

namespace openapi {

struct EndpointInfo {
    std::string path;
    std::string method;
    std::string summary;
    std::string description;
    std::vector<std::string> tags;
    nlohmann::json request_schema;
    nlohmann::json response_schema;
    std::string operation_id;
    bool auth_required = false;
    
    EndpointInfo() = default;
    
    EndpointInfo(const std::string& p, const std::string& m, const std::string& s, 
                 const std::string& d, const std::vector<std::string>& t,
                 const nlohmann::json& req = nlohmann::json::object(),
                 const nlohmann::json& resp = nlohmann::json::object(),
                 const std::string& op_id = "",
                 bool auth = false)
        : path(p), method(m), summary(s), description(d), tags(t),
          request_schema(req), response_schema(resp), operation_id(op_id), auth_required(auth) {}
};

class OpenAPIRegistry {
private:
    static std::vector<EndpointInfo> endpoints;
    static std::string service_name;
    static std::string service_version;
    static std::string service_description;
    
public:
    static void setServiceInfo(const std::string& name, const std::string& version, const std::string& description) {
        service_name = name;
        service_version = version;
        service_description = description;
    }
    
    static void registerEndpoint(const EndpointInfo& endpoint) {
        endpoints.push_back(endpoint);
    }
    
    static const std::vector<EndpointInfo>& getEndpoints() {
        return endpoints;
    }
    
    static const std::string& getServiceName() { return service_name; }
    static const std::string& getServiceVersion() { return service_version; }
    static const std::string& getServiceDescription() { return service_description; }
    
    static void clear() {
        endpoints.clear();
        service_name.clear();
        service_version.clear();
        service_description.clear();
    }
    
    static nlohmann::json generateOpenAPISpec();
    static void saveOpenAPISpec(const std::string& outputPath);
};

// Convenience macros for easy annotation
#define OPENAPI_SERVICE(name, version, description) \
    openapi::OpenAPIRegistry::setServiceInfo(name, version, description)

#define OPENAPI_ENDPOINT(path, method, summary, description, tags, req_schema, resp_schema, op_id, auth) \
    do { \
        openapi::EndpointInfo _endpoint(path, method, summary, description, tags, req_schema, resp_schema, op_id, auth); \
        openapi::OpenAPIRegistry::registerEndpoint(_endpoint); \
    } while(0)

// Simplified macros that take pre-constructed JSON objects
#define OPENAPI_GET_SIMPLE(path, summary, description, tags, resp_schema, op_id) \
    OPENAPI_ENDPOINT(path, "GET", summary, description, tags, nlohmann::json::object(), resp_schema, op_id, false)

#define OPENAPI_POST_SIMPLE(path, summary, description, tags, req_schema, resp_schema, op_id, auth) \
    OPENAPI_ENDPOINT(path, "POST", summary, description, tags, req_schema, resp_schema, op_id, auth)

#define OPENAPI_PUT_SIMPLE(path, summary, description, tags, req_schema, resp_schema, op_id, auth) \
    OPENAPI_ENDPOINT(path, "PUT", summary, description, tags, req_schema, resp_schema, op_id, auth)

#define OPENAPI_DELETE_SIMPLE(path, summary, description, tags, resp_schema, op_id, auth) \
    OPENAPI_ENDPOINT(path, "DELETE", summary, description, tags, nlohmann::json::object(), resp_schema, op_id, auth)

} // namespace openapi
