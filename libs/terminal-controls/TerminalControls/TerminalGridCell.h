#pragma once
#include "TerminalLabelFixedWidth.h"

class TerminalGridCell;
using GridCellSelectedCallback = std::function<void(TerminalGridCell* sender)>;

class TerminalGridCell : public TerminalLabelFixedWidth {
public:
    DECLARE_KIND(TerminalLabelFixedWidth, TerminalControl::Kind::GRID_CELL)

    DECLARE_CREATE(TerminalGridCell)

    TerminalGridCell(Utf8String label, TerminalCoord position);

public:
    bool SetSelected(bool isSelect, bool isForce = false);
    bool IsSelected() const;
    int SelectedWeight() const;
    void AddOnSelectedCallback(GridCellSelectedCallback selectedCallback);

    void SetGridPosition(size_t gridRow, size_t gridCol);
    size_t GridRow() const;
    size_t GridCol() const;

protected:
    int selectedWeight = 0;

    std::vector<GridCellSelectedCallback> selectedCallbacks;
    size_t gridRow = -1;
    size_t gridCol = -1;
};