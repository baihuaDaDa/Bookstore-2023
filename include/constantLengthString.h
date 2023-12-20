#ifndef BOOKSTORE_2023_CONSTANTLENGTHSTRING_H
#define BOOKSTORE_2023_CONSTANTLENGTHSTRING_H

#include <exception>
#include <cstring>
#include <iostream>
#include <string>

class StringIndexingError : public std::exception {
public:
    const char *what() noexcept {
        return "String indexing error";
    }
};

template<int length>
class ConstLenStr {
private:
    char str[length] = {};

public:
    friend int CmpStr(const ConstLenStr<length> &lhs,
                      const ConstLenStr<length> &rhs);

    ConstLenStr() = default;

    ConstLenStr(const ConstLenStr<length> &str1) {
        std::strcpy(str, str1.str);
    }

    ConstLenStr(const char str1[length]) {
        std::strcpy(str, str1);
    }

    ~ConstLenStr() = default;

    const char *GetHead() const {
        return str;
    }

    ConstLenStr &operator=(const ConstLenStr<length> &rhs) {
        if (this == &rhs) {
            return *this;
        }
        std::strcpy(str, rhs.str);
        return *this;
    }

    char &operator[](int index) {
        if (index >= length) {
            throw StringIndexingError();
        }
        return str[index];
    }
};

template<int length>
int CmpStr(const ConstLenStr<length> &lhs,
           const ConstLenStr<length> &rhs) {
    int flag = std::strcmp(lhs.GetHead(), rhs.GetHead());
    if (flag < 0) return -1;
    if (flag > 0) return 1;
    return 0;
}

#endif //BOOKSTORE_2023_CONSTANTLENGTHSTRING_H
