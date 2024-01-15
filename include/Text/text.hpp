#ifndef TEXT_TEXT_HPP
#define TEXT_TEXT_HPP

#include <iostream>

class Text {
protected:
    std::string binary_str;
    size_t len;
    size_t index;

public:
    Text(const std::string &);

    virtual ~Text();

    size_t GetSize() const;

    void SetIndex(size_t);

    virtual bool ReadSingleChar(std::string &) = 0;

};

class UtfText : public Text {
public:
    UtfText(const std::string &);
    ~UtfText() override;
    bool ReadSingleChar(std::string &) override;

private:
    size_t GetUtfLen(const char);

};

class GbkText : public Text {
public:
    GbkText(const std::string &);
    ~GbkText() override;
    bool ReadSingleChar(std::string &) override;
};

#endif //TEXT_TEXT_HPP
