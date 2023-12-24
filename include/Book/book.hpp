#ifndef BOOKSTORE_2023_BOOK_HPP
#define BOOKSTORE_2023_BOOK_HPP

#include "../constantLengthString.h"

// 结构体用于打包存储于四个不同的块状链表存储文件中
struct ISBNIndexingInfo {
    ConstLenStr<61> book_name;
    ConstLenStr<61> author_name;
    ConstLenStr<61> keyword;

    ISBNIndexingInfo() = default;

    ISBNIndexingInfo(const ConstLenStr<61> &book_name, const ConstLenStr<61> &author_name,
                     const ConstLenStr<61> &keyword)
            : book_name(book_name), author_name(author_name), keyword(keyword) {}
};

int ISBNIndexingCmp(const ISBNIndexingInfo &, const ISBNIndexingInfo &);

std::ostream &operator<<(std::ostream &out, const ISBNIndexingInfo &info);

struct BookNameIndexingInfo {
    ConstLenStr<21> isbn;
    ConstLenStr<61> author_name;
    ConstLenStr<61> keyword;

    BookNameIndexingInfo() = default;

    BookNameIndexingInfo(const ConstLenStr<21> &isbn, const ConstLenStr<61> &author_name,
                         const ConstLenStr<61> &keyword)
            : isbn(isbn), author_name(author_name), keyword(keyword) {}
};

std::ostream &operator<<(std::ostream &out, const BookNameIndexingInfo &info);

struct AuthorNameIndexingInfo {
    ConstLenStr<21> isbn;
    ConstLenStr<61> book_name;
    ConstLenStr<61> keyword;

    AuthorNameIndexingInfo() = default;

    AuthorNameIndexingInfo(const ConstLenStr<21> &isbn, const ConstLenStr<61> &book_name,
                           const ConstLenStr<61> &keyword)
            : isbn(isbn), book_name(book_name), keyword(keyword) {}
};

std::ostream &operator<<(std::ostream &out, const AuthorNameIndexingInfo &info);

struct KeywordIndexingInfo {
    ConstLenStr<21> isbn;
    ConstLenStr<61> book_name;
    ConstLenStr<61> author_name;

    KeywordIndexingInfo() = default;

    KeywordIndexingInfo(const ConstLenStr<21> &isbn, const ConstLenStr<61> &book_name,
                        const ConstLenStr<61> &author_name)
            : isbn(isbn), book_name(book_name), author_name(author_name) {}
};

std::ostream &operator<<(std::ostream &out, const KeywordIndexingInfo &info);

template<class INDEXING>
int NonISBNIndexingCmp(const INDEXING &, const INDEXING &);

struct TradeInfo {};

class Book {
private:
    ConstLenStr<21> isbn;
    ConstLenStr<61> book_name;
    ConstLenStr<61> author_name;
    ConstLenStr<61> keyword;
    double price = 0;
    int storage = 0;

public:
    Book() = default;

    ~Book() = default;

    Book(const ConstLenStr<21> &isbn) : isbn(isbn), book_name(), author_name(), keyword(), price(0), storage(0) {}

    Book(const ConstLenStr<21> &isbn, const ConstLenStr<61> &book_name,
         const ConstLenStr<61> &author_name, const ConstLenStr<61> &keyword,
         const double &price, int storage)
            : isbn(isbn), book_name(book_name), author_name(author_name), keyword(keyword), price(price), storage(storage) {}

    friend class System;

    bool operator==(const Book &);
};


#endif //BOOKSTORE_2023_BOOK_HPP
