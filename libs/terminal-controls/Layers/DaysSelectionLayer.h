#pragma once

#include "SelectionLayer.h"
#include "data-field.h"

class DaysSelectionLayer : public SelectionLayer {
public:
    bool Select(TerminalControl* first, TerminalControl* last) override;
    bool StopSelect() override;

    void RemovePrevSelect();
    void ApplyCurSelect(storage::date begDate, storage::date endDate);
protected:
    storage::date prevBegDate;
    storage::date prevEndDate;
    std::vector<TerminalGridCell*> selectedControls;
};