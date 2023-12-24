#include "include/System/system.hpp"
#include <iomanip>

int main() {
    std::cout << std::fixed << std::setprecision(2);
    System system;
    while (!system.Exit()) {
        system.GetInstruction();
        system.Execute();
    }
}