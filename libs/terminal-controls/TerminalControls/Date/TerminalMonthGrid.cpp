#include "TerminalMonthGrid.h"
#include "TerminalGridCell.h"
#include <TerminalLabel.h>
#include "data-view.h"

TerminalMonthGrid::TerminalMonthGrid(const std::vector<Utf8String>& header, DataContainerPtr container, TerminalCoord position)
    : TerminalGrid(header, container, position)
{
    InitData();
}

void TerminalMonthGrid::InitData() {
    for (size_t row_num = 0; row_num < container->RowsCount(); ++row_num) {
        short col = 1;
        auto row = container->GetRow(row_num);

        for (size_t field_num = 0; field_num < row->FieldsCount(); ++field_num) {
            storage::date d = row->GetField<storage::date>(field_num);
            if (d.ok()) {
                auto dayValue = static_cast<unsigned>(d.day());
                std::string day = std::to_string(dayValue);
                if (day.size() < 2) {
                    day = " " + day;
                }
                auto dayCell = TerminalGridCell::Create(day, columns[field_num].width, TerminalCoord{ .row = ONE + (short)row_num, .col = col });
                dayCell->SetData(d);
                dayCell->SetGridPosition(row_num, field_num);
                dayCell->SetGridCellFormatter(&formatter);
                dayCell->AddClickCallback([dayCell, this](const MouseContext& ctx) {
                    FinilizeSelectedCell(dayCell->GridRow(), dayCell->GridCol());
                    return true;
                    });
                dayCell->AddOnSelectedCallback([this](TerminalGridCell* sender) {
                    for (auto& cellSelectedCallback : cellSelectedCallbacks) {
                        cellSelectedCallback(sender);
                    }
                    });
                AddControl(dayCell);
                cells[row_num][field_num] = dayCell;
            } else {
                std::string empty_cell(columns[field_num].width, ' ');
                auto emptyLabel = TerminalLabel::Create(empty_cell, TerminalCoord{ .row = ONE + (short)row_num, .col = col });
                AddControl(emptyLabel);
            }
            col += columns[field_num].width + 1;
        }
    }

}
void TerminalMonthGrid::SetSelectionLayer(SelectionLayer* selectionLayer) {
    for (size_t row = 0; row < cells.size(); ++row) {
        for (size_t col = 0; col < cells[row].size(); ++col) {
            if (cells[row][col]) {
                cells[row][col]->SetSelectionLayer(selectionLayer);
            }
        }
    }
}