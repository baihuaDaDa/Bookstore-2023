#ifndef BOOKSTORE_2023_USER_HPP
#define BOOKSTORE_2023_USER_HPP

#include "../constantLengthString.h"

enum Privilege {
    VISITOR = 0,
    CUSTOMER = 1,
    EMPLOYEE = 3,
    SHOPKEEPER = 7
};

struct UserInfo {
    ConstLenStr<31> password; // 合法字符集：数字， 字母， 下划线
    ConstLenStr<31> username; // 除不可见以外的ASCII字符
    Privilege privilege = VISITOR;

    UserInfo() = default;

    ~UserInfo() = default;

    UserInfo(const ConstLenStr<31> &_password, const ConstLenStr<31> &_username, Privilege _privilege)
            : password(_password), username(_username), privilege(_privilege) {}
};

int CmpUserInfo(const UserInfo &, const UserInfo &);

struct Finance {
    double income = 0;
    double expenditure = 0;

    Finance() = default;

    Finance(const double &income, const double &expenditure) :income(income), expenditure(expenditure) {}
};

struct FinanceRecord {
    int time = 0;
    ConstLenStr<40> finance_record;

    FinanceRecord()  = default;

    FinanceRecord(int time, const ConstLenStr<40> &finance_record) : time(time), finance_record(finance_record) {}
};

struct EmployeeWork {
    int time = 0;
    ConstLenStr<280> work_record;

    EmployeeWork() = default;

    EmployeeWork(int time, ConstLenStr<280> &work_record) : time(time), work_record(work_record) {}
};

int CmpEmployeeWork(const EmployeeWork &, const EmployeeWork &);

struct LogRecord {
    int time = 0;
    ConstLenStr<280> log_record;

    LogRecord() = default;

    LogRecord(int time, const ConstLenStr<280> &log_record) : time(time), log_record(log_record) {}
};

class User {
private:
    ConstLenStr<31> ID; // 合法字符集：数字、字母、下划线（修改密码时的原密码和新密码也是如此）
    UserInfo user_info;

public:
    User() = default;

    User(const ConstLenStr<31> &ID, const UserInfo &user_info) : ID(ID), user_info(user_info) {}

    ~User() = default;

    friend class System;
};

#endif //BOOKSTORE_2023_USER_HPP
