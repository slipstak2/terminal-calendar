#include "TerminalRectangle.h"

TerminalRectangle::TerminalRectangle(TerminalCoord position)
    : position(position)
{}

TerminalRectangle::TerminalRectangle(TerminalCoord position, TerminalSize size)
    : position(position)
    , size(size)
{}

short TerminalRectangle::RowBeg() const {
    return position.row;
}

short TerminalRectangle::RowEnd() const {
    return position.row + size.height - 1;
}

short TerminalRectangle::ColBeg() const {
    return position.col;
}

short TerminalRectangle::ColEnd() const {
    return position.col + size.width - 1;
}

short TerminalRectangle::Width() const {
    return size.width;
}

short TerminalRectangle::Height() const {
    return size.height;
}
