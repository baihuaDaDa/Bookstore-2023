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
    return 0;
}

std::ostream &operator<<(std::ostream &out, const ISBNIndexingInfo &info) {
    out << info.book_name << '\t' << info.author_name << '\t' << info.keyword;
    return out;
}

std::ostream &operator<<(std::ostream &out, const BookNameIndexingInfo &info) {
    out << info.isbn << '\t' << info.author_name << '\t' << info.keyword << '\t';
    return out;
}

std::ostream &operator<<(std::ostream &out, const AuthorNameIndexingInfo &info) {
    out << info.isbn << '\t' << info.book_name << '\t' << info.keyword << '\t';
    return out;
}

std::ostream &operator<<(std::ostream &out, const KeywordIndexingInfo &info) {
    out << info.isbn << '\t' << info.book_name << '\t' << info.author_name << '\t';
    return out;
}

template<class INDEXING>
int NonISBNIndexingCmp(const INDEXING &lhs, const INDEXING &rhs) {
    int flag_isbn = CmpStr(lhs.isbn, rhs.isbn);
    if (flag_isbn < 0) return -1;
    if (flag_isbn > 0) return 1;
    return 0;
}

bool Book::operator==(const Book &book) {
    return this->isbn == book.isbn;
}
