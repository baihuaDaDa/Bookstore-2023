#ifndef BOOKSTORE_2023_SYSTEM_HPP
#define BOOKSTORE_2023_SYSTEM_HPP

#include "../Book/book.hpp"

struct UserInfo {
    ConstLenStr<31> password; // 合法字符集：数字， 字母， 下划线
    ConstLenStr<31> username; // 除不可见以外的ASCII字符

    UserInfo() = default;

    ~UserInfo() = default;

    UserInfo(const ConstLenStr<31> &_password, const ConstLenStr<31> &_username)
            : password(_password), username(_username) {}
};

int CmpUserInfo(const UserInfo &, const UserInfo &);

struct Finance {
    double income;
    double expenditure;

    Finance() = default;

    Finance(const double &income, const double &expenditure) :income(income), expenditure(expenditure) {}
};

struct EmployeeWork {
    int time;
    ConstLenStr<280> work_record;

    EmployeeWork() = default;

    EmployeeWork(int time, ConstLenStr<280> &work_record) : time(time), work_record(work_record) {}
};

int CmpEmployeeWork(const EmployeeWork &, const EmployeeWork &);

class System {
private:
    int time;
    memory<ConstLenStr<31>, UserInfo, CmpStr, CmpUserInfo> user_memory;
    memory<ConstLenStr<21>, ISBNIndexingInfo, CmpStr, ISBNIndexingCmp> isbn_memory;
    memory<ConstLenStr<61>, BookNameIndexingInfo, CmpStr, NonISBNIndexingCmp> book_name_memory;
    memory<ConstLenStr<61>, AuthorNameIndexingInfo, CmpStr, NonISBNIndexingCmp> author_name_memory;
    memory<ConstLenStr<61>, KeywordIndexingInfo, CmpStr, NonISBNIndexingCmp> keyword_memory;
    fstream finance_memory; // 文件头存储记录条数
    fstream finance_report_memory;
    memory<ConstLenStr<31>, EmployeeWork, CmpStr, CmpEmployeeWork> employee_work_memory; // 以用户ID为索引
    fstream log_memory;

public:
    friend class User;

    System() = default;

    ~System() = default;

    System(const string &user_index_name, const string &user_element_name,
           const string &isbn_index_name, const string &isbn_element_name,
           const string &book_name_index_name, const string &book_name_element_name,
           const string &author_name_index_name, const string &author_name_element_name,
           const string &keyword_index_name, const string &keyword_element_name,
           const string &finance_name, const string &finance_report_name,
           const string &employee_work_index_name, const string &employee_work_element_name,
           const string &log_name)
            : user_memory(user_index_name, user_element_name),
              isbn_memory(isbn_index_name, isbn_element_name),
              book_name_memory(book_name_index_name, book_name_element_name),
              author_name_memory(author_name_index_name, author_name_element_name),
              keyword_memory(keyword_index_name, keyword_element_name),
              employee_work_memory(employee_work_index_name, employee_work_element_name) {
        finance_memory.open(finance_name, std::ios::app | std::ios::binary);
        finance_memory.close();
        finance_report_memory.open(finance_report_name, std::ios::app | std::ios::binary);
        finance_report_memory.close();
        log_memory.open(log_name, std::ios::app | std::ios::binary);
        log_memory.close();
    }

    void initialise();

    void quit(); // quit or exit
};

#endif //BOOKSTORE_2023_SYSTEM_HPP
