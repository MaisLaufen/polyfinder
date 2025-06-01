#pragma once

#ifndef GPU_MAIN_H
#define GPU_MAIN_H
#include "polyfinder_gpu.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

json gpu_main_multi_poly(unsigned int m_start, unsigned int m_end, unsigned int t_start, unsigned int t_end);
json gpu_main_single_poly(unsigned int m, unsigned int t);

#endif