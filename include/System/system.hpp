#ifndef BOOKSTORE_2023_SYSTEM_HPP
#define BOOKSTORE_2023_SYSTEM_HPP

#include "../User/user.hpp"
#include "../Book/book.hpp"

struct Finance {
    double income;
    double expenditure;

    Finance() = default;

    Finance(const double &income, const double &expenditure) :income(income), expenditure(expenditure) {}
};

struct EmployeeWork {
    int time;
    ConstLenStr<> work_record;

    EmployeeWork() = default;

    EmployeeWork(int time, ConstLenStr<> &work_record) : time(time), work_record(work_record) {}
};

int CmpEmployeeWork(const EmployeeWork &, const EmployeeWork &);

class System {
private:
    int time;
    memory<ConstLenStr<31>, UserInfo, CmpStr, CmpUserInfo> user_memory;
    memory<ConstLenStr<21>, ISBNIndexingInfo, CmpStr, ISBNIndexingCmp> ISBN_memory;
    memory<ConstLenStr<61>, BookNameIndexingInfo, CmpStr, BookNameIndexingCmp> book_name_memory;
    memory<ConstLenStr<61>, AuthorNameIndexingInfo, CmpStr, AuthorNameIndexingCmp> author_name_memory;
    memory<ConstLenStr<61>, KeywordIndexingInfo, CmpStr, KeywordIndexingCmp> keyword_memory;
    fstream finance_memory;
    fstream finance_report_memory;
    memory<ConstLenStr<31>, EmployeeWork, CmpStr, CmpEmployeeWork> employee_work_memory;
    fstream log_memory;

public:
    friend class User;

    System() = default;

    ~System() = default;

    System(const string &user_index_name, const string &user_element_name,
           const string &ISBN_index_name, const string &ISBN_element_name,
           const string &book_name_index_name, const string &book_name_element_name,
           const string &author_name_index_name, const string &author_name_element_name,
           const string &keyword_index_name, const string &keyword_element_name,
           const string &finance_name, const string &finance_report_name,
           const string &employee_work_index_name, const string &employee_work_element_name,
           const string &log_name)
            : user_memory(user_index_name, user_element_name),
              ISBN_memory(ISBN_index_name, ISBN_element_name),
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
