#ifndef BOOKSTORE_2023_SYSTEM_HPP
#define BOOKSTORE_2023_SYSTEM_HPP

#include "../Book/book.hpp"
#include "../User/user.hpp"
#include "../Memory/memory.h"
#include "../InstructionScanner/instructionScanner.hpp"

class System {
private:
    bool if_exit = false;
    InstructionScanner scanner;
    int time = 0;
    int finance_count = 0;
    std::vector<Pair<User, Book>> user_list; // user和select绑定
    memory<ConstLenStr<31>, UserInfo, CmpStr, CmpUserInfo> user_memory;
    memory<ConstLenStr<21>, ISBNIndexingInfo, CmpStr, ISBNIndexingCmp> isbn_memory;
    memory<ConstLenStr<61>, ConstLenStr<21>, CmpStr, CmpStr> book_name_memory;
    memory<ConstLenStr<61>, ConstLenStr<21>, CmpStr, CmpStr> author_name_memory;
    memory<ConstLenStr<61>, ConstLenStr<21>, CmpStr, CmpStr> keyword_memory;
    memoryRiver<Finance, 1> finance_memory; // 文件头存储记录条数
    memoryRiver<Pair<ConstLenStr<31>, FinanceRecord>, 0> finance_report_memory;
    memory<ConstLenStr<31>, LogRecord, CmpStr, CmpLogRecord> employee_work_memory; // 以用户ID为索引
    memoryRiver<Pair<ConstLenStr<31>, LogRecord>, 1> log_memory;

public:
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
              employee_work_memory(employee_work_index_name, employee_work_element_name),
              finance_memory(finance_name), finance_report_memory(finance_report_name),
              log_memory(log_name) {
        if (!finance_memory.file_exist()) {
            finance_memory.initialise();
        }
        if (!finance_report_memory.file_exist()) {
            finance_report_memory.initialise();
        }
        if (!log_memory.file_exist()) {
            log_memory.initialise();
        }
        finance_memory.get_info(finance_count, 1);
        log_memory.get_info(time, 1);
        finance_memory.get_info(finance_count, 1);
    }

    void Initialise();

    void Su();

    void Register();

    void Logout();

    void Passwd();

    void Useradd();

    void Delete();

    void Show();

    void Buy();

    void Select();

    void Modify();

    void Import();

    void ShowFinance();

    void Log();

    void ReportFinance();

    void ReportEmployee();

    void GetInstruction();

    void Execute();

    bool Exit();
};

#endif //BOOKSTORE_2023_SYSTEM_HPP
