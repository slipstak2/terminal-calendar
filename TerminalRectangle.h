#pragma once

struct TerminalCoord {
	short row = 0;
	short col = 0;
};

struct TerminalSize {
	short height = 0;
	short width = 0;
};

class TerminalRectangle {
public:
	TerminalRectangle(TerminalCoord lu, TerminalSize size)
		: lu(lu)
		, size(size)
	{}
	short RowBeg() const { return lu.row; }
	short RowEnd() const { return lu.row + size.height - 1; }
	short ColBeg() const { return lu.col; }
	short ColEnd() const { return lu.col + size.width - 1; }

	short Width() const {
		return size.width;
	}
	short Height() const {
		return size.height;
	}

protected:
	TerminalCoord lu;
	TerminalSize size;
};