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

void TerminalControl::Resize(short rows, short cols) {
    SetSize(TerminalSize{.height = rows, .width = cols});
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

int TerminalControl::RemoveControls(std::vector<TerminalControlPtr>& controls, const FilterControlCallback& filterCallback) {
    auto it = std::remove_if(controls.begin(), controls.end(), filterCallback);
    int result = controls.end() - it;
    controls.erase(it, controls.end());
    return result;
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
        if (!control->IsVisible()) {
            continue;
        }
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

TerminalCoord TerminalControl::GetRelativePosition(TerminalCoord absPosition) {
    if (parent) {
        return parent->GetRelativePosition(absPosition) - position;
    }
    return absPosition - position;
}