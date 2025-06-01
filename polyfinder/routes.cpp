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

        res.set_chunked_content_provider(
            "application/json",
            [=](size_t offset, httplib::DataSink& sink) {
                sink.write("{\"results\":[", 13);

                bool first = true;

                for (int m = m_start; m <= m_end; ++m) {
                    for (int t = t_start; t <= t_end; ++t) {
                        json item;
                        if (mode == "gpu") {
                            item = gpu_main_single_poly(m, t);
                        }
                        else {
                            item = cpu_main_single_poly(m, t);
                        }
                        std::string chunk = (first ? "" : ",") + item[0].dump();
                        first = false;
                        sink.write(chunk.c_str(), chunk.size());
                    }
                }
                sink.write("]}", 2);
                sink.done();
                return true;
            }
        );
        });
}