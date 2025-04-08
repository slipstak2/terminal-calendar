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
        if (!selectedFlags.Value()) {
            return SetSelected(true, SelectedFlag::SINGLE);
        } else {
            ApplyTryUnselected();
            SetSelected(false, SelectedFlag::SINGLE);
            return true;
        }
    });
}

const SelectedFlags& TerminalGridCell::GetSelectedFlags() const {
    return selectedFlags;
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

void TerminalGridCell::AddOnSelectedCallback(GridCellSelectedCallback selectedCallback) {
    selectedCallbacks.push_back(selectedCallback);
}

void TerminalGridCell::AddOnTryUnselectedCallback(GridCellTryUnselectedCallback tryUnselectedCallback) {
    tryUnselectedCallbacks.push_back(tryUnselectedCallback);
    //void (GridCellTryUnselectedCallback tryUnselectedCallback);
}

void TerminalGridCell::ApplyTryUnselected() {
    for (auto& tryUnselectedCallback : tryUnselectedCallbacks) {
        tryUnselectedCallback(this);
    }
}

void TerminalGridCell::SetGridPosition(size_t gridRow, size_t gridCol) {
    this->gridRow = gridRow;
    this->gridCol = gridCol;
}


size_t TerminalGridCell::GridRow() const {
    return gridRow;
}

size_t TerminalGridCell::GridCol() const {
    return gridCol;
}