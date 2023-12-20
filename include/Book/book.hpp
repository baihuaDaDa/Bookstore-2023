#ifndef BOOKSTORE_2023_BOOK_HPP
#define BOOKSTORE_2023_BOOK_HPP

#include "../constantLengthString.h"
#include "../Memory/memory.h"
#include "../User/user.hpp"

enum BookInfoType {
    ISBN,
    NAME,
    AUTHOR,
    KEYWORD,
    PRICE
};

// 派生类用于modify的时候便于用基类指针操作，且可以绑定信息类型和信息本身
class BookInfo {
public:
    BookInfoType info_type;

    BookInfo() = default;

    BookInfo(BookInfoType info_type) : info_type(info_type) {}
};

class ISBNNumber : public BookInfo {
public:
    ConstLenStr<21> ISBN; // 除不可见字符以外的ASCII字符

    ISBNNumber() = default;

    ISBNNumber(BookInfoType info_type, const ConstLenStr<21> &ISBN) : BookInfo(info_type), ISBN(ISBN) {}
};

class BookName : public BookInfo {
public:
    ConstLenStr<61> book_name; // 除不可见字符和英文双引号以外的ASCII字符

    BookName() = default;

    BookName(BookInfoType info_type, const ConstLenStr<61> &book_name) : BookInfo(info_type), book_name(book_name) {}
};

class AuthorName : public BookInfo {
public:
    ConstLenStr<61> author_name; // 除不可见字符和英文双引号以外的ASCII字符

    AuthorName() = default;

    AuthorName(BookInfoType info_type, const ConstLenStr<61> &author_name) : BookInfo(info_type),
                                                                             author_name(author_name) {}
};

class Keyword : public BookInfo {
public:
    ConstLenStr<61> keyword; // 除不可见字符和英文双引号以外的ASCII字符

    Keyword() = default;

    Keyword(BookInfoType info_type, const ConstLenStr<61> &keyword) : BookInfo(info_type), keyword(keyword) {}
};

class Price : public BookInfo {
public:
    double price; // 保留两位小数

    Price() = default;

    Price(BookInfoType info_type, const double &price) : BookInfo(info_type), price(price) {}
};

// 结构体用于打包存储于四个不同的块状链表存储文件中
struct ISBNIndexingInfo {
    ConstLenStr<61> book_name;
    ConstLenStr<61> author_name;
    ConstLenStr<61> keyword;
    double price;

    ISBNIndexingInfo() = default;

    ISBNIndexingInfo(const ConstLenStr<61> &book_name, const ConstLenStr<61> &author_name,
                     const ConstLenStr<61> &keyword, const double &price)
            : book_name(book_name), author_name(author_name), keyword(keyword), price(price) {}
};

int ISBNIndexingCmp(const ISBNIndexingInfo &, const ISBNIndexingInfo &);

struct BookNameIndexingInfo {
    ConstLenStr<21> ISBN;
    ConstLenStr<61> author_name;
    ConstLenStr<61> keyword;
    double price;

    BookNameIndexingInfo() = default;

    BookNameIndexingInfo(const ConstLenStr<21> &ISBN, const ConstLenStr<61> &author_name,
                         const ConstLenStr<61> &keyword, const double &price)
            : ISBN(ISBN), author_name(author_name), keyword(keyword), price(price) {}
};

int BookNameIndexingCmp(const BookNameIndexingInfo &, const BookNameIndexingInfo &);

struct AuthorNameIndexingInfo {
    ConstLenStr<21> ISBN;
    ConstLenStr<61> book_name;
    ConstLenStr<61> keyword;
    double price;

    AuthorNameIndexingInfo() = default;

    AuthorNameIndexingInfo(const ConstLenStr<21> &ISBN, const ConstLenStr<61> &book_name,
                           const ConstLenStr<61> &keyword, const double &price)
            : ISBN(ISBN), book_name(book_name), keyword(keyword), price(price) {}
};

int AuthorNameIndexingCmp(const AuthorNameIndexingInfo &, const AuthorNameIndexingInfo &);

struct KeywordIndexingInfo {
    ConstLenStr<21> ISBN;
    ConstLenStr<61> book_name;
    ConstLenStr<61> author_name;
    double price;

    KeywordIndexingInfo() = default;

    KeywordIndexingInfo(const ConstLenStr<21> &ISBN, const ConstLenStr<61> &book_name,
                        const ConstLenStr<61> &author_name, const double &price)
            : ISBN(ISBN), book_name(book_name), author_name(author_name), price(price) {}
};

int KeywordIndexingCmp(const KeywordIndexingInfo &, const KeywordIndexingInfo &);

class Book {
private:
    ConstLenStr<21> ISBN;
    ConstLenStr<61> book_name;
    ConstLenStr<61> author_name;
    ConstLenStr<61> keyword;
    double price;
    int storage;

public:
    Book() = default;

    ~Book() = default;

    Book(const ConstLenStr<21> &ISBN) : ISBN(ISBN), book_name(), author_name(), keyword(), price(0), storage(0) {}

    Book(const ConstLenStr<21> &ISBN, const ConstLenStr<61> &book_name,
         const ConstLenStr<61> &author_name, const ConstLenStr<61> &keyword,
         const double &price)
            : ISBN(ISBN), book_name(book_name), author_name(author_name), keyword(keyword), price(price), storage(0) {}

    friend class User;
};


#endif //BOOKSTORE_2023_BOOK_HPP
