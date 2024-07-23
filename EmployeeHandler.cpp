#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <string>
#include "EmployeeManageSystem.h"
#include <sstream>

using namespace Pistache;
using namespace std;

class EmployeeHandler : public Http::Handler {
public:
    HTTP_PROTOTYPE(EmployeeHandler)

    EmployeeHandler(EmployeeManageSystem& ems) : ems_(ems) {}

    void onRequest(const Http::Request& request, Http::ResponseWriter response) override {
        // Add CORS headers
        response.headers().add<Http::Header::AccessControlAllowOrigin>("*");
        response.headers().add<Http::Header::AccessControlAllowMethods>("GET, POST, PUT, DELETE, OPTIONS");
        response.headers().add<Http::Header::AccessControlAllowHeaders>("Content-Type");

        auto path = request.resource();
        auto method = request.method();

        // Handle preflight requests
        if (method == Http::Method::Options) {
            response.send(Http::Code::Ok);
            return;
        }

        if (method == Http::Method::Get) {
            if (path == "/employees") {
                handleGetEmployees(response);
            } else if (path.find("/employee/") == 0) {
                handleGetEmployee(path, response);
            }
        } else if (method == Http::Method::Post) {
            if (path == "/employee") {
                handleAddEmployee(request, response);
            }
        } else if (method == Http::Method::Put) {
            if (path.find("/employee/") == 0) {
                handleUpdateEmployee(path, request, response);
            }
        } else if (method == Http::Method::Delete) {
            if (path.find("/employee/") == 0) {
                handleDeleteEmployee(path, response);
            }
        } else {
            response.send(Http::Code::Not_Found, "Not Found");
        }
    }

private:
    EmployeeManageSystem& ems_;

    void handleGetEmployees(Http::ResponseWriter& response) {
        auto employees = ems_.getEmployees();
        response.send(Http::Code::Ok, employees, MIME(Text, Json));
    }

    void handleGetEmployee(const string& path, Http::ResponseWriter& response) {
        int empId = stoi(path.substr(10));  // Extract employee ID from URL
        auto employee = ems_.getEmployee(empId);
        if (!employee.empty()) {
            response.send(Http::Code::Ok, employee, MIME(Text, Json));
        } else {
            response.send(Http::Code::Not_Found, "Employee not found");
        }
    }

    void handleAddEmployee(const Http::Request& request, Http::ResponseWriter& response) {
        auto body = request.body();
        try {
            string name = getParam(body, "name");
            string designation = getParam(body, "designation");
            double salary = stod(getParam(body, "salary"));

            ems_.addEmployee(name, designation, salary);
            response.send(Http::Code::Created, "Employee added", MIME(Text, Plain));
        } catch (const std::exception& e) {
            response.send(Http::Code::Bad_Request, "Invalid input: " + string(e.what()));
        }
    }

    void handleUpdateEmployee(const string& path, const Http::Request& request, Http::ResponseWriter& response) {
        int empId = stoi(path.substr(10));  // Extract employee ID from URL
        auto body = request.body();
        try {
            string name = getParam(body, "name");
            string designation = getParam(body, "designation");
            double salary = stod(getParam(body, "salary"));

            if (ems_.updateEmployee(empId, name, designation, salary)) {
                response.send(Http::Code::Ok, "Employee updated", MIME(Text, Plain));
            } else {
                response.send(Http::Code::Not_Found, "Employee not found");
            }
        } catch (const std::exception& e) {
            response.send(Http::Code::Bad_Request, "Invalid input: " + string(e.what()));
        }
    }

    void handleDeleteEmployee(const string& path, Http::ResponseWriter& response) {
        int empId = stoi(path.substr(10));  // Extract employee ID from URL
        if (ems_.deleteEmployee(empId)) {
            response.send(Http::Code::Ok, "Employee deleted", MIME(Text, Plain));
        } else {
            response.send(Http::Code::Not_Found, "Employee not found");
        }
    }

    // Utility function to extract parameters from form-urlencoded body
    string getParam(const string& body, const string& key) {
        size_t keyPos = body.find(key + "=");
        if (keyPos == string::npos) {
            throw std::invalid_argument("Missing parameter: " + key);
        }
        size_t start = keyPos + key.length() + 1;
        size_t end = body.find("&", start);
        if (end == string::npos) {
            end = body.length();
        }
        return body.substr(start, end - start);
    }
};
