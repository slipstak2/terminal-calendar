#include "DaysSelectionLayer.h"
#include "TerminalGridCell.h"
#include "TerminalGrid.h"

bool DaysSelectionLayer::Select(TerminalControl* begControl, TerminalControl* endControl) {
    TerminalGridCell* begCell = begControl->As<TerminalGridCell>();
    TerminalGridCell* endCell = endControl->As<TerminalGridCell>();
    if (begCell == nullptr || endCell == nullptr) {
        return false;
    }
    RemovePrevSelect();
    ApplyCurSelect(begCell->GetData(), endCell->GetData());

    return true;
}

bool DaysSelectionLayer::StopSelect() {
    prevBegDate = storage::date();
    prevEndDate = storage::date();
    selectedControls.clear();
    return true;
}

void DaysSelectionLayer::RemovePrevSelect() {
    for (TerminalGridCell* cell : selectedControls) {
        cell->SetSelected(false);
        TerminalGrid* grid = cell->GetParent()->As<TerminalGrid>();
        if (grid != nullptr) {
            grid->FinilizeSelectedCell(cell->GridRow(), cell->GridCol());
        }
    }
    selectedControls.clear();
}

void DaysSelectionLayer::ApplyCurSelect(storage::date begDate, storage::date endDate) {
    if (begDate > endDate) {
        std::swap(begDate, endDate);
    }
    for (TerminalControl* control : controls) {
        TerminalGridCell* cell = control->As<TerminalGridCell>();
        if (cell == nullptr) {
            std::cerr << "invalid cell" << std::endl;
            exit(-1);
        }
        storage::date d = cell->GetData();
        if (begDate <= d && d <= endDate) {
            if (cell->SelectedWeight() == 0) {
                cell->SetSelected(true);
                selectedControls.push_back(cell);
                TerminalGrid* grid = cell->GetParent()->As<TerminalGrid>();
                if (grid != nullptr) {
                    grid->FinilizeSelectedCell(cell->GridRow(), cell->GridCol());
                }
            }

        }
    }
    prevBegDate = begDate;
    prevEndDate = endDate;
}