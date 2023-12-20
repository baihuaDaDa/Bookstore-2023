#ifndef BOOKSTORE_2023_USER_HPP
#define BOOKSTORE_2023_USER_HPP

#include <vector>
#include "../Book/book.hpp"

enum Privilege {
    VISITOR = 0,
    CUSTOMER = 1,
    EMPLOYEE = 3,
    SHOPKEEPER = 7
};

struct UserInfo {
    ConstLenStr<31> password; // 合法字符集：数字， 字母， 下划线
    ConstLenStr<31> username; // 除不可见以外的ASCII字符

    UserInfo() = default;

    ~UserInfo() = default;

    UserInfo(const ConstLenStr<31> &_password, const ConstLenStr<31> &_username)
            : password(_password), username(_username) {}
};

int CmpUserInfo(const UserInfo &, const UserInfo &);

class User {
private:

    static std::vector<User *> user_list;

    ConstLenStr<31> ID; // 合法字符集：数字、字母、下划线（修改密码时的原密码和新密码也是如此）
    UserInfo user_info;

public:
    User() = default;

    User(const ConstLenStr<31> &ID, const UserInfo &user_info) : ID(ID), user_info(user_info) {}

    virtual Privilege GetType();

    virtual void Log(ConstLenStr<31> &userID, ConstLenStr<31> &_password);

    virtual ~User() = default;

    void Register(ConstLenStr<31> &userID, ConstLenStr<31> &_password, ConstLenStr<31> &_username);
};

class Customer : public User {
public:
    Customer() = default;

    Customer(const ConstLenStr<31> &ID, const UserInfo &user_info) : User(ID, user_info) {}

    virtual Privilege GetType() override;

    virtual void Log(ConstLenStr<31> &userID, ConstLenStr<31> &_password) override;

    ~Customer() override = default;

    void Logout();

    void Passwd(ConstLenStr<31> &userID, ConstLenStr<31> &current_password, ConstLenStr<31> &New_password);

    void Show(ConstLenStr<61> &info, BookInfoType info_type);

    void Buy(ConstLenStr<21> &ISBN, int quantity);
};

class Employee : public Customer {
private:
    ConstLenStr<21> ISBN;

public:
    Employee() = default;

    Employee(const ConstLenStr<31> &ID, const UserInfo &user_info, const ConstLenStr<21> &ISBN)
            : Customer(ID, user_info), ISBN(ISBN) {}

    virtual Privilege GetType() override;

    virtual void Log(ConstLenStr<31> &userID, ConstLenStr<31> &_password) override;

    ~Employee() override = default;

    void Useradd(ConstLenStr<31> &userID, ConstLenStr<31> &_password, int privilege, ConstLenStr<31> &_username);

    void Select(ConstLenStr<21> &_ISBN);

    void Modify(ConstLenStr<61> &info, BookInfoType info_type);

    void Import(int quantity, double total_cost);
};

class Shopkeeper : public Employee {
public:
    Shopkeeper() = default;

    Shopkeeper(const ConstLenStr<31> &ID, const UserInfo &user_info, const ConstLenStr<21> &ISBN)
            : Employee(ID, user_info, ISBN) {}

    virtual Privilege GetType() override;

    virtual void Log(ConstLenStr<31> &userID, ConstLenStr<31> &_password) override;

    ~Shopkeeper() override = default;

    void Delete(ConstLenStr<31> &userID);

    void ShowFinance(int count = -1);
};

#endif //BOOKSTORE_2023_USER_HPP
