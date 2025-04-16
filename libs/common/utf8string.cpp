#include "utf8string.h"

Utf8String Utf8String::Empty;

Utf8String::Utf8String(char c) {
    runes.push_back(c);
}

Utf8String::Utf8String(const char* s) {
    Init(s);
}

Utf8String::Utf8String(const char* s, short len) {
    Rune c(s);
    runes.resize(len, c);
}

Utf8String::Utf8String(const Rune& rune) {
    runes.resize(1, rune);
}

Utf8String::Utf8String(const std::string& s) {
    Init(s.data());
}

bool Utf8String::empty() const {
    return runes.empty();
}

size_t Utf8String::size() const {
    return runes.size();
}
const Rune& Utf8String::operator[] (size_t idx) const {
    return runes[idx];
}

Rune& Utf8String::operator[] (size_t idx) {
    return runes[idx];
}

Utf8String Utf8String::operator + (const Utf8String& rhs) {
    Utf8String result;
    result.runes.resize(size() + rhs.size());
    for (size_t i = 0; i < size(); ++i) {
        result.runes[i] = runes[i];
    }
    for (size_t i = size(); i < result.size(); ++i) {
        result.runes[i] = rhs[i - size()];
    }
    return result;
}

bool Utf8String::operator == (const Utf8String& rhs) const {
    return runes == rhs.runes;
}

void Utf8String::Init(const char* data) {
    const char* pos = data;
    while (*pos) {
        uint8_t len = utf8SymbolLen(*pos);
        runes.push_back(Rune(pos, len));
        pos += len;
    }
}

void Utf8String::resize(int size, Rune defaultRune) {
    runes.resize(size, defaultRune);
}

void Utf8String::push_back(Rune rune) {
    runes.push_back(rune);
}
