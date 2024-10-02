#include "TerminalWindow.h"

TerminalWindowPtr CreateBackgroundWindow(short rows, short cols) {
	return TerminalWindow::Create({ .row = 0, .col = 0 }, {.height = rows, .width = cols});
}

TerminalWindowPtr CreateTerminalWindow(short rowBeg, short colBeg, short rows, short cols) {
	return TerminalWindow::Create({ .row = rowBeg, .col = colBeg }, {.height = rows, .width = cols});
}

TerminalWindowPtr TerminalWindow::Create(TerminalCoord lu, TerminalSize size) {
	return std::make_shared<TerminalWindow>(lu, size);
}

void TerminalWindow::SetBackground(const TerminalCell& cell) {
	BackgroundCell = CreateCell(cell);
}

void TerminalWindow::SetBorderColor(const FontColor& borderColor) {
	BorderColor = borderColor;
}

void TerminalWindow::SetName(const std::string& newName) {
	name = newName;
}

void TerminalWindow::FlushUpBorder() {
	short row = 0;
	data[row][0] = CreateCell("╭", BorderColor);
	for (short col = 1; col < Width() - 1; ++col) {
		data[row][col] = CreateCell("─", BorderColor);
	}
	data[row][Width() - 1] = CreateCell("╮", BorderColor);
}

void TerminalWindow::FlushBottomBorder() {
	short row = Height() - 1;
	data[row][0] = CreateCell("╰", BorderColor);
	for (short col = 1; col < Width() - 1; ++col) {
		data[row][col] = CreateCell("─", BorderColor);
	}
	data[row][Width() - 1] = CreateCell("╯", BorderColor);
}

void TerminalWindow::FlushSelf() {
	FlushUpBorder();
	for (short row = 1; row < Height() - 1; ++row) {
		data[row][0] = CreateCell("│", BorderColor);
		for (short col = 1; col < Width() - 1; ++col) {
			data[row][col] = BackgroundCell;
		}
		data[row][Width() - 1] = CreateCell("│", BorderColor);
	}
	FlushBottomBorder();
}