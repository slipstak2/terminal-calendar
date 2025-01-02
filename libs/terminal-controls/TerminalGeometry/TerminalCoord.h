#pragma once

struct TerminalCoord {
    short row = 0;
    short col = 0;
    /*
    TerminalCoord() = default;
    TerminalCoord(short row, short col)
        : row(row)
        , col(col)
    {} */
};

TerminalCoord operator - (const TerminalCoord& lhs, const TerminalCoord& rhs);
TerminalCoord& operator += (TerminalCoord& lhs, const TerminalCoord& rhs);
bool operator == (const TerminalCoord& lhs, const TerminalCoord& rhs);
