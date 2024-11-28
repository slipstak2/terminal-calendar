#include "TerminalCoord.h"
#include <tuple>

TerminalCoord operator - (const TerminalCoord& lhs, const TerminalCoord& rhs) {
    return TerminalCoord{ .row = lhs.row - rhs.row, .col = lhs.col - rhs.col };
}

bool operator == (const TerminalCoord& lhs, const TerminalCoord& rhs) {
    return std::tie(lhs.row, lhs.col) == std::tie(rhs.row, rhs.col);
}