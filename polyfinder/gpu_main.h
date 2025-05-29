#ifndef GPU_MAIN_H
#define GPU_MAIN_H
#include "polyfinder_gpu.h"

void gpu_main_multi_poly(unsigned int m_start, unsigned int m_end, unsigned int t_start, unsigned int t_end);
void gpu_main_single_poly(unsigned int m, unsigned int t);

#endif