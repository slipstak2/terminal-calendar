#pragma once
#include "TerminalLabelFixedWidth.h"

class TerminalGridCell;
using GridCellSelectedCallback = std::function<void(TerminalGridCell* sender, bool isSelected)>;

class TerminalGridCell : public TerminalLabelFixedWidth {
public:
    DECLARE_KIND(TerminalLabelFixedWidth, TerminalControl::Kind::GRID_CELL)

    DECLARE_CREATE(TerminalGridCell)

    TerminalGridCell(Utf8String label, TerminalCoord position);

public:
    bool SetSelected(bool isCheck);
    bool GetSelected();
    void AddOnSelectedCallback(GridCellSelectedCallback selectedCallback) {
        selectedCallbacks.push_back(selectedCallback);
    }

protected:
    bool isSelected = false;
protected:
    std::vector<GridCellSelectedCallback> selectedCallbacks;
};