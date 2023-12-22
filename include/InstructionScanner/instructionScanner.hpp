#ifndef BOOKSTORE_2023_INSTRUCTIONSCANNER_HPP
#define BOOKSTORE_2023_INSTRUCTIONSCANNER_HPP

#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include "../constantLengthString.h"

enum InfoType {
    ISBN,
    BOOK_NAME,
    AUTHOR_NAME,
    KEYWORD,
    PRICE,
    USER_ID,
    PASSWORD,
    USERNAME,
    NEW_PASSWORD,
    PRIVILEGE,
    SINGLE_KEYWORD,
    QUANTITY,
    TOTAL_COST,
    COUNT
};

enum InstrType {
    QUIT,
    EXIT,
    SU,
    LOGOUT,
    REGISTER,
    PASSWD,
    USERADD,
    DELETE,
    SHOW,
    BUY,
    SELECT,
    MODIFY,
    IMPORT,
    SHOW_FINANCE,
    LOG,
    REPORT_FINANCE,
    REPORT_EMPLOYEE,
    EMPTY
};

class instructionScanner {
private:
    std::string buffer;

public:
    // 可供调用的合法数据块
    InstrType instr_type;
    int factor_num = 0; // 参数个数
    ConstLenStr<31> ID;
    ConstLenStr<31> password;
    ConstLenStr<31> username;
    ConstLenStr<31> new_password;
    int privilege;
    ConstLenStr<21> isbn;
    ConstLenStr<61> book_name;
    ConstLenStr<61> author_name;
    ConstLenStr<61> keyword;
    ConstLenStr<61> single_keyword;
    std::vector<ConstLenStr<61>> keyword_list = {};
    InfoType show_what; // show指令查找的附加参数是什么类型
    bool modify_list[5] = {}; // modify修改了哪些元素
    int quantity;
    double price;
    double total_cost;
    int count;

    instructionScanner() = default;

    ~instructionScanner() = default;

    void SetBuffer(const std::string &_buffer);

    bool IsUerData(const std::string &token, InfoType info_type);

    bool IsASCII(const std::string &token, InfoType info_type); // 除不可见字符（和英文双引号）以外的ASCII字符

    bool IsInteger(const std::string &token, InfoType info_type);

    bool IsDouble(const std::string &token, InfoType info_type);

    bool IsKeyword(const std::string &token);

    bool Scan();

    bool ModifyScan(const std::string &token);
};


#endif //BOOKSTORE_2023_INSTRUCTIONSCANNER_HPP
