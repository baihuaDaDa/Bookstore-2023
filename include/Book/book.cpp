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
    if (lhs.storage < rhs.storage) return -1;
    if (lhs.storage > rhs.storage) return 1;
    return 0;
}

std::ostream &operator<<(std::ostream &out, const ISBNIndexingInfo &info) {
    out << info.book_name << '\t' << info.author_name << '\t' << info.keyword << '\t'
        << info.price << '\t' << info.storage;
    return out;
}

bool Book::operator==(const Book &book) {
    return this->isbn == book.isbn;
}
