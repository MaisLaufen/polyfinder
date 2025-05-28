#include "gpu_main.h";

void gpu_main(unsigned int m_start, unsigned int m_end, unsigned int t_start, unsigned int t_end) {
    printf("gpu_main start.\n");
    for (unsigned int i = m_start; i <= m_end; ++i) {
        for (unsigned int j = t_start; j <= t_end; ++j) {
            polyfinder_gpu my(i, j);
        }
    }
}