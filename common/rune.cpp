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

const uint8_t* Rune::get() const {
    return data;
}

void Rune::clear() {
    memset(data, 0, sizeof(data));
}

void Rune::copy(const char* s, uint8_t len) {
    memcpy(data, s, len);
}

Rune& Rune::operator = (const Rune& r) {
    memcpy(data, r.data, sizeof(data));
    return *this;
}

bool Rune::operator == (const Rune& r) const {
    return memcmp(data, r.data, sizeof(data)) == 0;
}