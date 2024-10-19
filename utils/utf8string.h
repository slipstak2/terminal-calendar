#pragma once
#include "common.h"

class Rune {
public:
    Rune(char c);
    Rune(const char* s);
    Rune(const char* s, uint8_t len);
    const uint8_t* get() const;
    Rune& operator = (const Rune& r) {
        memcpy(data, r.data, sizeof(data));
        return *this;
    }
    bool operator == (const Rune& r) {
        return memcmp(data, r.data, sizeof(data)) == 0;
    }

protected:
    uint8_t data[4 + 1];

private:
    inline void clear();
    inline void copy(const char* s, uint8_t len);
};

class Utf8String {
public:
    static Utf8String Empty;
    Utf8String() = default;
    Utf8String(char c);
    Utf8String(const char* s);
    Utf8String(const char* s, short len);
    Utf8String(const std::string& s);
    size_t size() const;
    const Rune& operator[] (size_t idx) const;
    Rune& operator[] (size_t idx);

protected:
    std::vector<Rune> runes;
private:
    void Init(const char* data);
};