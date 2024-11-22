#include "rune.h"
#include <cstring>


uint8_t utf8SymbolLen(uint8_t firstSymbol) {
    if ((firstSymbol & 0b1000'0000) == 0b0000'0000) return 1;
    if ((firstSymbol & 0b1110'0000) == 0b1100'0000) return 2;
    if ((firstSymbol & 0b1111'0000) == 0b1110'0000) return 3;
    if ((firstSymbol & 0b1111'1000) == 0b1111'0000) return 4;
    return 0;
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

const uint8_t* Rune::get() const {
    return data;
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

Rune& Rune::operator = (const Rune& r) {
    memcpy(data, r.data, sizeof(data));
    return *this;
}

bool Rune::operator == (const Rune& r) const {
    return memcmp(data, r.data, sizeof(data)) == 0;
}