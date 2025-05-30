#include "routes.h"

using json = nlohmann::json;

void register_routes(httplib::Server& svr) {
    svr.Post("/find_polynomials", [](const httplib::Request& req, httplib::Response& res) {
        json input = json::parse(req.body);
        int m_start = input["m_start"];
        int m_end = input["m_end"];
        int t_start = input["t_start"];
        int t_end = input["t_end"];
        std::string mode = input.value("mode", "cpu");

        json result_json;

        if (mode == "gpu") { // gpu mode
            result_json = cpu_main_multi_poly(m_start, m_end, t_start, t_end);
        }
        else { // cpu mode
            result_json = cpu_main_multi_poly(m_start, m_end, t_start, t_end);
        }

        res.set_content(json{ {"results", result_json} }.dump(), "application/json");
        });
}