#include "TerminalGridCell.h"


TerminalGridCell::TerminalGridCell(Utf8String label, TerminalCoord position)
    : TerminalLabelFixedWidth(label, position)
{
    AddOnSelectedCallback([this](TerminalGridCell* sender) {
        if (sender->IsSelected()) {
            SetFontColor(FontColor::Blue);
            SetBackgroundColor(BackgroundColor::Brightcyan);
        } else {
            SetFontColor(FontColor::Default);
            SetBackgroundColor(BackgroundColor::Default);
        }
    });

    AddClickCallback([this]() {
        if (!IsSelected()) {
            return SetSelected(true);
        } else {
            return Unselected();
        }
    });
}

bool TerminalGridCell::SetSelected(bool isSelect) {
    bool isChanged = IsSelected() != isSelect;
    if (isSelect) {
        selectedCount++;
    } else {
        selectedCount = std::max(0, selectedCount - 1);
    }
    if (isChanged) {
        for (auto& selectedCallback : selectedCallbacks) {
            selectedCallback(this);
        }
    }
    return isChanged;
}

bool TerminalGridCell::Unselected() {
    bool isChanged = IsSelected();
    selectedCount = 0;
    if (isChanged) {
        for (auto& selectedCallback : selectedCallbacks) {
            selectedCallback(this);
        }
    }
    return isChanged;
}

void TerminalGridCell::AddOnSelectedCallback(GridCellSelectedCallback selectedCallback) {
    selectedCallbacks.push_back(selectedCallback);
}

bool TerminalGridCell::IsSelected() const {
    return selectedCount;
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