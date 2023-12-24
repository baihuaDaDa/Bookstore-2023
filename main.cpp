#include "include/System/system.hpp"
#include <iomanip>

int main() {
    freopen("/mnt/c/Users/zwg_w/Desktop/CLion/OJ Homework_big/5-Bookstore/Bookstore-2023/data/basic/testcase7/3.in", "r", stdin);
    freopen("/mnt/c/Users/zwg_w/Desktop/CLion/OJ Homework_big/5-Bookstore/Bookstore-2023/data/basic/testcase7/3.out", "w", stdout);
    std::cout << std::fixed << std::setprecision(2);
    System system("database/UserIndex.bin", "database/UserElement.bin",
                  "database/ISBNIndex.bin", "database/ISBNElement.bin",
                  "database/BookNameIndex.bin", "database/BookNameElement.bin",
                  "database/AuthorNameIndex.bin", "database/AuthorNameElement.bin",
                  "database/KeywordIndex.bin", "database/KeywordElement.bin",
                  "database/Finance.bin", "database/FinanceReport.bin",
                  "database/EmployeeWorkIndex.bin", "database/EmployeeWorkElement.bin",
                  "database/Log.bin");
    system.initialise();
//    InstructionScanner scanner;
//    scanner.IsDouble("49.8", PRICE);
//    std::cout << scanner.price;
    int line_num = 0;
    while (!system.Exit()) {
//        std::cout << ++line_num << ' ';
        system.GetInstruction();
        system.Execute();
    }
}