#include "text.hpp"

Text::~Text() = default;

Text::Text(const std::string &str) : binary_str(str), index(0) {
    len = binary_str.size();
}

size_t Text::GetSize() const {
    return len;
}

void Text::SetIndex(size_t _index) {
    index = _index;
}

UtfText::UtfText(const std::string &str) : Text(str) {}

bool UtfText::ReadSingleChar(std::string &parsed_line) {
    if (index == len) return false;
    size_t _len = GetUtfLen(binary_str[index]);
    if (_len == 0 && _len + index > index) return false;
    parsed_line = binary_str.substr(index, _len);
    index += _len;
    return true;
}

size_t UtfText::GetUtfLen(const char head_byte) {
    unsigned char mask = 0x80;
    size_t _len = 0;
    while (head_byte & mask) {
        _len++;
        if (_len > 3) return 0;
        mask >>= 1;
    }
    if (_len == 0) _len = 1;
    return _len;
}

UtfText::~UtfText() = default;

