#pragma once
#include "TerminalLabelFixedWidth.h"
#include "data-field.h"

class TerminalGridCell;
using GridCellSelectedCallback = std::function<void(TerminalGridCell* sender, int prevSelectedWeight)>;

class GridCellFormatter;

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
    
    void SetData(storage::date value);
    const storage::date& GetData() const;

    void SetGridCellFormatter(GridCellFormatter* formatter);

protected:
    int selectedWeight = 0;

    std::vector<GridCellSelectedCallback> selectedCallbacks;
    size_t gridRow = -1;
    size_t gridCol = -1;

    storage::date data; // TODO: how to save any template data?

    GridCellFormatter* formatter = nullptr;
};