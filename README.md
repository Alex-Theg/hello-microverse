# Hello Microverse

Hello Microverse is a microservices-based demo application designed to simulate a **Greeting Generation Platform**.  
The goal of this project is to **practice designing, building, and deploying** a distributed system using modern architectural patterns.

---

## **Overview**

The system consists of multiple services that work together to generate personalized greetings for users:

- **Identity Service** → Manages user profiles and language preferences.
- **Template Service** → Manages greeting templates.
- **Greeting Store** → Stores generated greetings.
- **Greeter Orchestrator** → Coordinates the greeting generation workflow.
- **Translator Service (Stub)** → Simulates a flaky external API for translation.
- **Notifier Service** → Simulates notifications when greetings are generated.
- **Audit Service** → Logs system events.
- **API Gateway** → Single entry point for API requests and authentication.

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
- [Docker](https://docs.docker.com/get-docker/)
- [Docker Compose](https://docs.docker.com/compose/)
- [Make](https://www.gnu.org/software/make/)

### **Local Development**
```bash
# Start services and dependencies
make up

# Run database migrations
make migrate

# Seed initial data (optional)
make seed

# Run tests
make test
````

The API Gateway will be available at:
`http://localhost:8080`

---

## **Service Status**

| Service          | Port | Description                  |
| ---------------- | ---- | ---------------------------- |
| API Gateway      | 8080 | Entry point for API requests |
| Identity Service | 8081 | Manages user profiles        |
| Template Service | 8082 | Manages greeting templates   |
| Greeting Store   | 8083 | Stores generated greetings   |
| Orchestrator     | 8084 | Handles greeting requests    |
| Translator Stub  | 8085 | Simulates translations       |
| Notifier         | 8086 | Simulates notifications      |
| Audit            | 8087 | Logs system events           |

---

## **Contributing**

1. Create a new branch from `main`
2. Follow the commit message convention:
   `feature/<service-name>-<short-description>`
   Example: `feature/orchestrator-add-idempotency`
3. Submit a PR for review before merging.

---

## **License**

This project is for **educational purposes only** and is not intended for production use.
