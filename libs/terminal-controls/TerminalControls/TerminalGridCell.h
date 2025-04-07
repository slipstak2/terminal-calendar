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

class TerminalGridCell : public TerminalLabelFixedWidth {
public:
    DECLARE_KIND(TerminalLabelFixedWidth, TerminalControl::Kind::GRID_CELL)

    DECLARE_CREATE(TerminalGridCell)

    TerminalGridCell(Utf8String label, TerminalCoord position);

public:
    bool SetSelected(bool isSelect, SelectedFlag flag);
    bool GetSelected(SelectedFlag flag);
    void AddOnSelectedCallback(GridCellSelectedCallback selectedCallback) {
        selectedCallbacks.push_back(selectedCallback);
    }

protected:
    SelectedFlags selectedFlags;
protected:
    std::vector<GridCellSelectedCallback> selectedCallbacks;
};