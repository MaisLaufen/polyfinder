#include "cpu_main.h"
#include "gpu_main.h"

int main() {
    unsigned int m_start = 1, m_end = 3, t_start = 1, t_end = 3;
    int choice = -1;

    std::cout << "Input m_start from 1 to 32: ";
    std::cin >> m_start;
    if (m_start < 1 || m_start > 32) {
        std::cerr << "Error: m_start out of bound.\n";
        return 1;
    }

    std::cout << "Input m_end from " << m_start << " to 32: ";
    std::cin >> m_end;
    if (m_end > 32 || m_end < m_start) {
        std::cerr << "Error: m_end out of bound.\n";
        return 1;
    }

    std::cout << "Input t_start from 1 to 32: ";
    std::cin >> t_start;
    if (t_start < 1 || t_start > 32) {
        std::cerr << "Error: t_start out of bound.\n";
        return 1;
    }

    std::cout << "Input t_end from " << t_start << " to 32: ";
    std::cin >> t_end;
    if (t_end > 32 || t_end < t_start) {
        std::cerr << "Error: t_end out of bound.\n";
        return 1;
    }

    std::cout << "Choose mode (0 for CPU, 1 for GPU, 2 for test): ";
    std::cin >> choice;

    if (choice == 0) {
        cpu_main(m_start, m_end, t_start, t_end);
    }
    if (choice == 1) {

        gpu_main(m_start, m_end, t_start, t_end);
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
    return 0;
}