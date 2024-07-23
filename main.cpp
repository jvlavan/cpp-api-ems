#include <pistache/endpoint.h>
#include "EmployeeManageSystem.h"
#include "EmployeeHandler.cpp"

using namespace Pistache;

int main() {
    EmployeeManageSystem ems;

    // Add some initial employees
    ems.addEmployee("Alice", "Developer", 70000);
    ems.addEmployee("Bob", "Manager", 80000);

    // Initialize the address
    auto address = Address("*:9082");

    // Create and start the server
    auto opts = Http::Endpoint::options().threads(1);
    Http::Endpoint server(address);
    server.init(opts);
    server.setHandler(Http::make_handler<EmployeeHandler>(ems));
    server.serve();

    return 0;
}
