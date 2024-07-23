#ifndef EMPLOYEE_MANAGE_SYSTEM_H
#define EMPLOYEE_MANAGE_SYSTEM_H

#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>

using namespace std;

class Employee {
private:
    int empId;
    string name, designation;
    double salary;

public:
    Employee(int empId, const string& name, const string& designation, double salary)
        : empId(empId), name(name), designation(designation), salary(salary) {}

    int getID() const { return empId; }
    string getName() const { return name; }
    string getDesignation() const { return designation; }
    double getSalary() const { return salary; }

    void setName(const string& n) { name = n; }
    void setDesignation(const string& des) { designation = des; }
    void setSalary(double sal) { salary = sal; }

    string toJson() const {
        stringstream ss;
        ss << "{"
           << "\"id\": " << empId << ", "
           << "\"name\": \"" << name << "\", "
           << "\"designation\": \"" << designation << "\", "
           << "\"salary\": " << salary
           << "}";
        return ss.str();
    }
};

class EmployeeManageSystem {
private:
    vector<Employee> employees;
    int nextId = 1;

public:
    void addEmployee(const string& name, const string& designation, double salary) {
        Employee newEmployee(nextId++, name, designation, salary);
        employees.push_back(newEmployee);
        cout << "Employee added successfully\n\n";
    }

    bool updateEmployee(int empId, const string& name, const string& designation, double salary) {
        for (auto& emp : employees) {
            if (emp.getID() == empId) {
                emp.setName(name);
                emp.setDesignation(designation);
                emp.setSalary(salary);
                cout << "Employee updated successfully";
                return true;
            }
        }
        return false;
    }

    bool deleteEmployee(int empId) {
        for (auto it = employees.begin(); it != employees.end(); ++it) {
            if (it->getID() == empId) {
                employees.erase(it);
                cout << "Employee deleted successfully";
                return true;
            }
        }
        cout << "Employee not found";
        return false;
    }

    string getEmployees() const {
        stringstream ss;
        ss << "[";
        for (size_t i = 0; i < employees.size(); ++i) {
            if (i > 0) ss << ", ";
            ss << employees[i].toJson();
        }
        ss << "]";
        return ss.str();
    }

    string getEmployee(int empId) const {
        for (const auto& emp : employees) {
            if (emp.getID() == empId) {
                return emp.toJson();
            }
        }
        return "{}";
    }
};

#endif // EMPLOYEE_MANAGE_SYSTEM_H
