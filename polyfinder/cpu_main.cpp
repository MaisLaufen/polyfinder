#include "cpu_main.h";

void cpu_main(unsigned int m_start, unsigned int m_end, unsigned int t_start, unsigned int t_end) {
    for (unsigned int i = m_start; i <= m_end; ++i) {
        for (unsigned int j = t_start; j <= t_end; ++j) {
            polyfinder my(i, j);
        }
    }
}