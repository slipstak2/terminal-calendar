#include "TerminalGridCell.h"


bool SelectedFlags::Is(SelectedFlag flag) {
    return value & flag;
}

void SelectedFlags::Set(SelectedFlag flag, bool val) {
    if (val) {
        value |= flag;
    } else {
        value &= ~flag;
    }
}

const int SelectedFlags::Value() {
    return value;
}

TerminalGridCell::TerminalGridCell(Utf8String label, TerminalCoord position)
    : TerminalLabelFixedWidth(label, position)
{
    AddOnSelectedCallback([this](TerminalGridCell* sender, SelectedFlags selectedFlags) {
        if (selectedFlags.Value()) {
            SetFontColor(FontColor::Blue);
            SetBackgroundColor(BackgroundColor::Brightcyan);
        } else {
            SetFontColor(FontColor::Default);
            SetBackgroundColor(BackgroundColor::Default);
        }
    });

    AddClickCallback([this]() {
        return SetSelected(!GetSelected(SelectedFlag::SINGLE), SelectedFlag::SINGLE);
    });
}

bool TerminalGridCell::SetSelected(bool isSelect, SelectedFlag flag) {
    bool isChanged = GetSelected(flag) != isSelect;
    if (isChanged) {
        this->selectedFlags.Set(flag, isSelect);
        for (auto& selectedCallback : selectedCallbacks) {
            selectedCallback(this, selectedFlags);
        }
    }
    return isChanged;
}

bool TerminalGridCell::GetSelected(SelectedFlag flag) {
    return selectedFlags.Is(flag);

}