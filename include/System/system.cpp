#include "system.hpp"

int CmpEmployeeWork(const EmployeeWork &lhs, const EmployeeWork &rhs) {
    if (lhs.time < rhs.time) return -1;
    if (lhs.time > rhs.time) return 1;
    return 0;
}