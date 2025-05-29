#include "server.h"

void run_server() {
    httplib::Server svr;

    register_routes(svr);

    std::cout << "Server is running on http://localhost:8080\n";
    svr.listen("0.0.0.0", 8080);
}