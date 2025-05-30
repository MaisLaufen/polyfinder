#include "cpu_main.h"

json cpu_main_multi_poly(unsigned int m_start, unsigned int m_end, unsigned int t_start, unsigned int t_end) {
    json result_json = json::array();
    for (unsigned int m = m_start; m <= m_end; ++m) {
        for (unsigned int t = t_start; t <= t_end; ++t) {
            polyfinder pf(m, t);
            PolyfinderResult result = pf.find_cyclic_polynom();
            if (result.polynom != -1) {
                result_json.push_back({
                    {"m", m},
                    {"t", t},
                    {"polynom", result.polynom},
                    {"iterations", result.iterations},
                    {"time_seconds", result.time_seconds}
                    });
            }
            else {
                result_json.push_back({
                    {"m", m},
                    {"t", t},
                    {"polynom", nullptr},
                    {"iterations", result.iterations},
                    {"time_seconds", result.time_seconds},
                    {"error", "Cyclic polynomial not found"}
                    });
            }
        }
    }
    return result_json;
}

json cpu_main_single_poly(unsigned int m, unsigned int t) {
    json result_json = json::array();
    polyfinder pf(m, t);
    PolyfinderResult result = pf.find_cyclic_polynom();
    if (result.polynom != -1) {
        result_json.push_back({
            {"m", m},
            {"t", t},
            {"polynom", result.polynom},
            {"iterations", result.iterations},
            {"time_seconds", result.time_seconds}
            });
    }
    else {
        result_json.push_back({
            {"m", m},
            {"t", t},
            {"polynom", nullptr},
            {"iterations", result.iterations},
            {"time_seconds", result.time_seconds},
            {"error", "Cyclic polynomial not found"}
            });
    }
    return result_json;
}