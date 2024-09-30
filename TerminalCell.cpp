#include "TerminalCell.h"

void TerminalCell::SetParent(const TerminalControl* newParent) {
	parent = newParent;
}

const TerminalControl* TerminalCell::GetParent() const {
	return parent;
}

// https://habr.com/ru/articles/119436/
void TerminalCell::Render() {
	printf("\033[10;%d;%dm%s\033[0m", (int)backgroundColor, (int)fontColor, data.data);
}

TerminalCell::TerminalCell(const Rune& rune) 
	: data(rune) 
{}

TerminalCell::TerminalCell(const Rune& rune, const FontColor& fColor)
	: data(rune)
	, fontColor(fColor)
{}

TerminalCell::TerminalCell(const Rune& rune, const FontColor& fColor, const BackgroundColor& bColor)
	: data(rune)
	, fontColor(fColor)
	, backgroundColor(bColor)
{}

TerminalCell::TerminalCell(const TerminalCell& other)
	: data(other.data)
	, fontColor(other.fontColor)
	, backgroundColor(other.backgroundColor)
	, parent(other.parent)
{}