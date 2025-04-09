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

    AddClickCallback([this](const MouseContext& ctx) {
        if (!IsSelected()) {
            return SetSelected(true);
        } else {
            return SetSelected(false, true);
        }
    });
    allowUseDoubleClickAsSingleClick = true;
}

bool TerminalGridCell::SetSelected(bool isSelect, bool isForce) {
    bool isChanged = IsSelected() != isSelect;
    if (isForce) {
        selectedWeight = (isSelect ? 1 : 0);
    }
    else {
        if (isSelect) {
            selectedWeight++;
        }
        else {
            selectedWeight = std::max(0, selectedWeight - 1);
        }
    }
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
    return selectedWeight;
}

int TerminalGridCell::SelectedWeight() const {
    return selectedWeight;
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