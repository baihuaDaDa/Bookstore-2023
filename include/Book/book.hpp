#ifndef BOOKSTORE_2023_BOOK_HPP
#define BOOKSTORE_2023_BOOK_HPP

#include "../constantLengthString.h"

// 结构体用于打包存储于四个不同的块状链表存储文件中
struct ISBNIndexingInfo {
    ConstLenStr<61> book_name;
    ConstLenStr<61> author_name;
    ConstLenStr<61> keyword;
    double price = 0;
    int storage = 0;

    ISBNIndexingInfo() = default;

    ISBNIndexingInfo(const ConstLenStr<61> &book_name, const ConstLenStr<61> &author_name,
                     const ConstLenStr<61> &keyword, const double &price, int storage)
            : book_name(book_name), author_name(author_name), keyword(keyword), price(price), storage(storage) {}
};

int ISBNIndexingCmp(const ISBNIndexingInfo &, const ISBNIndexingInfo &);

std::ostream &operator<<(std::ostream &out, const ISBNIndexingInfo &info);

class Book {
private:
    ConstLenStr<21> isbn;
    ISBNIndexingInfo isbn_indexing_info;

public:
    Book() = default;

    ~Book() = default;

    Book(const ConstLenStr<21> &isbn) : isbn(isbn) {}

    Book(const ConstLenStr<21> &isbn, const ISBNIndexingInfo &isbn_indexing_info)
            : isbn(isbn), isbn_indexing_info(isbn_indexing_info) {}

    friend class System;

    bool operator==(const Book &);
};


#endif //BOOKSTORE_2023_BOOK_HPP
