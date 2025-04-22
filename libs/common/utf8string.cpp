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

Utf8String::Utf8String(std::string_view sv) {
    std::string s(sv); // TODO: without std::string
    Init(s.data());
}

std::string Utf8String::to_string() const {
    std::string result;
    for (const Rune& r : runes) {
        for (uint8_t byte = 0; byte < r.bytes; ++byte) {
            result.append(1, r.data[byte]);
        }
    }
    return result;
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

void Utf8String::insert(size_t pos, Rune rune) {
    runes.insert(runes.begin() + pos, rune);
}

void Utf8String::erase(size_t pos) {
    runes.erase(runes.begin() + pos);
}

void Utf8String::push_back(Rune rune) {
    runes.push_back(rune);
}

void Utf8String::pop_back() {
    runes.pop_back();
}

bool Utf8String::contains(const Utf8String& substr) {
    for (size_t offset = 0; offset + substr.size() <= size(); ++offset) {
        bool isOK = true;
        for (size_t i = 0; i < substr.size(); ++i) {
            if (runes[offset + i] != substr.runes[i]) {
                isOK = false;
                break;
            }
        }
        if (isOK) {
            return true;
        }
    }
    return false;
}
