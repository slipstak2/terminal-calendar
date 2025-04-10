#pragma once

#include "SelectionLayer.h"
#include "data-field.h"

class DaysSelectionLayer : public SelectionLayer {
public:
    bool SelectStart(TerminalControl* begControl) override;
    bool Select(TerminalControl* endControl) override;
    bool SelectStop() override;

    void RemovePrevSelect();
    void ApplyCurSelect(storage::date begDate, storage::date endDate);
protected:

    bool HackNeedSelectFirstCell(TerminalGridCell* cell);

    bool isSelection = true;

    TerminalControl* begControl = nullptr;

    storage::date prevBegDate;
    storage::date prevEndDate;
    std::vector<TerminalGridCell*> selectedControls;
};