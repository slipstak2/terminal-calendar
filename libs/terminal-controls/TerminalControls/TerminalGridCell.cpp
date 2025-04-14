#include "TerminalGridCell.h"
#include "GridCellFormatter.h"


TerminalGridCell::TerminalGridCell(Utf8String label, TerminalCoord position)
    : TerminalLabelFixedWidth(label, position)
{
    AddOnSelectedCallback([this](TerminalGridCell* sender) {
        if (formatter) {
            formatter->Apply(sender);
        }
    });

    AddClickCallback([this](const MouseContext& ctx) {
        return SetSelected(!IsSelected());
    });
    allowUseDoubleClickAsSingleClick = true;
}

bool TerminalGridCell::SetSelected(bool isSelect) {
    bool isChanged = IsSelected() != isSelect;

    if (isChanged) {
        this->isSelected = isSelect;
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
    return isSelected;
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

void TerminalGridCell::SetGridCellFormatter(GridCellFormatter* formatter) {
    this->formatter = formatter;
    formatter->Apply(this);
}