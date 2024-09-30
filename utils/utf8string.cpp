#include "utf8string.h"

uint8_t utf8SymbolLen(uint8_t firstSymbol) {
	if ((firstSymbol & 0b1000'0000) == 0b0000'0000) return 1;
	if ((firstSymbol & 0b1110'0000) == 0b1100'0000) return 2;
	if ((firstSymbol & 0b1111'0000) == 0b1110'0000) return 3;
	if ((firstSymbol & 0b1111'1000) == 0b1111'0000) return 4;
	return 0;
}