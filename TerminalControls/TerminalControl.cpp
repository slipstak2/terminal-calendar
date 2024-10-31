#include "TerminalControl.h"

TerminalControl::TerminalControl(TerminalCoord position)
    : TerminalRectangle(position) {

}
TerminalControl::TerminalControl(TerminalCoord position, TerminalSize size)
    : TerminalRectangle(position)
{
    SetSize(size);
}

TerminalControl::TerminalControl(TerminalCoord position, TerminalSize size, FormatSettings formatSettings)
    : TerminalRectangle(position)
    , formatSettings(formatSettings)
{
    SetSize(size);
}

void TerminalControl::SetSize(TerminalSize newSize) {
    size = newSize;
    data.resize(Height());
    for (auto& row : data) {
        row.resize(Width(), CreateCell('~'));
    }
}

void TerminalControl::SetPosition(TerminalCoord newPosition) {
    position = newPosition;
}

void TerminalControl::SetParentWindow(TerminalWindow* newParentWindow) {
    parentWindow = newParentWindow;
    for (auto& control : controls) {
        control->SetParentWindow(parentWindow);
    }
}

void TerminalControl::AddControl(TerminalControlPtr control) {
    control->parent = this;
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

std::vector<TerminalControlPtr>& TerminalControl::GetControls() {
    return controls;
}