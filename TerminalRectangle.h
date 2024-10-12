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
    TerminalRectangle(TerminalCoord position)
        : position(position)
    {}

    TerminalRectangle(TerminalCoord position, TerminalSize size)
        : position(position)
        , size(size)
    {}
    short RowBeg() const { return position.row; }
    short RowEnd() const { return position.row + size.height - 1; }
    short ColBeg() const { return position.col; }
    short ColEnd() const { return position.col + size.width - 1; }

    short Width() const {
        return size.width;
    }
    short Height() const {
        return size.height;
    }

protected:
    TerminalCoord position;
    TerminalSize size;
};