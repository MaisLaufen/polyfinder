#include "gpu_main.h";

void gpu_main_multi_poly(unsigned int m_start, unsigned int m_end, unsigned int t_start, unsigned int t_end) {
    printf("gpu_main start.\n");
    for (unsigned int m = m_start; m <= m_end; ++m) {
        for (unsigned int t = t_start; t <= t_end; ++t) {
            polyfinder_gpu my(m, t);
        }
    }
}


void gpu_main_single_poly(unsigned int m, unsigned int t) {
    polyfinder_gpu my(m, t);
}