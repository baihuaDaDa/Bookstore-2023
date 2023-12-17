//
// Created by zwg_w on 2023/12/16.
//

#ifndef BOOKSTORE_2023_BOOK_HPP
#define BOOKSTORE_2023_BOOK_HPP

enum BookInfoType {
    ISBN,
    NAME,
    AUTHOR,
    KEYWORD
};

class Book {
private:
    char ISBN[21]; // 除不可见字符以外的ASCII字符
    char book_name[61]; // 除不可见字符和英文双引号以外的ASCII字符
    char keyword[61];
    double price;
    int storage;

public:
    Book();
};


#endif //BOOKSTORE_2023_BOOK_HPP
