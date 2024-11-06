#include "TerminalRectangle.h"

TerminalCoord operator - (const TerminalCoord& lhs, const TerminalCoord& rhs) {
    return TerminalCoord{ .row = lhs.row - rhs.row, .col = lhs.col - rhs.col };
}