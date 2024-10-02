#pragma once
#include "common.h"

uint8_t utf8SymbolLen(uint8_t firstSymbol); // move to source

struct Rune {
	Rune(char c) {
		clear();
		data[0] = c;
	}
	Rune(const char* s) {
		clear();
		uint8_t len = utf8SymbolLen(s[0]);
		copy(s, len);
	}
	Rune(const char* s, uint8_t len) {
		clear();
		copy(s, len);
	}
	uint8_t data[4 + 1];

private:
	inline void clear()  {
		memset(data, 0, sizeof(data));
	}
	inline void copy(const char* s, uint8_t len) {
		memcpy(data, s, len);
	}
};

struct Utf8String {
	Utf8String(char c) {
		runes.push_back(c);
	}
	Utf8String(const char* s) {
		Init(s);
	}
	Utf8String(const std::string& s) {
		Init(s.data());
	}
	size_t size() const {
		return runes.size();
	}
	const Rune& operator[] (size_t idx) {
		return runes[idx];
	}
	std::vector<Rune> runes;
private:
	void Init(const char* data) {
		const char* pos = data;
		while (*pos) {
			uint8_t len = utf8SymbolLen(*pos);
			runes.push_back(Rune(pos, len));
			pos += len;
		}
	}
};