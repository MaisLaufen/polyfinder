#pragma once

#include <httplib.h>
#include "polyfinder.h"
#include <nlohmann/json.hpp>

void register_routes(httplib::Server& svr);