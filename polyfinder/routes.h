#pragma once

#include <httplib.h>
#include <nlohmann/json.hpp>
#include "cpu_main.h"
#include "gpu_main.h"

using json = nlohmann::json;

void register_routes(httplib::Server& svr);