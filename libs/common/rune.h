#pragma once

#include <cstdint>

class Rune {
public:
    Rune();
    Rune(char c);
    Rune(const char* s);
    Rune(const char* s, uint8_t bytes);
    const uint8_t* get() const;
    uint8_t size() const;

    Rune& operator = (const Rune& r);
    bool operator == (const Rune& r) const;

protected:
    uint8_t data[4 + 1];
    uint8_t bytes = 0;

private:
    inline void clear();
    inline void copy(const char* s, uint8_t bytes);
};

uint8_t utf8SymbolLen(uint8_t firstSymbol);
