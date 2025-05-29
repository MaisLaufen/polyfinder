#include "routes.h"

using json = nlohmann::json;

void register_routes(httplib::Server& svr) {
    svr.Post("/find_polynomials", [](const httplib::Request& req, httplib::Response& res) {
        json input = json::parse(req.body);
        int m_start = input["m_start"];
        int m_end = input["m_end"];
        int t_start = input["t_start"];
        int t_end = input["t_end"];
        std::string mode = input.value("mode", "cpu"); // "cpu" или "gpu"

        json result_json = json::array();

        for (int m = m_start; m <= m_end; ++m) {
            for (int t = t_start; t <= t_end; ++t) {
                polyfinder pf(m, t);
                long long result;
                if (mode == "gpu") {
                    printf("user choise = gpu.");
                    result = pf.find_cyclic_polynom();
                }
                else {
                    printf("user choice = cpu.");
                    result = pf.find_cyclic_polynom();
                }

                if (result != -1) {
                    result_json.push_back({ {"m", m}, {"t", t}, {"polynom", result} });
                }
            }
        }

        json response;
        response["results"] = result_json;
        res.set_content(response.dump(), "application/json");
        });
}