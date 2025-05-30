#pragma once

#include <httplib.h>
#include "polyfinder.h"
#include <nlohmann/json.hpp>
#include "cpu_main.h"

void register_routes(httplib::Server& svr);