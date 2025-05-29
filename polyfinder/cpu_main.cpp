#include "cpu_main.h";

void cpu_main_multi_poly(unsigned int m_start, unsigned int m_end, unsigned int t_start, unsigned int t_end) {
    for (unsigned int m = m_start; m <= m_end; ++m) {
        for (unsigned int t = t_start; t <= t_end; ++t) {
            polyfinder my(m, t);
            long long result = my.find_cyclic_polynom();
            printf("%llu ", result); printf(" poly\n");

        }
    }
}
void cpu_main_single_poly(unsigned int m, unsigned int t) {
    polyfinder my(m, t);
}