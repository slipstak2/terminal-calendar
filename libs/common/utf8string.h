#pragma once

#include "rune.h"
#include <vector>
#include <string>

class Utf8String {
public:
    static Utf8String Empty;
    Utf8String() = default;
    Utf8String(char c);
    Utf8String(const char* s);
    Utf8String(const char* s, short len);
    Utf8String(const Rune& rune);
    Utf8String(const std::string& s);

    bool empty() const;
    size_t size() const;
    void resize(int size, Rune defaultRune);
    void push_back(Rune rune);

    const Rune& operator[] (size_t idx) const;
    Rune& operator[] (size_t idx);

    Utf8String operator + (const Utf8String& rhs);
    bool operator == (const Utf8String& rhs) const;

protected:
    std::vector<Rune> runes;
private:
    void Init(const char* data);
};