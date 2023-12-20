#include "book.hpp"

int ISBNIndexingCmp(const ISBNIndexingInfo &lhs, const ISBNIndexingInfo &rhs) {
    int flag_book_name = CmpStr(lhs.book_name, rhs.book_name);
    if (flag_book_name < 0) return -1;
    if (flag_book_name > 0) return 1;
    int flag_author_name = CmpStr(lhs.author_name, rhs.author_name);
    if (flag_author_name < 0) return -1;
    if (flag_author_name > 0) return 1;
    int flag_keyword = CmpStr(lhs.keyword, rhs.keyword);
    if (flag_keyword < 0) return -1;
    if (flag_keyword > 0) return 1;
    if (lhs.price < rhs.price) return -1;
    if (lhs.price > rhs.price) return 1;
    return 0;
}

int BookNameIndexingCmp(const BookNameIndexingInfo &lhs, const BookNameIndexingInfo &rhs) {
    int flag_ISBN = CmpStr(lhs.ISBN, rhs.ISBN);
    if (flag_ISBN < 0) return -1;
    if (flag_ISBN > 0) return 1;
    return 0;
}

int AuthorNameIndexingCmp(const AuthorNameIndexingInfo &lhs, const AuthorNameIndexingInfo &rhs) {
    int flag_ISBN = CmpStr(lhs.ISBN, rhs.ISBN);
    if (flag_ISBN < 0) return -1;
    if (flag_ISBN > 0) return 1;
    return 0;
}

int KeywordIndexingCmp(const KeywordIndexingInfo &lhs, const KeywordIndexingInfo &rhs) {
    int flag_ISBN = CmpStr(lhs.ISBN, rhs.ISBN);
    if (flag_ISBN < 0) return -1;
    if (flag_ISBN > 0) return 1;
    return 0;
}