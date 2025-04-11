#include "TerminalGridCell.h"


TerminalGridCell::TerminalGridCell(Utf8String label, TerminalCoord position)
    : TerminalLabelFixedWidth(label, position)
{
    AddOnSelectedCallback([this](TerminalGridCell* sender, int prevSelectedWeight) {
        if (sender->IsSelected()) {
            SetFontColor(RGB::Blue);
            if (sender->SelectedWeight() == 1) {
                SetBackgroundColor(RGB::Brightcyan);
            } else {
                SetBackgroundColor(RGB::DarkBrightcyan);
            }
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
    int prevSelectedWeight = selectedWeight;
    
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
    bool isChanged = prevSelectedWeight != selectedWeight;

    if (isChanged) {
        for (auto& selectedCallback : selectedCallbacks) {
            selectedCallback(this, prevSelectedWeight);
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

void TerminalGridCell::SetData(storage::date data) {
    this->data = data;
}

const storage::date& TerminalGridCell::GetData() const {
    return data;
}