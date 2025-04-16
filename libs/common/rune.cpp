#define _CRT_SECURE_NO_WARNINGS

#include "rune.h"
#include <cstring>
#include <algorithm>
#include <stdexcept>


uint8_t utf8SymbolLen(uint8_t firstSymbol) {
    if ((firstSymbol & 0b1000'0000) == 0b0000'0000) return 1;
    if ((firstSymbol & 0b1110'0000) == 0b1100'0000) return 2;
    if ((firstSymbol & 0b1111'0000) == 0b1110'0000) return 3;
    if ((firstSymbol & 0b1111'1000) == 0b1111'0000) return 4;
    return 0;
}

uint8_t bits_count(wchar_t code) {
    uint8_t bits = 0;
    while (code) {
        code >>= 1;
        ++bits;
    }
    return bits;
}

Rune::Rune() {
    clear();
    bytes = 0;
}

Rune::Rune(char c) {
    clear();
    data[0] = c;
    bytes = 1;
}

Rune::Rune(const char* s) {
    clear();
    bytes = utf8SymbolLen(s[0]);
    copy(s, bytes);
}

Rune::Rune(const char* s, uint8_t bytes) {
    clear();
    this->bytes = bytes;
    copy(s, bytes);
}

Rune::Rune(wchar_t wc) {
    clear();
    uint8_t bits = bits_count(wc);
    if (bits <= 7) {
        bytes = 1;
        data[0] = (uint8_t)wc;
    }
    else if (bits <= 11) {
        bytes = 2;
        // 110xxxxx 10xxxxxx
        data[1] = 0b1000'0000 | (0b0011'1111 & wc);
        wc >>= 6;
        data[0] = 0b1100'0000 | (0b0001'1111 & wc);
    }
    else {
        throw std::runtime_error("unimplement decode wchar_t to Rune");
    }
}

Rune::Rune(const Rune& r)
    : bytes(r.bytes)
{
    memcpy(data, r.data, sizeof(data));
}

void Rune::swap(Rune& r) {
    std::swap(bytes, r.bytes);
    std::swap(data, r.data);
}

const uint8_t* Rune::get() const {
    return data;
}

bool Rune::is_empty() const {
    return bytes == 0;
}

uint8_t Rune::size() const {
    return bytes;
}

void Rune::clear() {
    memset(data, 0, sizeof(data));
}

void Rune::copy(const char* s, uint8_t bytes) {
    memcpy(data, s, bytes);
}

Rune& Rune::operator = (Rune r) {
    this->swap(r);
    return *this;
}

bool Rune::operator == (const Rune& r) const {
    return memcmp(data, r.data, sizeof(data)) == 0;
}