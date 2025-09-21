# Hello Microverse

Hello Microverse is a microservices-based demo application designed to simulate a **Greeting Generation Platform**.  
The goal of this project is to **practice designing, building, and deploying** a distributed system using modern architectural patterns.

---

## **Overview**

The system consists of multiple services that work together to generate personalized greetings for users:

TBD

---

## **Documentation**

For detailed specifications, workflows, and design decisions, see the **Confluence Project**:  
[📄 Hello Microverse – Confluence](https://alexanderstheg.atlassian.net/wiki/x/YAEGAg)

---

## **Project Management**

All epics, user stories, and tasks are tracked in **Jira**:  
[🎫 Hello Microverse – Jira Board](https://alexanderstheg.atlassian.net/jira/software/c/projects/HM/boards/67?atlOrigin=eyJpIjoiYjRkZmYxYTlhZGZiNDZiMDg0MjliMDUxYjZiNmUwOWEiLCJwIjoiaiJ9)

---

## **Getting Started**

### **Prerequisites**
- [CMake](https://cmake.org/) (version 3.16 or higher)
- [C++20 compatible compiler](https://en.cppreference.com/w/cpp/compiler_support) (GCC, Clang, or MSVC)
- [Docker](https://docs.docker.com/get-docker/) (for containerized deployment)
- [Docker Compose](https://docs.docker.com/compose/) (for multi-service deployment)

### **Local Development**

**Quick Start:**
```bash
# Build and run services using CMake targets
cmake -B build -S .
cmake --build build --target run          # Run API Gateway (default)
cmake --build build --target run-greeter  # Run Greeter service
cmake --build build --target run-identity # Run Identity service
cmake --build build --target run-preferences # Run Preferences service
cmake --build build --target run-audit    # Run Audit service
```

**Available CMake Targets:**
```bash
cmake --build build --target help-services  # Show all available targets
cmake --build build --target test-services  # Show health check endpoints
```

**Manual Execution:**
```bash
# After building, run executables directly
./build/gateway/gateway
./build/services/greeter/greeter
./build/services/identity/identity
./build/services/preferences/preferences
./build/services/audit/audit
```

**Service Endpoints:**
- Gateway: http://localhost:8080
- Greeter: http://localhost:8081
- Identity: http://localhost:8082
- Preferences: http://localhost:8083
- Audit: http://localhost:8084

**Test Commands:**
```bash
curl http://localhost:8081/greetings  # Test greeter service
curl http://localhost:8080/healthz    # Test gateway health
curl http://localhost:8081/healthz    # Test greeter health
```

---

## **Service Status**

TBD

---

## **Contributing**

1. Create a new branch from `dev`
2. Follow the commit message convention:
   `<Jira-ticket>-<short-description>`
   Example: `HM-123-orchestrator-add-idempotency`
3. Submit a PR for review before merging.

---

## **License**

This project is for **educational purposes only** and is not intended for production use.
