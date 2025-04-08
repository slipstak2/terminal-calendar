#pragma once
#include "TerminalLabelFixedWidth.h"

enum SelectedFlag {
    SINGLE      = 1 << 0,
    WEEK_DAY    = 1 << 1,
    WEEK        = 1 << 2,
    MONTH       = 1 << 3
};

struct SelectedFlags {
    bool Is(SelectedFlag flag);
    void Set(SelectedFlag flag, bool value);
    const int Value();
private:
    int value = 0;
};

class TerminalGridCell;
using GridCellSelectedCallback = std::function<void(TerminalGridCell* sender, SelectedFlags selectedFlags)>;
using GridCellTryUnselectedCallback = std::function<void(TerminalGridCell* sender)>;

class TerminalGridCell : public TerminalLabelFixedWidth {
public:
    DECLARE_KIND(TerminalLabelFixedWidth, TerminalControl::Kind::GRID_CELL)

    DECLARE_CREATE(TerminalGridCell)

    TerminalGridCell(Utf8String label, TerminalCoord position);

public:
    const SelectedFlags& GetSelectedFlags() const;
    bool SetSelected(bool isSelect, SelectedFlag flag);
    bool GetSelected(SelectedFlag flag);
    void AddOnSelectedCallback(GridCellSelectedCallback selectedCallback);
    void AddOnTryUnselectedCallback(GridCellTryUnselectedCallback tryUnselectedCallback);
    void ApplyTryUnselected();

    void SetGridPosition(size_t gridRow, size_t gridCol);
    size_t GridRow() const;
    size_t GridCol() const;

protected:
    SelectedFlags selectedFlags;

    std::vector<GridCellSelectedCallback> selectedCallbacks;
    std::vector<GridCellTryUnselectedCallback> tryUnselectedCallbacks;

    size_t gridRow = -1;
    size_t gridCol = -1;
};