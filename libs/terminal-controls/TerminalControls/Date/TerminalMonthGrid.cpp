#include "TerminalMonthGrid.h"
#include "TerminalGridCell.h"

TerminalMonthGrid::TerminalMonthGrid(const std::vector<Utf8String>& header, DataStoragePtr storage, TerminalCoord position)
    : TerminalGrid(header, storage, position)
{}

void TerminalMonthGrid::SetSelectionLayer(SelectionLayer* selectionLayer) {
    for (size_t row = 0; row < cells.size(); ++row) {
        for (size_t col = 0; col < cells[row].size(); ++col) {
            if (cells[row][col]) {
                cells[row][col]->SetSelectionLayer(selectionLayer);
            }
        }
    }
}