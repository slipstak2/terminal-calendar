#include "DaysSelectionLayer.h"
#include "TerminalGridCell.h"
#include "TerminalGrid.h"


bool DaysSelectionLayer::SelectStart(TerminalControl* begControl) {
    if (this->begControl == nullptr) { // Wrong dedicated double click with small mouse move
        this->begControl = begControl;
        TerminalGridCell* begCell = begControl->As<TerminalGridCell>();
        if (begCell != nullptr) {
            isSelection = !begCell->IsSelected();
        }
    }
    return true;
}

bool DaysSelectionLayer::Select(TerminalControl* endControl) {
    TerminalGridCell* begCell = begControl->As<TerminalGridCell>();
    TerminalGridCell* endCell = endControl->As<TerminalGridCell>();
    if (begCell == nullptr || endCell == nullptr) {
        return false;
    }
    RemovePrevSelect();
    ApplyCurSelect(begCell->GetData(), endCell->GetData());

    return true;
}

bool DaysSelectionLayer::SelectStop() {
    prevBegDate = storage::date();
    prevEndDate = storage::date();
    begControl = nullptr;
    selectedControls.clear();
    return true;
}

void DaysSelectionLayer::RemovePrevSelect() {
    for (TerminalGridCell* cell : selectedControls) {
        cell->SetSelected(!isSelection);
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
            bool isOK = false;
            if (isSelection) {
                if (!cell->IsSelected()) {
                    cell->SetSelected(true);
                    isOK = true;
                }
            }
            if (!isSelection) {
                if (cell->IsSelected()) {
                    cell->SetSelected(false);
                    isOK = true;
                }
            }

            if (isOK) {
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