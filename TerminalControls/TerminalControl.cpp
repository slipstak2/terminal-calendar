#include "TerminalControl.h"

TerminalControl::TerminalControl(TerminalCoord position, TerminalSize size)
    : TerminalRectangle(position, size)
{
    data.resize(Height(), std::vector<TerminalCell>(Width(), CreateCell(' ')));
}

void TerminalControl::AddControl(TerminalControlPtr control) {
    controls.push_back(control);
}

const TerminalCell& TerminalControl::Get(short row, short col) {
    return data[row][col];
}

void TerminalControl::Flush() {
    FlushSelf();
    FlushControls();
}

void TerminalControl::FlushControls() {
    for (TerminalControlPtr control : controls) {
        control->Flush();
        for (short row = control->RowBeg(); row <= control->RowEnd(); ++row) {
            for (short col = control->ColBeg(); col <= control->ColEnd(); ++col) {
                if (row < data.size() && col < data[row].size()) {
                    data[row][col] = control->Get(row - control->RowBeg(), col - control->ColBeg());
                }
            }
        }
    }
}