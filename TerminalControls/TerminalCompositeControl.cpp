#include "TerminalCompositeControl.h"

TerminalCompositeControl::TerminalCompositeControl(TerminalCoord position)
    : TerminalControl(position)
{}

TerminalCompositeControl::TerminalCompositeControl(TerminalCoord position, TerminalSize size)
    : TerminalControl(position, size)
{
    backgroundCell = CreateBackgroundCell(' ');
}

void TerminalCompositeControl::FlushSelf() {
    for (int row = 0; row < Height(); ++row) {
        for (int col = 0; col < Width(); ++col) {
            data[row][col] = backgroundCell;
        }
    }
}

void TerminalCompositeControl::AddControl(TerminalControlPtr control) { //TODO: switch beetween windows
    TerminalControl::AddControl(control);
}
