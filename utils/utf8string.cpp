#include "utf8string.h"

uint8_t utf8SymbolLen(uint8_t firstSymbol) {
    if ((firstSymbol & 0b1000'0000) == 0b0000'0000) return 1;
    if ((firstSymbol & 0b1110'0000) == 0b1100'0000) return 2;
    if ((firstSymbol & 0b1111'0000) == 0b1110'0000) return 3;
    if ((firstSymbol & 0b1111'1000) == 0b1111'0000) return 4;
    return 0;
}

Rune::Rune(char c) {
    clear();
    data[0] = c;
}

Rune::Rune(const char* s) {
    clear();
    uint8_t len = utf8SymbolLen(s[0]);
    copy(s, len);
}

Rune::Rune(const char* s, uint8_t len) {
    clear();
    copy(s, len);
}

const uint8_t*  Rune::get() const{
    return data;
}

void Rune::clear() {
    memset(data, 0, sizeof(data));
}

void Rune::copy(const char* s, uint8_t len) {
    memcpy(data, s, len);
}


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

Utf8String::Utf8String(const std::string& s) {
    Init(s.data());
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

void Utf8String::Init(const char* data) {
    const char* pos = data;
    while (*pos) {
        uint8_t len = utf8SymbolLen(*pos);
        runes.push_back(Rune(pos, len));
        pos += len;
    }
}
