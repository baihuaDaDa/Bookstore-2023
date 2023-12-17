#ifndef BOOKSTORE_2023_USER_HPP
#define BOOKSTORE_2023_USER_HPP

#include <vector>
#include "../Memory/memory.h"
#include "../Book/book.hpp"

enum Privilege {
    VISITOR = 0,
    CUSTOMER = 1,
    EMPLOYEE = 3,
    SHOPKEEPER = 7
};

class User {
private:
    static std::vector<User *> user_list;
    char ID[31]; // 合法字符集：数字、字母、下划线（修改密码时的原密码和新密码也是如此）
    char password[31]; // 合法字符集：数字， 字母， 下划线
    char username[31]; // 除不可见以外的ASCII字符

public:
    virtual Privilege GetType();

    virtual void Log(char *userID, char *_password = "");

    virtual ~User() = default;

    void Register(char *userID, char *_password, char *_username);
};

class Customer : public User {
public:
    virtual Privilege GetType() override;

    virtual void Log(char *userID, char *_password = "") override;

    ~Customer();

    void Logout();

    void Passwd(char *userID, char *current_password, char *New_password);

    void Show(char *info,)
};

class Employee : public Customer {
public:
    virtual Privilege GetType() override;

    virtual void Log(char *userID, char *_password = "") override;

    ~Employee();

    void Useradd(char *userID, char *_password, int privilege, char *_username);
};

class Shopkeeper : public Employee {
public:
    Privilege GetType() override;

    void Log(char *userID, char *_password = "") override;

    ~Shopkeeper();

    void Delete(char *userID);
};

#endif //BOOKSTORE_2023_USER_HPP
