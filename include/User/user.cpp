#include "user.hpp"

int CmpUserInfo(const UserInfo &lhs, const UserInfo &rhs) {
    int flag_password = CmpStr(lhs.password, rhs.password);
    if (flag_password < 0) return -1;
    if (flag_password > 0) return 1;
    int flag_username = CmpStr(lhs.username, rhs.username);
    if (flag_username < 0) return -1;
    if (flag_username > 0) return 1;
    return 0;
}

int CmpEmployeeWork(const EmployeeWork &lhs, const EmployeeWork &rhs) {
    if (lhs.time < rhs.time) return -1;
    if (lhs.time > rhs.time) return 1;
    return 0;
}