#include "TerminalCoord.h"
#include <tuple>

TerminalCoord operator - (const TerminalCoord& lhs, const TerminalCoord& rhs) {
    return TerminalCoord{ .row = lhs.row - rhs.row, .col = lhs.col - rhs.col };
}

TerminalCoord& operator += (TerminalCoord& lhs, const TerminalCoord& rhs) {
    lhs.row += rhs.row;
    lhs.col += rhs.col;

    return lhs;
}

bool operator == (const TerminalCoord& lhs, const TerminalCoord& rhs) {
    return std::tie(lhs.row, lhs.col) == std::tie(rhs.row, rhs.col);
}