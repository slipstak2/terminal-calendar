#pragma once

struct TerminalCoord {
    short row = 0;
    short col = 0;
};

TerminalCoord operator - (const TerminalCoord& lhs, const TerminalCoord& rhs);
bool operator == (const TerminalCoord& lhs, const TerminalCoord& rhs);
