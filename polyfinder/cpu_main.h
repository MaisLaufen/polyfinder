#pragma once

#ifndef CPU_MAIN_H
#define CPU_MAIN_H
#include "polyfinder.h"
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

json cpu_main_multi_poly(unsigned int m_start, unsigned int m_end, unsigned int t_start, unsigned int t_end);
json cpu_main_single_poly(unsigned int m, unsigned int t);

#endif