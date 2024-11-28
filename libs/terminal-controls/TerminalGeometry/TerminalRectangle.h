#pragma once

#include "TerminalCoord.h"
#include "TerminalSize.h"

class TerminalRectangle {
public:
    TerminalRectangle(TerminalCoord position);
    TerminalRectangle(TerminalCoord position, TerminalSize size);

    short RowBeg() const;
    short RowEnd() const;
    short ColBeg() const;
    short ColEnd() const;

    short Width() const;
    short Height() const;

protected:
    TerminalCoord position;
    TerminalSize size;
};
