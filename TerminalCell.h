#pragma once

#include "utils/utf8string.h"
#include "FontColor.h"
#include "BackgroundColor.h"

class TerminalCell {
public:
	TerminalCell() = default;
	explicit TerminalCell(const Rune& rune);
	TerminalCell(const Rune& rune, const FontColor& fColor);
	TerminalCell(const Rune& rune, const FontColor& fColor, const BackgroundColor& bColor);
	TerminalCell(const TerminalCell& other);

	const TerminalControl* GetParent() const;
	void SetParent(const TerminalControl* newParent);
	void Render();

private:
	Rune data = Rune(" ");
	FontColor fontColor = FontColor::Default;
	BackgroundColor backgroundColor = BackgroundColor::Default;

	const TerminalControl* parent;
};
