#pragma once
#include "common.h"

class Rune {
public:
	Rune(char c);
	Rune(const char* s);
	Rune(const char* s, uint8_t len);
	uint8_t* get();

protected:
	uint8_t data[4 + 1];

private:
	inline void clear();
	inline void copy(const char* s, uint8_t len);
};

class Utf8String {
public:
	Utf8String() = default;
	Utf8String(char c);
	Utf8String(const char* s);
	Utf8String(const std::string& s);
	size_t size() const;
	const Rune& operator[] (size_t idx) const;

protected:
	std::vector<Rune> runes;
private:
	void Init(const char* data);
};